//
// Created by liyu on 5/25/23.
//

#include "physical/BufferPool.h"

thread_local int BufferPool::colCount = 0;
thread_local int BufferPool::rgCount = 0;
thread_local std::vector<std::map<uint32_t, uint64_t>> BufferPool::nrBytes;
thread_local bool BufferPool::isInitialized = false;
thread_local std::vector<std::map<uint32_t, std::shared_ptr<ByteBuffer>>> BufferPool::buffers;
std::shared_ptr<DirectIoLib> BufferPool::directIoLib;

BufferPool &BufferPool::Instance() {
	static BufferPool INSTANCE;
	return INSTANCE;
}


void BufferPool::Initialize(uint32_t rgCount, std::vector<uint32_t> colIds, std::vector<uint64_t> bytes) {
	assert(colIds.size() == bytes.size());
	int fsBlockSize = std::stoi(ConfigFactory::Instance().getProperty("localfs.block.size"));
	if(!BufferPool::isInitialized) {
		directIoLib = std::make_shared<DirectIoLib>(fsBlockSize);
		BufferPool::nrBytes.resize(rgCount);
		BufferPool::buffers.resize(rgCount);
		for(int rgId = 0; rgId < rgCount; rgId++) {
			for(int i = 0; i < colIds.size(); i++) {
				uint32_t colId = colIds.at(i);
				uint64_t byte = bytes.at(i);
				auto buffer = BufferPool::directIoLib->allocateDirectBuffer(byte + static_cast<int64_t>(EXTRA_POOL_SIZE));
				BufferPool::nrBytes[rgId][colId] = buffer->size();
				BufferPool::buffers[rgId][colId] = buffer;
			}
		}
		BufferPool::rgCount = rgCount;
		BufferPool::colCount = bytes.size();
		BufferPool::isInitialized = true;
	} else {
		// check if resize the buffer is needed
		assert(rgCount == BufferPool::rgCount);
		assert(colIds.size() == BufferPool::colCount);
		for(int rgId = 0; rgId < rgCount; rgId++) {
			for (int i = 0; i < colIds.size(); i++) {
				uint32_t colId = colIds.at(i);
				uint64_t byte = bytes.at(i);
				if (BufferPool::nrBytes[rgId].find(colId) == BufferPool::nrBytes[rgId].end()) {
					throw InvalidArgumentException("BufferPool::Initialize: no such the column id.");
				}
				// Note: this code should never happen in the pixels scenario
				if (BufferPool::nrBytes[rgId][colId] < byte) {
					throw InvalidArgumentException("the new buffer byte cannot larger than the previous buffer byte. ");
				}
			}
		}
	}
}

std::shared_ptr<ByteBuffer> BufferPool::GetBuffer(uint32_t rgId, uint32_t colId) {
	return BufferPool::buffers.at(rgId)[colId];
}

void BufferPool::Reset() {
	BufferPool::isInitialized = false;
	BufferPool::nrBytes.clear();
	BufferPool::buffers.clear();
	BufferPool::rgCount = 0;
	BufferPool::colCount = 0;
}
