#ifndef M_MYSQL_WRAPPER_MUTEX_INCLUDE
#define M_MYSQL_WRAPPER_MUTEX_INCLUDE

#include "mysql_wrapper_config.h"

#ifdef M_PLATFORM_WIN
#include <Windows.h>
#else
#include <pthread.h>
#endif

namespace MysqlWrapperDetail {

#ifdef M_PLATFORM_WIN
	class MutexLock
	{
	public:
		MutexLock()
		{
			InitializeCriticalSection(&_cs);
		}
		~MutexLock()
		{
			DeleteCriticalSection(&_cs);
		}
		void lock()
		{
			EnterCriticalSection(&_cs);
		}
		void unlock()
		{
			LeaveCriticalSection(&_cs);
		}
	private:
		MutexLock(const MutexLock&);
		MutexLock& operator=(const MutexLock&);

		CRITICAL_SECTION _cs;
	};
#endif

#ifndef M_PLATFORM_WIN
	class MutexLock
	{
	public:
		MutexLock()
		{
			pthread_mutex_init(&_mutex, 0);
		}
		~MutexLock()
		{
			pthread_mutex_destroy(&_mutex);
		}
		void lock()
		{
			pthread_mutex_lock(&_mutex);
		}
		void unlock()
		{
			pthread_mutex_unlock(&_mutex);
		}

	private:
		MutexLock(const MutexLock&);
		MutexLock& operator=(const MutexLock&);

		pthread_mutex_t _mutex;
	};
#endif

	class ScopedLock
	{
	public:
		ScopedLock(MutexLock& mutex) :_mutex(mutex)
		{
			_mutex.lock();
		}
		~ScopedLock()
		{
			_mutex.unlock();
		}
	private:
		ScopedLock(const ScopedLock&);
		ScopedLock& operator=(const ScopedLock&);

		MutexLock& _mutex;
	};

}
#endif