#ifndef __STATISTICS_HPP
#define __STATISTICS_HPP

#include "atomic.hpp"
#include "noncopyable.hpp"

namespace Demo {

class Statistics final : private noncopyable {
private:
    AtomicInt32 connections_count_;

public:
    static Statistics *getInstance() {
        static Statistics statistics;
        return &statistics;
    }

    int getConnectionsCount() {
        return connections_count_.get();
    }

    void incrConnectionsCount() {
        connections_count_.increment();
    }

    void decrConnectionsCount() {
        connections_count_.decrement();
    }
};

} // namespace Demo

#endif // __STATISTICS_HPP
