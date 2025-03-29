#ifndef NNG_PACKAGE_DLL_LIBRARY_H
#define NNG_PACKAGE_DLL_LIBRARY_H

#include <string>
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

extern "C" NNG_API nng_socket* ConnectUrl(const std::string& url);

#endif //NNG_PACKAGE_DLL_LIBRARY_H
