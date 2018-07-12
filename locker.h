#pragma once
#ifdef WIN32
#include <Windows.h>


class CLocker
{
public:
	CLocker()
	{
		InitializeCriticalSection(&_cs);
	}
	virtual ~CLocker()
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
	int try_lock()
	{
		return (TryEnterCriticalSection(&_cs) ? 0 : -1);
	}

protected:
	CRITICAL_SECTION _cs;
};

#else
extern "C"
{
#include <pthread.h>
}


class CLocker
{
public:
	CLocker()
	{
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, 1);
		pthread_mutex_init(&_mutex, &attr);
	}
	virtual ~CLocker()
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
	int try_lock()
	{
		return pthread_mutex_trylock(&_mutex);;
	}

protected:
	pthread_mutex_t _mutex;
};


class CRWLokcer
{
public:
	CRWLokcer()
	{
		pthread_rwlock_init(&_rwlocker, NULL);
	}
	virtual ~CRWLokcer() 
	{
		pthread_rwlock_destroy(&_rwlocker);
	}

	bool read_lock()
	{
		return (0 == pthread_rwlock_rdlock(&_rwlocker));
	}
	bool write_lock()
	{
		return (0 == pthread_rwlock_wrlock(&_rwlocker));
	}
	bool try_read_lock()
	{
		return (0 == pthread_rwlock_tryrdlock(&_rwlocker));
	}
	bool try_write_lock()
	{
		return (0 == pthread_rwlock_trywrlock(&_rwlocker));
	}
	bool unlock()
	{
		return (0 == pthread_rwlock_unlock(&_rwlocker));
	}

protected:
	pthread_rwlock_t _rwlocker;
};

#endif


class CAutoLocker
{
public:
	CAutoLocker(CLocker &locker) : _locker(locker)
	{
		_locker.lock();
	}
	virtual ~CAutoLocker()
	{
		_locker.unlock();
	}

protected:
	CLocker _locker;
};



class CAutoUnlocker
{
public:
	CAutoUnlocker(CLocker &locker) : _locker(locker)
	{
		_locker.unlock();
	}
	virtual ~CAutoUnlocker()
	{
		_locker.lock();
	}

protected:
	CLocker &_locker;
};
