#include "http_server.hpp"
#include "util.hpp"

namespace Demo {

void HttpServer::ev_handler(struct mg_connection *conn, int ev, void *ev_data) {
    struct http_message *hm = (struct http_message *)ev_data;
    if (ev == MG_EV_HTTP_REQUEST) {
        std::string uri = std::string(hm->uri.p, hm->uri.p + hm->uri.len);
        std::string response;
        std::map<std::string, std::string> args;
        std::string uri_args = std::string(hm->query_string.p, hm->query_string.p + hm->query_string.len);
        auto elems = Util::stringSplit(uri_args, '&');
        for (auto &elem : elems) {
            size_t pos = elem.find('=');
            if (pos != std::string::npos) {
                args[std::string(elem.begin(), elem.begin()+pos)] = std::string(elem.begin()+pos+1, elem.end());
            }
        }

        URIControllerMap *uriControllerMap = reinterpret_cast<URIControllerMap *>(conn->mgr->user_data);
        auto iter = uriControllerMap->find(uri);
        if (iter != uriControllerMap->end()) {
            response = iter->second(args);
        } else {
            response = "404 NOT FOUND";
        }
        mg_printf(conn, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
        mg_printf_http_chunk(conn, response.c_str());
        // send empty chunk, the end of response
        mg_send_http_chunk(conn, "", 0);
    }
}

} // namespace Demo
