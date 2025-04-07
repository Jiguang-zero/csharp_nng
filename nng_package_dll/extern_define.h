#ifndef EXTERN_DEFINE_H
#define EXTERN_DEFINE_H

#ifdef _WIN32
	#ifdef _NNG_EXPORTS
		#define NNG_API __declspec(dllexport)
	#else
		#define NNG_API __declspec(dllimport)
	#endif
#else
	#define NNG_API
#endif

#endif
