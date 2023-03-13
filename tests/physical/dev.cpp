//
// Created by liyu on 2/27/23.
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

TEST(physical, StorageFunctionTest) {
    EXPECT_EQ(Storage::file, Storage::from("FiLe"));
    EXPECT_NE(Storage::s3, Storage::from("s4"));
    EXPECT_EQ(Storage::minio, Storage::fromPath("minio:///hello/world"));
    EXPECT_EQ(Storage::file, Storage::fromPath("file:///home/liyu"));
    EXPECT_TRUE(Storage::isValid("file"));
    EXPECT_FALSE(Storage::isValid("sssss"));

    LocalFS localfs;
    EXPECT_EQ(localfs.getScheme(), Storage::file);
    EXPECT_EQ("file:///home/liyu", localfs.ensureSchemePrefix("/home/liyu"));
    EXPECT_EQ("file:///home/liyu", localfs.ensureSchemePrefix("file:///home/liyu"));
    EXPECT_THROW(localfs.ensureSchemePrefix("sss:///home/liyu"), std::invalid_argument);
    EXPECT_THROW(localfs.ensureSchemePrefix(":///home/liyu"), std::invalid_argument);
    EXPECT_THROW(localfs.ensureSchemePrefix("s3:///home/liyu"), std::invalid_argument);
}

TEST(physical, ByteBufferTest) {
    auto bb1 = std::make_unique<ByteBuffer>(30);
    bb1->printPosition();
    printf("bb1 bytes Remaining: %i\n", bb1->bytesRemaining());
    bb1->putInt(134);
    bb1->printPosition();
    EXPECT_EQ((int)bb1->get(), 134);
    EXPECT_EQ((int)bb1->get(), 0);
    EXPECT_EQ((int)bb1->get(), 0);
    EXPECT_EQ((int)bb1->get(), 0);
}

TEST(physical, DirectRandomAccessFile) {
    DirectRandomAccessFile p("/home/liyu/files/hello.txt");
    ByteBuffer * bb1 = p.readFully(3);
    ByteBuffer * bb2 = p.readFully(1);
    ByteBuffer * bb3 = p.readFully(2);
    ByteBuffer * bb4 = p.readFully(4);
    ByteBuffer * bb5 = p.readFully(5);
    p.close();
}

TEST(physical, PhysicalReader) {
    auto * localfs = new LocalFS();
    PhysicalLocalReader localReader(localfs, "/home/liyu/files/hello.txt");
    ByteBuffer * bb1 = localReader.readFully(3);
    ByteBuffer * bb2 = localReader.readFully(1);
    ByteBuffer * bb3 = localReader.readFully(2);
    ByteBuffer * bb4 = localReader.readFully(4);
    ByteBuffer * bb5 = localReader.readFully(5);
    localReader.seek(10);
    localReader.close();
}

TEST(physical, RandomFile) {
    auto * localfs = new LocalFS();
    std::string path = "/home/liyu/files/file_64M";
    int fd = open(path.c_str(), O_RDONLY);
    long len = 64 * 1024 * 1024;
    auto * buffer = new uint8_t[len];
    if(pread(fd, buffer, len, 0) == -1) {
        throw std::runtime_error("the open file fail!");
    }
    ::close(fd);

    PhysicalLocalReader localReader(localfs, path);

    ByteBuffer * bb1 = localReader.readFully(3);
    ByteBuffer * bb2 = localReader.readFully(1);
    ByteBuffer * bb3 = localReader.readFully(2);
    ByteBuffer * bb4 = localReader.readFully(4);
    ByteBuffer * bb5 = localReader.readFully(5);
    localReader.seek(10);
    localReader.close();
}


TEST(physical, StorageFactory) {
    StorageFactory * sf = StorageFactory::getInstance();
    auto enabledSchemes = sf->getEnabledSchemes();
    EXPECT_EQ(Storage::file, enabledSchemes[0]);
    EXPECT_EQ(1, enabledSchemes.size());
    EXPECT_TRUE(sf->isEnabled(Storage::file));
    EXPECT_FALSE(sf->isEnabled(Storage::s3));
    sf->reloadAll();
    sf->reloadAll();
    std::cout<<"finish"<<std::endl;
}

TEST(physical, PixelsReaderImpl) {
    Storage * storage = StorageFactory::getInstance()->getStorage(Storage::file);
    auto * builder = new PixelsReaderBuilder();
    PixelsReader * pixelsReader = builder
        ->setStorage(storage)
        ->setPath("/home/liyu/files/file_64M")
        ->build();
    std::cout<<"fuck"<<std::endl;

}

TEST(physical, NoopScheduler) {
    Scheduler * noop = SchedulerFactory::Instance()->getScheduler();
    long queryId = 0;
    auto * localfs = new LocalFS();
    RequestBatch batch;
    batch.add(queryId, 5, 9);
    batch.add(queryId, 1, 4);
    batch.add(queryId, 2, 5);
    batch.add(queryId, 3, 7);
    PhysicalLocalReader localReader(localfs, "/home/liyu/files/hello.txt");
    auto bbs = noop->executeBatch(&localReader, batch, queryId);
    std::cout<<"fuck"<<std::endl;
}

TEST(physical, PixelsRecordReaderImpl) {
    Storage * storage = StorageFactory::getInstance()->getStorage(Storage::file);
    auto * builder = new PixelsReaderBuilder();
    PixelsReader * pixelsReader = builder
            ->setStorage(storage)
            ->setPath("/home/liyu/files/file_64M")
            ->build();
    PixelsRecordReader * pixelsRecordReader = pixelsReader->read();
//    auto v = pixelsRecordReader->readBatch(1, false);

}