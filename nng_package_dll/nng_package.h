#ifndef NNG_PACKAGE_DLL_LIBRARY_H
#define NNG_PACKAGE_DLL_LIBRARY_H

#include <nng/nng.h>

#ifdef _WIN32
    #ifdef _NNG_EXPORTS
        #define NNG_API __declspec(dllexport)
    #else
        #define NNG_API __declspec(dllimport)
    #endif
#else
    #define NNG_API
#endif

/**
 * connect url
 * @param url const char*
 * @return Pointer Address. nng_socket*
 */
extern "C" NNG_API nng_socket* ConnectRequestSocket(const char *url);

/**
 * Set sending timeout.
 * @param socket nng_socket*
 * @param milliseconds
 * @return return the result of setting sending timeout. 0 is success.
 */
extern "C" NNG_API int SetSocketSendTimeOut(const nng_socket* socket, int milliseconds);

/**
 * Set receiving timeout.
 * @param socket nng_socket*
 * @param milliseconds
 * @return return the result of setting receive timeout. 0 is success
 */
extern "C" NNG_API int SetSocketReceiveTimeOut(const nng_socket* socket, int milliseconds);

/**
 * Set min reconnect time.
 * @param socket
 * @param milliseconds
 * @return
 */
extern "C" NNG_API int SetSocketReConnectMinTime(const nng_socket* socket, int milliseconds);

/**
 * Set max reconnect time.
 * @param socket
 * @param milliseconds
 * @return
 */
extern "C" NNG_API int SetSocketReConnectMaxTime(const nng_socket* socket, int milliseconds);

/**
/**
 * Close Connect
 * @param socket nng_socket*
 */
extern "C" NNG_API void Release(const nng_socket* socket);

/**
 * We will get the result state when we call connect, bind, receive, send, etc...
 * The type of this state is integer. And we can call this function to see what does the state mean.
 * @param rv
 * @return const char*
 */
extern "C" NNG_API const char* GetErrorString(int rv);

/**
 * Send request with the length [size].
 * @param socket const nng_socket*
 * @param data
 * @param size
 * @return return the result of sending.
 */
int send(const nng_socket* socket, const char* data, size_t size);

/**
 *
 * @param socket
 * @param request
 * @param request_size  The size of request
 * @return
 */
extern "C" NNG_API const char* GetReceiveMessage(const nng_socket* socket, const char* request, int request_size);

#endif //NNG_PACKAGE_DLL_LIBRARY_H
