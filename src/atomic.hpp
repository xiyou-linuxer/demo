#ifndef __ATOMIC_HPP
#define __ATOMIC_HPP

#include <cstdint>
#include "noncopyable.hpp"

namespace Demo {

template<typename T>
class AtomicInt final : private noncopyable {
public:
    AtomicInt() : value_(0) {}

    T get() {
        // in g++ >= 4.7: __atomic_load_n(&value_, __ATOMIC_SEQ_CST)
        return __sync_val_compare_and_swap(&value_, 0, 0);
    }

    T getAndAdd(T x) {
        return __sync_fetch_and_add(&value_, x);
    }

    T addAndGet(T x) {
        return getAndAdd(x) + x;
    }

    T incrementAndGet() {
        return addAndGet(1);
    }

    T decrementAndGet() {
        return addAndGet(-1);
    }

    void add(T x) {
        getAndAdd(x);
    }

    void increment() {
        incrementAndGet();
    }

    void decrement() {
        decrementAndGet();
    }

    T getAndSet(T newValue) {
        return __sync_lock_test_and_set(&value_, newValue);
    }

private:
    volatile T value_;
};

typedef AtomicInt<int32_t> AtomicInt32;
typedef AtomicInt<int64_t> AtomicInt64;

} // namespace Demo

#endif // __ATOMIC_HPP
