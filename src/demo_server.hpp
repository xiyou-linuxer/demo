#ifndef __DemoSERVER_HPP
#define __DemoSERVER_HPP

#include <unistd.h>
#include "demo_client.hpp"
#include "noncopyable.hpp"
#include "logger.hpp"
#include "config.hpp"
#include "util.hpp"
#include "define.hpp"
#include "uv.h"

namespace Demo {

class DemoServer final : private noncopyable {
private:
    bool started_ = false;
    uv_loop_t *loop_ = nullptr;
    uv_tcp_t *server_;
    uv_async_t *close_async_;
    std::string listen_ip_ = "0.0.0.0";
    std::string listen_addr_;

private:
    bool initNetwork();
    static void onNewConnection(uv_stream_t *server, int status);
    static void allocBuffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
    static void onConnectionRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
    static void writeCallback(uv_write_t *req, int status);
    static void onClose(uv_handle_t *handle);
    static void onAsyncClose(uv_async_t* handle);

private:
    DemoServer() {
        loop_ = new uv_loop_t;
        server_ = new uv_tcp_t;
        close_async_ = new uv_async_t;
        uv_loop_init(loop_);
        uv_tcp_init(loop_, server_);
        uv_async_init(loop_, close_async_, onAsyncClose);
        close_async_->data = this;
    }

    ~DemoServer() final {
        uv_loop_close(loop_);
        delete close_async_;
        delete server_;
        delete loop_;
    }

    void stopUv() {
        uv_stop(loop_);
    }

public:
    static DemoServer *getInstance() {
        static DemoServer server;
        return &server;
    }

    void start() {
        if (!started_) {
            started_ = true;
            if (!initNetwork()) {
                return;
            }
            uv_run(loop_, UV_RUN_DEFAULT);
        }
    }

    void stop() {
        if (started_) {
            started_ = false;
            if (close_async_) {
                uv_async_send(close_async_);
            }
            LOG(WARNING) << "Demo exit";
        }
    }

    const std::string &getListenAddr() const {
        return listen_addr_;
    }

};

} // namespace Demo

#endif // __DEMOSERVER_HPP
