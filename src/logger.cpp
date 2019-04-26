#include "logger.hpp"

namespace Demo {
namespace Logger {

void getDefaultLoggerConfiguration(el::Configurations &defaultConf) {
    defaultConf.setToDefault();
    defaultConf.setGlobally(el::ConfigurationType::Filename, "demo.log");
    defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "true");
    defaultConf.setGlobally(el::ConfigurationType::Format, "[%datetime] %level (%fbase:%line) [%thread] %msg");
    defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, "20971520"); // 20 MB
}

void setCurrLogLevel(const std::string &level) {
    el::Configurations defaultConf;
    getDefaultLoggerConfiguration(defaultConf);
    if (level == "INFO") {
        defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Warning, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
    } else if (level == "WARN") {
        defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Warning, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
    } else if (level == "ERROR") {
        defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Warning, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
    } else {
        LOG(WARNING) << "Unknown log level: " << level;
    }
    el::Loggers::reconfigureLogger(el::Loggers::getLogger("default"), defaultConf);
}

void setLoggingThreadName(const std::string &name) {
    el::Helpers::setThreadName(name);
}

} // namespace Logger
} // namespace Demo
