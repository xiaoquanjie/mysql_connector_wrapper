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

#ifdef M_PLATFORM_WIN
#pragma warning(disable:4251)
#endif

#ifndef M_PLATFORM_WIN
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <pthread.h>
#ifndef gettid
#define gettid() syscall(SYS_gettid)
#endif

template<int N=0>
struct CThreadId {
	static int ctid()
	{
		if (__builtin_expect(_ctid == 0, 0))
		{
			_ctid = gettid();
		}
		return _ctid;
	}
private:
	static __thread int _ctid;
};

template<int N>
__thread int CThreadId<N>::_ctid;
#endif

#ifdef M_PLATFORM_WIN
#define get_cur_threadid()	GetCurrentThreadId()
#else
#define get_cur_threadid()  CThreadId<>::ctid()
#endif

#endif