#include "nng_package.h"

#include <iostream>

nng_socket * ConnectRequestSocket(const char *url) {
    auto * socket = new nng_socket();
    // Request type.
    if (const int res = nng_req0_open(socket); res != 0) {
        Release(socket);
        return nullptr;
    }

    if (nng_dial(*socket, url, nullptr, 0) != 0) {
        nng_socket_close(*socket);
        Release(socket);
        return nullptr;
    }
    return socket;
}

int SetSocketSendTimeOut(const nng_socket *socket, const int milliseconds) {
    return nng_socket_set_ms(*socket, NNG_OPT_SENDTIMEO, milliseconds);
}

int SetSocketReceiveTimeOut(const nng_socket *socket, const int milliseconds) {
    return nng_socket_set_ms(*socket, NNG_OPT_RECVTIMEO, milliseconds);
}

int SetSocketReConnectMinTime(const nng_socket *socket, const int milliseconds) {
    return nng_socket_set_int(*socket, NNG_OPT_RECONNMINT, milliseconds);
}

int SetSocketReConnectMaxTime(const nng_socket *socket, const int milliseconds) {
    return nng_socket_set_int(*socket, NNG_OPT_RECONNMAXT, milliseconds);
}

void Release(const nng_socket *socket) {
    nng_socket_close(*socket);
    delete socket;
}

const char * GetErrorString(const int rv) {
    // if rv is not a correct nng_err data, it will return nullptr.
    if ((rv >= 0 && rv <= 31) || rv == 999 || rv == 1000 || rv == 0x10000000 || rv == 0x20000000) {
        const auto rv_enum = static_cast<nng_err>(rv);
        return nng_strerror(rv_enum);
    }
    return nullptr;
}

int send(const nng_socket *socket, const char *data, const size_t size) {
    nng_msg* msg = nullptr;

    // ReSharper disable once CppDFAMemoryLeak
    int rv = nng_msg_alloc(&msg, 0);

    if (rv != 0) {
        // ReSharper disable once CppDFAMemoryLeak
        return rv;
    }

    rv = nng_msg_append(msg, data, size);
    if (rv != 0) {
        nng_msg_free(msg);
        return rv;
    }

    rv = nng_sendmsg(*socket, msg, NNG_FLAG_NONBLOCK);
    nng_msg_free(msg);

    return rv;
}

const char * GetReceiveMessage(const nng_socket *socket, const char *request, const int request_size) {
    nng_msg *msg = nullptr;

    int rv = send(socket, request, request_size);
    if (rv != 0) {
        return nullptr;
    }

    rv = nng_recvmsg(*socket, &msg, NNG_FLAG_NONBLOCK);
    if (rv != 0) {
        nng_msg_free(msg);
        return nullptr;
    }
    const auto response_size = nng_msg_len(msg);
    auto * response = new char[response_size + 1];
    memcpy(response, nng_msg_body(msg), request_size);
    response[request_size + 1] = '\0';

    nng_msg_free(msg);
    return response;
}
