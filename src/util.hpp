#ifndef __UTIL_HPP
#define __UTIL_HPP

#ifdef __APPLE__
#include <mach-o/dyld.h>
#elif __linux__
#include <linux/limits.h>
#include <sys/sysinfo.h>
#endif

#include <unistd.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <cerrno>
#include <cstring>

#include <string>
#include <sstream>
#include <vector>
#include <cmath>

namespace Demo {
namespace Util {

/// 获取指定网络设备的IP
static inline uint32_t getLocalAddr(const char *dev_name)
{
    int             fd = 0;
    struct ifreq    buf[16];
    struct ifconf   ifc;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) <= 0) {
        return 0;
    }
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t) buf;
    if (ioctl(fd, SIOCGIFCONF, (char *) &ifc)) {
        close(fd);
        return 0;
    }
    int n = ifc.ifc_len / sizeof(struct ifreq);
    while (n-- > 0) {
        if (ioctl(fd, SIOCGIFFLAGS, (char *) &buf[n])) {
            continue;
        }
        if (buf[n].ifr_flags & IFF_LOOPBACK)
            continue;
        if (!(buf[n].ifr_flags & IFF_UP))
            continue;
        if (dev_name != nullptr && strcmp(dev_name, buf[n].ifr_name) != 0)
            continue;
        if (!(ioctl(fd, SIOCGIFADDR, (char *) &buf[n]))) {
            close(fd);
            return ((struct sockaddr_in *) (&buf[n].ifr_addr))->sin_addr.s_addr;
        }
    }
    close(fd);
    return 0;
}

/// 获取系统 uname 信息
static inline std::string getUnameInfo()
{
    struct utsname uts;
    char uname_buff[120];

    if (uname(&uts) < 0) {
        return std::string(strerror(errno));
    }
    snprintf(uname_buff, sizeof(uname_buff), "%s %s %s %s", uts.sysname, uts.release, uts.version, uts.machine);

    return std::string(uname_buff);
}

/// 获取 CPU 核数
static inline int getNprocs()
{
    int core_count;

#ifdef __APPLE__
    size_t len = sizeof(core_count);
    sysctlbyname("machdep.cpu.core_count", &core_count, &len, 0, 0);
#elif __linux__
    core_count = get_nprocs();
#endif

    return core_count;
}

/// 获取自身可执行文件的地址
static inline std::string getSelfExeFilePath()
{
    char path[PATH_MAX] = {0};
    bzero(path, sizeof(path));

#ifdef __APPLE__
    uint32_t size = PATH_MAX;
    _NSGetExecutablePath(path, &size);
#elif __linux__
    readlink("/proc/self/exe", path, sizeof(path));
#endif

    return std::string(path);
}

/// uint32_t IP地址和port转换为字符串
static inline std::string address2string(uint32_t addr, uint16_t port) {
    char buf[128];
    char saddr[16];
    inet_ntop(AF_INET, &addr, saddr, sizeof(saddr));
    snprintf(buf, sizeof(buf), "%s:%i", saddr, port);
    return std::string(buf);
}

/// uint32_t IP地址转换为字符串
static inline std::string address2string(uint32_t addr) {
    char buf[128];
    char saddr[16];
    inet_ntop(AF_INET, &addr, saddr, sizeof(saddr));
    snprintf(buf, sizeof(buf), "%s", saddr);
    return std::string(buf);
}

/// 字符串IP地址转换为 uint32_t
static inline uint32_t string2addr(std::string str_addr) {
    struct in_addr addr;
    inet_pton(AF_INET, str_addr.c_str(), &addr);
    return addr.s_addr;
}

/// 字符串分割
static inline std::vector<std::string> stringSplit(const std::string &string, char delim) {
    std::stringstream ss(string);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.emplace_back(item);
    }
    return elems;
}

/// 按照指定格式获取本地时间
static inline std::string getLocalDateTime(const char *time_fmt, struct tm *tm = nullptr) {
    char time_buf[128] = {0};
    time_t now = time(nullptr);
    struct tm now_tm = {0};
    localtime_r(&now, &now_tm);
    strftime(time_buf, sizeof(time_buf), time_fmt, &now_tm);
    if (tm) {
        *tm = now_tm;
    }
    return std::string(time_buf);
}

/// 按照指定格式获取时间
static inline std::string getDateTime(const char *time_fmt, time_t time) {
    char time_buf[128] = {0};
    struct tm now_tm = {0};
    localtime_r(&time, &now_tm);
    strftime(time_buf, sizeof(time_buf), time_fmt, &now_tm);

    return std::string(time_buf);
}

static inline uint16_t bswap16(uint16_t x) {
    return (x << 8) | (x >> 8);
}

static inline uint32_t bswap32(uint32_t x) {
    x = ((x & 0xff00ff00U) >> 8) | ((x & 0x00ff00ffU) << 8);
    return (x >> 16) | (x << 16);
}

static inline uint64_t bswap64(uint64_t x) {
    x = ((x & 0xff00ff00ff00ff00ULL) >> 8) | ((x & 0x00ff00ff00ff00ffULL) << 8);
    x = ((x & 0xffff0000ffff0000ULL) >> 16) | ((x & 0x0000ffff0000ffffULL) << 16);
    return (x >> 32) | (x << 32);
}

}  // namespace Util
}  // namespace Demo

#endif // __UTIL_HPP
