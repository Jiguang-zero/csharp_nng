#include "nng_package.h"
#include "util/logger/Logger.h"

#include <iostream>
#include <nng/protocol/reqrep0/req.h>
#include <exception>

Nng * Nng::NewNngRequest(const char *url) {
    auto* instance = new Nng();
    if (!instance->StartRequestListening(url)) {
        delete instance;
        return nullptr;
    }
    instance->url = std::string(url);
    return instance;
}

void Nng::FreeNng(const Nng *nng) {
    delete nng;
}

char * Nng::SendRequestAndGetResponse(const unsigned char *request, const int requestSize) {
    std::lock_guard<std::mutex> lock(operationMutex);
    char * response = nullptr;
    nng_msg *msg = nullptr;
    LOG_LINE(url + ": request: " + std::string(reinterpret_cast<const char*>(request)));

    try {
        int rv;
        // ReSharper disable once CppDFAMemoryLeak
        if (( rv = nng_msg_alloc(&msg, 0)) != 0) {
            LOG_LINE(url + ": nng_msg_alloc failed: " + std::string(GetErrorString(rv)));
            // ReSharper disable once CppDFAMemoryLeak
            return nullptr;
        }

        if ((rv = nng_msg_append(msg, request, requestSize)) != 0) {
            LOG_LINE(url + ": nng_msg_append failed: " + std::string(GetErrorString(rv)));
            nng_msg_free(msg);
            // ReSharper disable once CppDFAMemoryLeak
            return nullptr;
        }

        if (( rv = nng_sendmsg(*socket, msg, 0)) != 0) {
            LOG_LINE(url + ": nng_sendmsg failed: " + std::string(GetErrorString(rv)));
            nng_msg_free(msg);
            // ReSharper disable once CppDFAMemoryLeak
            return nullptr;
        }

        if (( rv = nng_recvmsg(*socket, &msg, 0)) == 0) {
            const size_t len = nng_msg_len(msg);
            const void* body = nng_msg_body(msg);

            response = new char[len + 1];
            memcpy(response, body, len);
            response[len] = '\0';
            LOG_LINE(url + ": get response: " + std::string(response));
            nng_msg_free(msg);
        } else {
            LOG_LINE(url + ": nng_recvmsg failed: " + std::string(GetErrorString(rv)));
            nng_msg_free(msg);
        }

        // ReSharper disable once CppDFAMemoryLeak
        return response;
    }
    catch (std::exception ex) {
        LOG_LINE(url + ": " + ex.what());
        //TODO: Log the error.
        nng_msg_free(msg);
        delete[] response;
        return nullptr;
    }
}

bool Nng::StartRequestListening(const char *url) {
    try {
        std::lock_guard<std::mutex> lock(operationMutex);
        int rv;
        if ((rv = nng_req0_open(socket)) != 0) {
            LOG_LINE(std::string(url) + ": nng_req0_open failed: " + std::string(GetErrorString(rv)));
            CloseConnection();
            return false;
        }
        nng_dialer dialer{};
        if ((rv = nng_dialer_create(&dialer, *socket, url)) != 0) {
            LOG_LINE(std::string(url) + ": nng_dialer_create failed: " + std::string(GetErrorString(rv)));
            CloseConnection();
            return false;
        }
        if ((rv = nng_dialer_start(dialer, 0)) != 0) {
            LOG_LINE(std::string(url) + ": nng_dialer_start failed: " + std::string(GetErrorString(rv)));
            CloseConnection();
            return false;
        }
        LOG_LINE(std::string(url) + " is listening");
        return true;
    }
    catch (std::exception e) {
        LOG_LINE(std::string(url) + " exception happened when trying to start requestListening: " + e.what());
        return false;
    }
}

void Nng::CloseConnection() {
    try {
        std::lock_guard<std::mutex> lock(operationMutex);
        if (socket) {
            nng_socket_close(*socket);

            LOG_LINE(this->url + " close connection");
            delete socket;
        }
    }
    catch (std::exception e) {
        LOG << this->url << " " << "exception happened trying to close connection";
        LOG << e.what() << utils::endl;
    }
}

Nng::~Nng() {
    CloseConnection();
}

int Nng::SetReceiveTimeOut(const int millisecond) {
    return SetSocketReceiveTimeOut(socket, millisecond);
}

int Nng::SetSendTimeOut(int millisecond)
{
    return SetSocketSendTimeOut(socket, millisecond);
}

int Nng::SetReConnectMinTime(int millisecond)
{
    return SetSocketReConnectMinTime(socket, millisecond);
}

int Nng::SetReConnectMaxTime(int millisecond)
{
    return SetSocketReConnectMaxTime(socket, millisecond);
}

std::string Nng::ShowUrl() const
{
    return url;
}


Nng::Nng() {
    socket = new nng_socket();
}

Nng *ConnectRequestSocket(const char *url) {
    auto * nng = Nng::NewNngRequest(url);
    //nng->SetReceiveTimeOut(5000);

    return nng;
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

void Release(const Nng *nng) {
    delete nng;
}

intptr_t GetResponseWithRequest(Nng *nng, const unsigned char *request, const int requestSize) {
    auto * response = nng->SendRequestAndGetResponse(request, requestSize);
    if (!response) {
        return 0;
    }
    return reinterpret_cast<intptr_t>(response);
}

const char * GetErrorString(const int rv) {
    // if rv is not a correct nng_err data, it will return nullptr.
    if ((rv >= 0 && rv <= 31) || rv == 999 || rv == 1000 || rv == 0x10000000 || rv == 0x20000000) {
        return nng_strerror(rv);
    }
    return nullptr;
}

void FreeMessage(const char *msg) {
    delete msg;
}

void SetSendMaxTimeOut(Nng* nng, int millisecond)
{
    if (!nng)   return;
    int rv = nng->SetSendTimeOut(millisecond);
    if (rv != 0) {
        LOG_LINE(nng->ShowUrl() + ": set send time out failed " + GetErrorString(rv));
    }
    else {
        LOG_LINE(nng->ShowUrl() + ": set send time out: " + std::to_string(millisecond));
    }
}

void SetReceiveMaxTimeOut(Nng* nng, int millisecond)
{
    if (!nng)   return;
    int rv = nng->SetReceiveTimeOut(millisecond);
    if (rv != 0) {
        LOG_LINE(nng->ShowUrl() + ": set receive time out failed " + GetErrorString(rv));
    }
    else {
        LOG_LINE(nng->ShowUrl() + ": set receive time out: " + std::to_string(millisecond));
    }
}
