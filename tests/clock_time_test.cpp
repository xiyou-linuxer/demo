#include "gtest/gtest.h"
#include "clock_time.hpp"

using namespace Demo;

INITIALIZE_EASYLOGGINGPP

TEST(TEST_CLOCKTIMER, ONLY_TEST) {
    ASSERT_NE(ClockTime::rdtsc(), ClockTime::rdtsc());
    ASSERT_NE(ClockTime::rdtsc(), ClockTime::rdtsc());
    ASSERT_NE(ClockTime::rdtsc(), ClockTime::rdtsc());
    ASSERT_NE(ClockTime::rdtsc(), ClockTime::rdtsc());
    ASSERT_NE(ClockTime::rdtsc(), ClockTime::rdtsc());

    ASSERT_GE(ClockTime::nowMs(), ClockTime::nowMs());
    ASSERT_GE(ClockTime::nowMs(), ClockTime::nowMs());
    ASSERT_GE(ClockTime::nowMs(), ClockTime::nowMs());
    ASSERT_GE(ClockTime::nowMs(), ClockTime::nowMs());
    ASSERT_GE(ClockTime::nowMs(), ClockTime::nowMs());

    ASSERT_GE(ClockTime::nowUs(), ClockTime::nowUs());
    ASSERT_GE(ClockTime::nowUs(), ClockTime::nowUs());
    ASSERT_GE(ClockTime::nowUs(), ClockTime::nowUs());
    ASSERT_GE(ClockTime::nowUs(), ClockTime::nowUs());
    ASSERT_GE(ClockTime::nowUs(), ClockTime::nowUs());

    ASSERT_GE(ClockTime::nowNs(), ClockTime::nowNs());
    ASSERT_GE(ClockTime::nowNs(), ClockTime::nowNs());
    ASSERT_GE(ClockTime::nowNs(), ClockTime::nowNs());
    ASSERT_GE(ClockTime::nowNs(), ClockTime::nowNs());
    ASSERT_GE(ClockTime::nowNs(), ClockTime::nowNs());
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
