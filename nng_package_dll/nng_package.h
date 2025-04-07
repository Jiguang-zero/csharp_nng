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
 int SetSendTimeOut(int millisecond);
 int SetReConnectMinTime(int millisecond);
 int SetReConnectMaxTime(int millisecond);

 std::string ShowUrl() const;

private:
 Nng();

 std::string url;

 // Any operation about socket.
 // Including connection, disconnection, send, receive and or so.
 std::mutex operationMutex;

 nng_socket * socket;
};


/**
 * Set sending timeout.
 * @param socket nng_socket*
 * @param milliseconds
 * @return return the result of setting sending timeout. 0 is success.
 */
int SetSocketSendTimeOut(const nng_socket* socket, int milliseconds);

/**
 * Set receiving timeout.
 * @param socket nng_socket*
 * @param milliseconds
 * @return return the result of setting receive timeout. 0 is success
 */
int SetSocketReceiveTimeOut(const nng_socket* socket, int milliseconds);

/**
 * Set min reconnect time.
 * @param socket
 * @param milliseconds
 * @return
 */
int SetSocketReConnectMinTime(const nng_socket* socket, int milliseconds);

/**
 * Set max reconnect time.
 * @param socket
 * @param milliseconds
 * @return
 */
int SetSocketReConnectMaxTime(const nng_socket* socket, int milliseconds);

extern "C" {
	/**
	 * connect url
	 * @param url const char*
	 * @return Pointer Address. nng_socket*
	 */
	NNG_API Nng* ConnectRequestSocket(const char* url);


	/**
	/**
	 * Close Connect
	 * @param nng nng_socket*
	 */
	NNG_API void Release(const Nng* nng);

	/**
	 * Get response with the request.
	 * @param nng
	 * @param request
	 * @param requestSize
	 * @return intptr_t, which can cast to unsigned char*.
	 */
	NNG_API intptr_t GetResponseWithRequest(Nng* nng, const unsigned char* request, int requestSize);

	/**
	 * We will get the result state when we call connect, bind, receive, send, etc...
	 * The type of this state is integer. And we can call this function to see what does the state mean.
	 * @param rv
	 * @return const char*
	 */
	NNG_API const char* GetErrorString(int rv);

	NNG_API void FreeMessage(const char* msg);

	NNG_API void SetSendMaxTimeOut(Nng* nng, int millisecond);
	void SetReceiveMaxTimeOut(Nng* nng, int millisecond);
}


#endif //NNG_PACKAGE_DLL_LIBRARY_H
