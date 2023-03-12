//
// Created by yuliangyong on 2023-03-02.
//
#include "physical/natives/DirectRandomAccessFile.h"
#include <cstdio>
#include <malloc.h>
DirectRandomAccessFile::DirectRandomAccessFile(const std::string& file) {
    FILE * fp = fopen(file.c_str(), "r");
    // checking if the file exist or not
    if (fp == nullptr) {
        throw std::runtime_error("File not found");
    }
    fseek(fp, 0L, SEEK_END);
    // calculating the size of the file
    len = ftell(fp);
    // closing the file
    fclose(fp);
    // TODO: when the code runs on server, oflag should be changed to O_DIRECT
    fd = open(file.c_str(), O_RDONLY);
    offset = 0;
    // TODO: blockSize should read from pixels.properties.
    blockSize = 4096;
    bufferValid = false;
    try {
        smallBuffer = new ByteBuffer(blockSize);
    } catch (...){
        throw std::runtime_error("failed to allocate buffer");
    }
}

void DirectRandomAccessFile::close() {
    if(smallBuffer != nullptr) {
        delete smallBuffer;
        smallBuffer = nullptr;
    }
    largeBuffers.clear();
    if(fd != -1 && ::close(fd) != 0) {
        throw std::runtime_error("File is not closed properly");
    }
    fd = -1;
    offset = 0;
    len = 0;
}

ByteBuffer * DirectRandomAccessFile::readFully(int len_) {
    auto * buffer = new uint8_t[len_];
    if(pread(fd, buffer, len_, offset) == -1) {
        throw std::runtime_error("pread fail");
    }
    auto * bb = new ByteBuffer(buffer, static_cast<uint32_t>(len_));
    seek(offset + len_);
    largeBuffers.push_back(bb);
    return bb;
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


DirectRandomAccessFile::~DirectRandomAccessFile() = default;


