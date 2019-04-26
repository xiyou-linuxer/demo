#ifndef __DemoCLIENT_HPP
#define __DemoCLIENT_HPP

#include <cstdlib>
#include <string>
#include <sstream>
#include <list>
#include "noncopyable.hpp"
#include "define.hpp"
#include "buffer.hpp"
#include "uv.h"

namespace Demo {

class DemoClient final : private noncopyable {
private:
    bool auth = false;
    uv_tcp_t *client_ = nullptr;
    Buffer read_buffer_;

    std::string connected_time_;
    std::string client_addr_;
    int cached_fd_ = -1; // maybe expired, just for logging

public:
    explicit DemoClient(uv_loop_t *loop) {
        client_ = new uv_tcp_t;
        uv_tcp_init(loop, client_);
        client_->data = this;
    }

    ~DemoClient() final {
        delete client_;
    }

    uv_tcp_t *getUvTcp() const {
        return client_;
    }

    uv_stream_t *getUvStream() const {
        return reinterpret_cast<uv_stream_t *>(client_);
    }

    uv_handle_t *getUvHandle() const {
        return reinterpret_cast<uv_handle_t *>(client_);
    }

    void updateClientInfo();

    const std::string &getClientAddr() const {
        return client_addr_;
    }

    Buffer *getReadBuffer() {
        return &read_buffer_;
    }

    std::string toString() const {
        std::ostringstream stream;
        stream << getClientAddr() << "_" << cached_fd_ << "_" << this << "_" << connected_time_;
        return stream.str();
    }

};

} // namespace Demo

#endif //__DEMOCLIENT_HPP
