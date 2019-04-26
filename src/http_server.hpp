#ifndef __HTTPSERVER_HPP
#define __HTTPSERVER_HPP

#include <map>
#include <functional>

#include "logger.hpp"
#include "mongoose.h"
#include "noncopyable.hpp"

namespace Demo {

class HttpServer final : private noncopyable {
private:
    bool started_ = false;
    struct mg_mgr mgr_;
    std::string listen_addr_;
    typedef std::map<std::string, std::function<std::string(std::map<std::string, std::string>)>> URIControllerMap;
    URIControllerMap uriControllerMap_;

private:
    static void ev_handler(struct mg_connection *conn, int ev, void *ev_data);

public:
    HttpServer() = default;
    ~HttpServer() = default;

    bool addUriController(std::string uri, std::function<std::string(std::map<std::string, std::string>)> controller) {
        if (started_) {
            return false;
        }
        uriControllerMap_.insert(std::make_pair(uri, controller));
        return true;
    }

    void start(const std::string &listen_addr) {
        if (started_) {
            return;
        }
        started_ = true;
        mg_mgr_init(&mgr_, &uriControllerMap_);
        listen_addr_ = listen_addr;
        struct mg_connection *conn = mg_bind(&mgr_, listen_addr_.c_str(), ev_handler);
        if (!conn) {
            LOG(ERROR) << "HTTP listen error, addr: " << listen_addr_;
            exit(-1);
        }
        LOG(WARNING) << "HTTP listen successful, addr: " << listen_addr_;
        mg_set_protocol_http_websocket(conn);
        while (started_) {
            mg_mgr_poll(&mgr_, 1000);
        }
    }

    void stop() {
        LOG(WARNING) << "HTTP server exit";
        started_ = false;
        mg_mgr_free(&mgr_);
    }
};

} // namespace Demo

#endif // __HTTPSERVER_HPP
