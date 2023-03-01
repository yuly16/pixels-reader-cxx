//
// Created by liyu on 2/27/23.
//

#include <gtest/gtest.h>
#include "physical/storage/LocalFS.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

TEST(HelloTest1, StorageSchemaTest1) {
    // Expect two strings not to be equal
    EXPECT_EQ(LocalFS::file, LocalFS::from("FiLe"));
    EXPECT_NE(LocalFS::s3, LocalFS::from("s4"));
}