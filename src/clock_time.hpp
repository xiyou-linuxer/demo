#ifndef __CLOCKTIME_HPP
#define __CLOCKTIME_HPP

#include <ctime>
#include <cstdint>

#include "logger.hpp"

namespace Demo {

/**
 * tsc/ns/us/ms time
 */
class ClockTime final {
public:
    /// rdtsc time
    static inline uint64_t rdtsc() {
        uint64_t rdtsc;
        __asm__ __volatile__("rdtsc" : "=A" (rdtsc));
        return rdtsc;
    }

    /// get ns time
    static inline uint64_t nowNs() {
        struct timespec tv = {0};
        int ret = clock_gettime(CLOCK_REALTIME, &tv);
        CHECK(ret != -1) << "clock_gettime error, errno: " << errno << " msg:" << strerror(errno);
        return (tv.tv_sec * (uint64_t) 1000000000 + tv.tv_nsec);
    }

    /// get us time
    static inline uint64_t nowUs() {
        struct timespec tv = {0};
        int ret = clock_gettime(CLOCK_REALTIME, &tv);
        CHECK(ret != -1) << "clock_gettime error, errno: " << errno << " msg:" << strerror(errno);
        return (tv.tv_sec * (uint64_t) 1000000 + tv.tv_nsec / 1000);
    }

    /// get ms time
    static inline uint64_t nowMs() {
        struct timespec tv = {0};
        int ret = clock_gettime(CLOCK_REALTIME, &tv);
        CHECK(ret != -1) << "clock_gettime error, errno: " << errno << " msg:" << strerror(errno);
        return (tv.tv_sec * (uint64_t) 1000 + tv.tv_nsec / 1000000);
    }
};

} // namespace Demo

#endif // __CLOCKTIME_HPP
