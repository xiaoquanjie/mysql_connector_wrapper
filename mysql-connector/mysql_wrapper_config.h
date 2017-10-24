#ifndef M_MYSQL_WRAPPER_CONFIG_INCLUDE
#define M_MYSQL_WRAPPER_CONFIG_INCLUDE

#ifndef M_PLATFORM_WIN32
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define M_PLATFORM_WIN32 1
#endif
#endif

#ifndef M_PLATFORM_WIN
#if defined(M_PLATFORM_WIN32) || defined(WIN64)
#define M_PLATFORM_WIN 1
#endif
#endif

#endif