#include "gtest/gtest.h"
#include "atomic.hpp"

using namespace Demo;

TEST(TEST_ATOMIC_INT32, ONLY_TEST) {
    AtomicInt32 refCount;

    ASSERT_EQ(0, refCount.get());

    ASSERT_EQ(1, refCount.incrementAndGet());
    ASSERT_EQ(2, refCount.incrementAndGet());
    ASSERT_EQ(3, refCount.incrementAndGet());
    ASSERT_EQ(2, refCount.decrementAndGet());
    ASSERT_EQ(1, refCount.decrementAndGet());
    ASSERT_EQ(0, refCount.decrementAndGet());

    ASSERT_EQ(0, refCount.getAndSet(10));
    ASSERT_EQ(10, refCount.get());
    ASSERT_EQ(10, refCount.getAndSet(0));
    ASSERT_EQ(0, refCount.get());

    ASSERT_EQ(0, refCount.getAndAdd(5));
    ASSERT_EQ(5, refCount.get());
    ASSERT_EQ(5, refCount.getAndAdd(-5));
    ASSERT_EQ(0, refCount.get());

    ASSERT_EQ(5, refCount.addAndGet(5));
    ASSERT_EQ(0, refCount.addAndGet(-5));

    refCount.add(5);
    ASSERT_EQ(5, refCount.get());

    refCount.add(-5);
    ASSERT_EQ(0, refCount.get());

    refCount.increment();
    ASSERT_EQ(1, refCount.get());
    refCount.decrement();
    ASSERT_EQ(0, refCount.get());
}

TEST(TEST_ATOMIC_INT64, ONLY_TEST) {
    AtomicInt64 refCount;

    ASSERT_EQ(0, refCount.get());

    ASSERT_EQ(1, refCount.incrementAndGet());
    ASSERT_EQ(2, refCount.incrementAndGet());
    ASSERT_EQ(3, refCount.incrementAndGet());
    ASSERT_EQ(2, refCount.decrementAndGet());
    ASSERT_EQ(1, refCount.decrementAndGet());
    ASSERT_EQ(0, refCount.decrementAndGet());

    ASSERT_EQ(0, refCount.getAndSet(10));
    ASSERT_EQ(10, refCount.get());
    ASSERT_EQ(10, refCount.getAndSet(0));
    ASSERT_EQ(0, refCount.get());

    ASSERT_EQ(0, refCount.getAndAdd(5));
    ASSERT_EQ(5, refCount.get());
    ASSERT_EQ(5, refCount.getAndAdd(-5));
    ASSERT_EQ(0, refCount.get());

    ASSERT_EQ(5, refCount.addAndGet(5));
    ASSERT_EQ(0, refCount.addAndGet(-5));

    refCount.add(5);
    ASSERT_EQ(5, refCount.get());

    refCount.add(-5);
    ASSERT_EQ(0, refCount.get());

    refCount.increment();
    ASSERT_EQ(1, refCount.get());
    refCount.decrement();
    ASSERT_EQ(0, refCount.get());
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
