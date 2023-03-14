//
// Created by liyu on 3/13/23.
//
#include <gtest/gtest.h>
#include "physical/storage/LocalFS.h"
#include "physical/natives/ByteBuffer.h"
#include "physical/natives/DirectRandomAccessFile.h"
#include "physical/io/PhysicalLocalReader.h"
#include "physical/StorageFactory.h"
#include "PixelsReaderImpl.h"
#include "PixelsReaderBuilder.h"
#include <iostream>
#include <future>
#include <thread>
#include "physical/scheduler/NoopScheduler.h"
#include "physical/SchedulerFactory.h"
#include "PixelsVersion.h"
#include "PixelsFooterCache.h"
#include "exception/PixelsReaderException.h"

TEST(reader, ByteBufferPopulateChar) {
    // randomly generate a file
    std::string path = "/home/liyu/files/file_1G";
    // read target data
    FILE * fp = fopen(path.c_str(), "r");
    // checking if the file exist or not
    if (fp == nullptr) {
        throw std::runtime_error("File not found");
    }
    fseek(fp, 0L, SEEK_END);
    long length = ftell(fp);
    fclose(fp);
    char * target = new char[length];
    int fd = open(path.c_str(), O_RDONLY);
    if(pread(fd, target, length, 0) == -1) {
        throw std::runtime_error("pread fail");
    }
    close(fd);
    // read the data via physicalReader;
    Storage * storage = StorageFactory::getInstance()->getStorage(Storage::file);
    PhysicalReader * fsReader = PhysicalReaderUtil::newPhysicalReader(
            storage, path);

    std::cout<<"start testing..."<<std::endl;
    for(long i = 0; i < length; i++) {
        EXPECT_EQ(fsReader->readChar(), target[i]);
    }
}

TEST(reader, PixelsVersion) {
    EXPECT_EQ(PixelsVersion::V1, PixelsVersion::currentVersion());
    EXPECT_EQ(PixelsVersion::V1, PixelsVersion::from(1));
}

 TEST(reader, recordReader) {
    std::string dataset = "/home/liyu/pixels-reader-cxx/tests/data/20230224150144_3.pxl";
    PixelsFooterCache footerCache;
    auto * builder = new PixelsReaderBuilder;
    Storage * storage = StorageFactory::getInstance()->getStorage(Storage::file);
    PixelsReader * pixelsReader = builder
            ->setPath(dataset)
            ->setStorage(storage)
            ->setPixelsFooterCache(footerCache)
            ->build();
    PixelsRecordReader * pixelsRecordReader = pixelsReader->read();
    VectorizedRowBatch v = pixelsRecordReader->readBatch(1, false);
}

TEST(reader, fileTail) {
    std::string path = "/home/liyu/pixels-reader-cxx/tests/data/20230224150144_3.pxl";
    Storage * storage = StorageFactory::getInstance()->getStorage(Storage::file);
    PhysicalReader * fsReader = PhysicalReaderUtil::newPhysicalReader(
            storage, path);
    pixels::proto::FileTail fileTail;

    if(fsReader == nullptr) {
        throw std::runtime_error("Failed to create PixelsReader due to error of creating PhysicalReader");
    }

    PixelsFooterCache pixelsFooterCache;
    std::string filename = fsReader->getName();
    EXPECT_EQ(filename, "20230224150144_3.pxl");
    for(int i = 0; i < 2; i++) {
        if(pixelsFooterCache.containsFileTail(filename)) {
            EXPECT_EQ(i, 1);
            fileTail = pixelsFooterCache.getFileTail(filename);
        } else {
            // get FileTail
            EXPECT_EQ(i, 0);
            long fileLen = fsReader->getFileLength();
            fsReader->seek(fileLen - (long)sizeof(long));
            long fileTailOffset = fsReader->readLong();
            int fileTailLength = (int) (fileLen - fileTailOffset - sizeof(long));
            fsReader->seek(fileTailOffset);
            ByteBuffer * fileTailBuffer = fsReader->readFully(fileTailLength);
            if(!fileTail.ParseFromArray(fileTailBuffer->getPointer(),
                                        fileTailLength)) {
                throw std::runtime_error("paring FileTail error!");
            }
            pixelsFooterCache.putFileTail(filename, fileTail);
        }
    }


}

