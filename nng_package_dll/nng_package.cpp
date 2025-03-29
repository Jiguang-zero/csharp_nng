#include "nng_package.h"

#include <iostream>

nng_socket * ConnectUrl(const std::string& url) {
    auto * socket = new nng_socket();
    if (const int res = nng_req0_open(socket); res != 0) {
        nng_socket_close(*socket);
        return nullptr;
    }
    return socket;
}
