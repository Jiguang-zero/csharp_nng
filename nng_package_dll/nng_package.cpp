#include "nng_package.h"

#include <iostream>
#include <nng/protocol/reqrep0/req.h>

Nng * Nng::NewNngRequest(const char *url) {
    auto* instance = new Nng();
    if (!instance->StartRequestListening(url)) {
        delete instance;
        return nullptr;
    }

    return instance;
}

void Nng::FreeNng(const Nng *nng) {
    delete nng;
}

char * Nng::SendRequestAndGetResponse(const unsigned char *request, const int requestSize) {
    std::lock_guard<std::mutex> lock(operationMutex);
    char * response = nullptr;
    nng_msg *msg = nullptr;

    try {
        // ReSharper disable once CppDFAMemoryLeak
        if (nng_msg_alloc(&msg, 0) != 0) {
            // ReSharper disable once CppDFAMemoryLeak
            return nullptr;
        }

        if (nng_msg_append(msg, request, requestSize) != 0) {
            nng_msg_free(msg);
            // ReSharper disable once CppDFAMemoryLeak
            return nullptr;
        }

        if (nng_sendmsg(*socket, msg, 0) != 0) {
            nng_msg_free(msg);
            // ReSharper disable once CppDFAMemoryLeak
            return nullptr;
        }

        if (nng_recvmsg(*socket, &msg, 0) == 0) {
            const size_t len = nng_msg_len(msg);
            const void* body = nng_msg_body(msg);

            // 分配内存并复制响应内容
            response = new char[len + 1];
            memcpy(response, body, len);
            response[len] = '\0'; // 添加字符串结束符
            nng_msg_free(msg);
        } else {
            nng_msg_free(msg);
        }

        // ReSharper disable once CppDFAMemoryLeak
        return response;
    }
    catch (...) {
        //TODO: Log the error.
        nng_msg_free(msg);
        delete[] response;
        return nullptr;
    }
}

bool Nng::StartRequestListening(const char *url) {
    std::lock_guard<std::mutex> lock(operationMutex);
    if (nng_req0_open(socket) /* != 0 */) {
        CloseConnection();
        return false;
    }
    nng_dialer dialer{};
    if (nng_dialer_create(&dialer, *socket, url) != 0) {
        CloseConnection();
        return false;
    }
    nng_dialer_start(dialer, NNG_FLAG_NONBLOCK);
    return true;
}

void Nng::CloseConnection() {
    std::lock_guard<std::mutex> lock(operationMutex);
    if (socket) {
        nng_socket_close(*socket);
        delete socket;
    }
}

Nng::~Nng() {
    CloseConnection();
}

Nng::Nng() {
    socket = new nng_socket();
}

nng_socket * ConnectRequestSocket(const char *url) {
    auto * socket = new nng_socket();
    // Request type.
    int rv = nng_req0_open(socket);
    if (rv != 0) {
        Release(socket);
        return nullptr;
    }

    nng_dialer dialer{};
    rv = nng_dialer_create(&dialer, *socket, url);
    if (rv != 0) {
        Release(socket);
        return nullptr;
    }
    nng_dialer_start(dialer, NNG_FLAG_NONBLOCK);

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
        return nng_strerror(rv);
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
    int rv = send(socket, request, request_size);
    if (rv != 0) {
        return nullptr;
    }

    char* response_msg = nullptr;
    size_t response_size = 0;
    rv = nng_recv(*socket, &response_msg, &response_size, NNG_FLAG_NONBLOCK | NNG_FLAG_ALLOC);
    if (rv != 0) {
        nng_free(response_msg, response_size);
        return nullptr;
    }
    auto * response = new char[response_size + 1];
    memcpy(response, response_msg, request_size);
    response[request_size + 1] = '\0';

    nng_free(response_msg, response_size);
    return response;
}
