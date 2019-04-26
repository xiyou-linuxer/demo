#include "http_service.hpp"

#include "demo_server.hpp"
#include "config.hpp"
#include "statistics.hpp"
#include "json.hpp"

namespace Demo {

inline const char *nullCheck(const char *item) {
    return item ? item : "(null)";
}

void HttpService::addUriControls() {
    httpServer_.addUriController("/", [](std::map<std::string, std::string> args) -> std::string {
        return "It Works!\n\n"
               "    /status -- show status info\n"
               "    /config -- show configuration\n\n";
    });

    httpServer_.addUriController("/config", [](std::map<std::string, std::string> args) -> std::string {
        return Config::getInstance()->getConfigJson();
    });

    httpServer_.addUriController("/status", [&](std::map<std::string, std::string> args) -> std::string {
        nlohmann::json general_json;
        general_json["start_time"] = start_time_;
        char hostname[128];
        if (gethostname(hostname, sizeof(hostname) - 1) == 0) {
            general_json["hostname"] = hostname;
        } else {
            general_json["hostname"] = strerror(errno);
        }
        general_json["uname"] = Util::getUnameInfo();
        general_json["cpu_cores"] = Util::getNprocs();
        general_json["exe_path"] =  Util::getSelfExeFilePath();
        general_json["pid"] = getpid();
        general_json["listen_addr"] = DemoServer::getInstance()->getListenAddr();

        Statistics *stat = Statistics::getInstance();
        nlohmann::json stat_json;
        stat_json["connections_count"] = stat->getConnectionsCount();

        nlohmann::json root_json;
        root_json["general"] = general_json;
        root_json["stat"] = stat_json;

        return root_json.dump(4);
    });

    httpServer_.addUriController("/log_info", [](std::map<std::string, std::string> args) -> std::string {
        Logger::setCurrLogLevel("INFO");
        nlohmann::json root_json;
        root_json["code"] = 0;
        root_json["message"] = "success";

        return root_json.dump(4);
    });

    httpServer_.addUriController("/log_warn", [](std::map<std::string, std::string> args) -> std::string {
        Logger::setCurrLogLevel("WARN");
        nlohmann::json root_json;
        root_json["code"] = 0;
        root_json["message"] = "success";

        return root_json.dump(4);
    });

}

} // namespace Demo
