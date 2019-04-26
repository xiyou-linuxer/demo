#ifndef __HTTPSERVICE_HPP
#define __HTTPSERVICE_HPP

#include <thread>

#include "http_server.hpp"
#include "noncopyable.hpp"
#include "util.hpp"

namespace Demo {

class HttpService final : private noncopyable {
private:
    std::string start_time_;
    bool started_ = false;
    HttpServer httpServer_;
    std::thread *httpThread_ = nullptr;

    void addUriControls();

private:
    HttpService() {
        addUriControls();
    }

public:
    static HttpService *getInstance() {
        static HttpService service;
        return &service;
    }

    void start(const std::string &listen_addr) {
        if (!started_) {
            started_ = true;
            start_time_ = Util::getLocalDateTime("%Y-%m-%d %H:%M:%S");
            httpThread_ = new std::thread([&]() {
                Logger::setLoggingThreadName("http_service");
                httpServer_.start(listen_addr);
            });
        }
    }

    void stop() {
        if (started_) {
            started_ = false;
            httpServer_.stop();
            httpThread_->join();
            delete httpThread_;
        }
    }
};

} // namespace Demo

#endif // __HTTPSERVICE_HPP
