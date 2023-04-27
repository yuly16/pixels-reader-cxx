//
// Created by yuly on 19.04.23.
//
#include "physical/natives/DirectIoLib.h"


DirectIoLib::DirectIoLib(int fsBlockSize) {
	this->fsBlockSize = fsBlockSize;
	this->fsBlockNotMask = ~((long) fsBlockSize - 1);
}

std::shared_ptr<ByteBuffer> DirectIoLib::allocateDirectBuffer(long size) {
	int toAllocate = blockEnd(size) + (size == 1? 0: fsBlockSize);
	uint8_t * directBufferPointer;
	posix_memalign((void **)&directBufferPointer, fsBlockSize, toAllocate);
	auto directBuffer = std::make_shared<ByteBuffer>(directBufferPointer, toAllocate, false);
	return directBuffer;
}

std::shared_ptr<ByteBuffer> DirectIoLib::read(int fd, long fileOffset,
                                              std::shared_ptr<ByteBuffer> directBuffer, long length) {
	// the file will be read from blockStart(fileOffset), and the first fileDelta bytes should be ignored.
	long fileOffsetAligned = blockStart(fileOffset);
	long toRead = blockEnd(fileOffset + length) - blockStart(fileOffset);
	if(pread(fd, directBuffer->getPointer(), toRead, fileOffsetAligned) == -1) {
		throw InvalidArgumentException("DirectIoLib::read: pread fail. ");
	}
	auto bb = std::make_shared<ByteBuffer>(*directBuffer,
	                                       fileOffset - fileOffsetAligned, length);
	return bb;
}


long DirectIoLib::blockStart(long value) {
	return (value & fsBlockNotMask);
}

long DirectIoLib::blockEnd(long value) {
	return (value + fsBlockSize - 1) & fsBlockNotMask;
}

void DirectIoLib::uringSubmitReadRequest(io_uring &ring, int fd, int idx, long fileOffset,
                                              std::shared_ptr<ByteBuffer> directBuffer, long length) {
	// the file will be read from blockStart(fileOffset), and the first fileDelta bytes should be ignored.
	long fileOffsetAligned = blockStart(fileOffset);
	long toRead = blockEnd(fileOffset + length) - blockStart(fileOffset);
	auto bb = new ByteBuffer(*directBuffer,fileOffset - fileOffsetAligned, length);
	uringData * data = new uringData{idx, bb};
	struct io_uring_sqe * sqe = io_uring_get_sqe(&ring);
	io_uring_prep_read(sqe, fd, directBuffer->getPointer(), toRead, fileOffsetAligned);
	io_uring_sqe_set_data(sqe, data);
	io_uring_submit(&ring);
}

std::pair<int, std::shared_ptr<ByteBuffer>>  DirectIoLib::uringGetCompletion(io_uring &ring) {
	struct io_uring_cqe *cqe;
	int ret = io_uring_wait_cqe(&ring, &cqe);
	if(ret < 0) {
		throw InvalidArgumentException("DirectIoLib::uringGetCompletion: io_uring_wait_cqe error.");
	}
	if (cqe->res < 0) {
		throw InvalidArgumentException("DirectIoLib::uringGetCompletion: async read fail.");
	}
	uringData * data = (uringData *)io_uring_cqe_get_data(cqe);
	io_uring_cqe_seen(&ring, cqe);
	std::pair<int, std::shared_ptr<ByteBuffer>> result = {data->idx, std::shared_ptr<ByteBuffer>(data->bb)};
	data->bb = nullptr;
	delete data;
	return result;
}
