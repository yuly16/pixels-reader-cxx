//
// Created by yuliangyong on 2023-03-02.
//
#include "physical/natives/DirectRandomAccessFile.h"

#include "physical/natives/DirectIoLib.h"
#include "utils/ConfigFactory.h"

#include <cstdio>
#include <malloc.h>
DirectRandomAccessFile::DirectRandomAccessFile(const std::string& file) {
    FILE * fp = fopen(file.c_str(), "r");
    // checking if the file exist or not
    if (fp == nullptr) {
        throw std::runtime_error("DirectRandomAccessFile: File not found or fd exceeds the limitation. ");
    }
    fseek(fp, 0L, SEEK_END);
    // calculating the size of the file
    len = ftell(fp);
    // closing the file
    fclose(fp);
    fd = open(file.c_str(), O_RDONLY|O_DIRECT);
    offset = 0;
	fsBlockSize = std::stoi(ConfigFactory::Instance().getProperty("localfs.block.size"));
    bufferValid = false;
	directIoLib = std::make_shared<DirectIoLib>(fsBlockSize);
    try {
		smallDirectBuffer = directIoLib->allocateDirectBuffer(fsBlockSize);
    } catch (...){
        throw std::runtime_error("failed to allocate buffer");
    }
}

void DirectRandomAccessFile::close() {
    largeBuffers.clear();

    if(fd != -1 && ::close(fd) != 0) {
        throw std::runtime_error("File is not closed properly");
    }
    fd = -1;
    offset = 0;
    len = 0;
}

std::shared_ptr<ByteBuffer> DirectRandomAccessFile::readFully(int len) {
	auto directBuffer = directIoLib->allocateDirectBuffer(len);
	auto buffer = directIoLib->read(fd, offset, directBuffer, len);
    seek(offset + len);
    largeBuffers.emplace_back(directBuffer);
    return buffer;
}

long DirectRandomAccessFile::length() {
    return len;
}

void DirectRandomAccessFile::seek(long off) {
    if(bufferValid && off > offset - smallBuffer->getReadPos() &&
            off < offset + smallBuffer->bytesRemaining()) {
        smallBuffer->setReadPos(off - offset + smallBuffer->getReadPos());
    } else {
        bufferValid = false;
    }
    offset = off;
}

long DirectRandomAccessFile::readLong() {
    if(!bufferValid || smallBuffer->bytesRemaining() < sizeof(long)) {
        populatedBuffer();
    }
    offset += sizeof(long);
    return smallBuffer->getLong();
}

int DirectRandomAccessFile::readInt() {
    if(!bufferValid || smallBuffer->bytesRemaining() < sizeof(int)) {
        populatedBuffer();
    }
    offset += sizeof(int);
    return smallBuffer->getInt();
}

char DirectRandomAccessFile::readChar() {
    if(!bufferValid || smallBuffer->bytesRemaining() < sizeof(char)) {
        populatedBuffer();
    }
    offset += sizeof(char);
    return smallBuffer->getChar();
}

void DirectRandomAccessFile::populatedBuffer() {
    smallBuffer = directIoLib->read(fd, offset, smallDirectBuffer, fsBlockSize);
    bufferValid = true;
}




DirectRandomAccessFile::~DirectRandomAccessFile() = default;


