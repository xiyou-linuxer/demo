#ifndef __CONFIG_HPP
#define __CONFIG_HPP

#include "noncopyable.hpp"
#include "logger.hpp"
#include "json.hpp"

namespace Demo {

/// 配置管理类
class Config final : private noncopyable {

private:
    Config() = default;

public:
    static Config *getInstance() {
        static Config config;
        return &config;
    }

    bool saveConfigToFile(std::string file_name) {
        if (file_name.empty()) {
            file_name = config_file_name_;
        }
        std::ofstream outfile(file_name);
        if (outfile.fail()) {
            LOG(ERROR) << "Cannot open config file `" << file_name << "`to write: " << strerror(errno);
            return false;
        }
        updateConfig();
        outfile << config_json_.dump(4);
        return true;
    }

    bool loadConfigFromFile(std::string file_name) {
        if (file_name.empty()) {
            file_name = config_file_name_;
        }
        std::ifstream infile(file_name);
        if (infile.fail()) {
            LOG(ERROR) << "Cannot open config file `" << file_name << "`to read: " << strerror(errno);
            return false;
        }
        try {
            infile >> config_json_;
            parseConfig();
        } catch (nlohmann::json::parse_error &ex) {
            LOG(ERROR) << "parse config file error: " << ex.what();
            return false;
        }
        return true;
    }

    void parseConfig() {
        daemon_mode = config_json_["daemon_mode"];
        http_listen_addr = config_json_["http_listen_addr"];
        log_level = config_json_["log_level"];
        listen_port = config_json_["listen_port"];
        dev_name = config_json_["dev_name"];
    }

    void updateConfig() {
        config_json_["daemon_mode"] = daemon_mode;
        config_json_["http_listen_addr"] = http_listen_addr;
        config_json_["log_level"] = log_level;
        config_json_["listen_port"] = listen_port;
        config_json_["dev_name"] = dev_name;
    }

    std::string getConfigJson() {
        updateConfig();
        return config_json_.dump(4);
    }

public:
    bool daemon_mode = false;
    std::string http_listen_addr = "0.0.0.0:9529";
    std::string log_level = "INFO";
    std::string dev_name;
    int listen_port = 6379;

private:
    std::string config_file_name_ = "etc/demo.conf";
    nlohmann::json config_json_;
};

} // namespace Demo

#endif // __CONFIG_HPP
