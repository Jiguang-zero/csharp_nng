#ifndef NNG_PACKAGE_DLL_LIBRARY_H
#define NNG_PACKAGE_DLL_LIBRARY_H

#include <mutex>
#include <nng/nng.h>

#include "extern_define.h"

class Nng {
public:
 // create a nng instance of request type.
 static Nng* NewNngRequest(const char* url);

 // free memory
 static void FreeNng(const Nng* nng);

 /**
  *
  * @param request The request.
  * @param requestSize The length of the request.
  * @return return the response if successful and nullptr if not.
  */
 char* SendRequestAndGetResponse(const unsigned char* request, int requestSize);

 Nng(const Nng&) = delete;
 bool operator = (const Nng&) = delete;

 // Start request listening. dialer
 bool StartRequestListening(const char* url);

 // Disconnect the socket.
 void CloseConnection();

 ~Nng();

 int SetReceiveTimeOut(int millisecond);

private:
 Nng();

 // Any operation about socket.
 // Including connection, disconnection, send, receive and or so.
 std::mutex operationMutex;

 nng_socket * socket;
};



/**
 * connect url
 * @param url const char*
 * @return Pointer Address. nng_socket*
 */
extern "C" NNG_API Nng *ConnectRequestSocket(const char *url);

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
 * @param nng nng_socket*
 */
extern "C" NNG_API void Release(const Nng *nng);

/**
 * Get response with the request.
 * @param nng
 * @param request
 * @param requestSize
 * @return intptr_t, which can cast to unsigned char*.
 */
extern "C" NNG_API intptr_t GetResponseWithRequest(Nng * nng, const unsigned char* request, int requestSize);

/**
 * We will get the result state when we call connect, bind, receive, send, etc...
 * The type of this state is integer. And we can call this function to see what does the state mean.
 * @param rv
 * @return const char*
 */
extern "C" NNG_API const char* GetErrorString(int rv);

extern "C" NNG_API void FreeMessage(const char* msg);

#endif //NNG_PACKAGE_DLL_LIBRARY_H
