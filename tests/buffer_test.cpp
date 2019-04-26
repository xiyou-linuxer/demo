#include "gtest/gtest.h"
#include "buffer.hpp"

using namespace Demo;

TEST(TEST_BUFFER, ONLY_TEST) {
    Buffer buffer;
    buffer.append("abc", 3);
    ASSERT_EQ(buffer.readableBytes(), 3);
    ASSERT_EQ(buffer.readInt8(), 'a');
    ASSERT_EQ(buffer.readableBytes(), 2);
    ASSERT_EQ(buffer.peekInt8(), 'b');
    ASSERT_EQ(buffer.readableBytes(), 2);
    buffer.retrieveAll();
    ASSERT_EQ(buffer.readableBytes(), 0);
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
