#include "demo_client.hpp"
#include "util.hpp"

namespace Demo {

void DemoClient::updateClientInfo() {
    connected_time_ = Util::getLocalDateTime("%Y-%m-%d~%T");
    char address[32] = {0};
    struct sockaddr_in addr = {0};
    int len = sizeof(addr);
    int status = uv_tcp_getpeername(getUvTcp(), reinterpret_cast<struct sockaddr *>(&addr), &len);
    if (status == UV_OK) {
        uv_ip4_name(&addr, address, sizeof(address));
        client_addr_ = std::string(address) + ":" + std::to_string(addr.sin_port);
        cached_fd_ = client_->io_watcher.fd;
    } else {
        LOG(WARNING) << "Get peer name error: " << uv_strerror(status);
    }
}

} // namespace Demo