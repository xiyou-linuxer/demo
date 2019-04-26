#include "demo_server.hpp"
#include "statistics.hpp"

namespace Demo {

bool DemoServer::initNetwork() {
    Config *config = Config::getInstance();
    started_ = true;
    if (!config->dev_name.empty()) {
        uint32_t addr = Util::getLocalAddr(config->dev_name.c_str());
        if (addr) {
            listen_ip_ = Util::address2string(addr);
            LOG(WARNING) << "Demo listen addr: " << listen_ip_ << ":" << config->listen_port;
        } else {
            LOG(WARNING) << "Cannot get IP from dev[" << config->dev_name << "], listen 0.0.0.0:" << config->listen_port;
        }
    } else {
        LOG(WARNING) << "dev_name is empty, listen 0.0.0.0:" << config->listen_port;
    }
    struct sockaddr_in addr = {0};
    uv_ip4_addr(listen_ip_.c_str(), config->listen_port, &addr);
    int status = uv_tcp_bind(server_, reinterpret_cast<const struct sockaddr *>(&addr), 0);
    if (status != UV_OK) {
        LOG(WARNING) << "Tcp Bind failed: " << uv_strerror(status);
        return false;
    }
    status = uv_tcp_nodelay(server_, 1);
    if (status != UV_OK) {
        LOG(WARNING) << "Tcp set nodelay failed: " << uv_strerror(status);
        return false;
    }
    status = uv_listen(reinterpret_cast<uv_stream_t *>(server_), DEFAULT_BACKLOG, onNewConnection);
    if (status != UV_OK) {
        LOG(WARNING) << "Tcp Listen failed: " << uv_strerror(status);
        return false;
    }
    listen_addr_ = listen_ip_ + ":" + std::to_string(config->listen_port);

    return true;
}

void DemoServer::onNewConnection(uv_stream_t *server, int status) {
    if (status < 0) {
        LOG(WARNING) << "New connection error: " << uv_strerror(status);
        return;
    }
    auto *client = new DemoClient(server->loop);
    if (uv_accept(server, client->getUvStream()) == UV_OK) {
        Statistics::getInstance()->incrConnectionsCount();
        client->updateClientInfo();
        client->getUvHandle()->data = client;
        LOG(INFO) << "Accept a new connection: " << client->toString();
        uv_read_start(client->getUvStream(), allocBuffer, onConnectionRead);
    } else {
        uv_close(client->getUvHandle(), onClose);
    }
}

void DemoServer::allocBuffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    DemoClient *client = reinterpret_cast<DemoClient *>(handle->data);
    Buffer *buffer = client->getReadBuffer();
    size_t readable = buffer->readableBytes();
    if (readable < CLIENT_READ_BUFFER_MAX) {
        buffer->ensureWritableBytes(suggested_size);
        *buf = uv_buf_init(buffer->beginWrite(), static_cast<unsigned int >(buffer->writableBytes()));
    } else {
        LOG(WARNING) << "Client read_buffer.size[" << readable << "]> CLIENT_READ_BUFFER_MAX["
                     << CLIENT_READ_BUFFER_MAX << "], force close it: " << client->toString();
        *buf = uv_buf_init(NULL, 0); // close in on_connection_read
    }
}

void DemoServer::onConnectionRead(uv_stream_t *stream, ssize_t nread, UNUSED const uv_buf_t *buf) {
    if (nread > 0) {
        DemoClient *client = reinterpret_cast<DemoClient *>(stream->data);
        Buffer *buffer = client->getReadBuffer();
        buffer->incrWriteIndex(static_cast<size_t >(nread));

        int decode_ret = 0;
        while (buffer->readableBytes() > 0) {
            // decode
        }
        if (decode_ret == 0) {
            // ...
        } else if (decode_ret == 1) {
            // ...
        } else {
            LOG(WARNING) << "Connection protocol decode error["
                         << decode_ret << "], abort it: " << client->toString();
            uv_close(reinterpret_cast<uv_handle_t *>(stream), onClose);
        }
    } else {
        if (nread < 0) {
            if (nread != UV_EOF) {
                LOG(WARNING) << "Connection read error: " << uv_strerror(static_cast<int>(nread));
            }
            uv_close(reinterpret_cast<uv_handle_t *>(stream), onClose);
        }
    }
}

void DemoServer::writeCallback(uv_write_t *req, int status) {
    if (status < 0) {
        LOG(WARNING) << "Connection write error: " << uv_strerror(status);
    }
    delete req;
}

void DemoServer::onClose(uv_handle_t *handle) {
    Statistics::getInstance()->decrConnectionsCount();
    DemoClient *client = reinterpret_cast<DemoClient *>(handle->data);
    LOG(INFO) << "Close connection: " << client->toString();
    delete client;
}

void DemoServer::onAsyncClose(uv_async_t* handle) {
    DemoServer *server = reinterpret_cast<DemoServer *>(handle->data);
    server->stopUv();
}

} // namespace Demo
