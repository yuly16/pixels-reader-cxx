//
// Created by liyu on 2/27/23.
//

#include <gtest/gtest.h>
#include "physical/storage/LocalFS.h"
#include "physical/natives/ByteBuffer.h"

TEST(physical, StorageFunctionTest) {
    EXPECT_EQ("file", Storage::reverseSchemeMap[Storage::file]);
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
    std::cout<<(int)bb1->get()<<std::endl;
    std::cout<<(int)bb1->get()<<std::endl;
    std::cout<<(int)bb1->get()<<std::endl;
    std::cout<<(int)bb1->get()<<std::endl;
    bb1->printPosition();
}