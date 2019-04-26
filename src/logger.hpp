#ifndef __LOGGER_HPP
#define __LOGGER_HPP

#include "easylogging++.h"
#include "noncopyable.hpp"

namespace Demo {
namespace Logger {

void getDefaultLoggerConfiguration(el::Configurations &defaultConf);
void setCurrLogLevel(const std::string &level);
void setLoggingThreadName(const std::string &name);

/// 日志初始化配置
class LoggerInitializer final : private noncopyable {
public:
    LoggerInitializer() {
        el::Configurations defaultConf;
        getDefaultLoggerConfiguration(defaultConf);
        el::Loggers::reconfigureLogger(el::Loggers::getLogger("default"), defaultConf);
        setCurrLogLevel("WARN");
    }

    ~LoggerInitializer() = default;
};

} // namespace Logger
} // namespace Demo

#endif // __LOGGER_HPP
