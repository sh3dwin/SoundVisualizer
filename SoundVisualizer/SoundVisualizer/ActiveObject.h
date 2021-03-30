#pragma once
#include <iostream>
#include <Windows.h>


class ActiveObject {
public:
	ActiveObject();
	virtual ~ActiveObject() {}
	void Kill();
protected:
	virtual void InitThread() = 0;
	virtual void Run() = 0;
	virtual void FlushThread() = 0;

	static DWORD WINAPI ThreadEntry(void* pArg);

	int _isDying;
	Thread _thread;
};

class Thread {
public:
	Thread(DWORD(WINAPI* pFun) (void* arg), void* pArg) {
		_handle = CreateThread(
			0, //Security attributes
			0, //Stack size
			pFun,
			pArg,
			CREATE_SUSPENDED,
			&_tid);
	}
	~Thread() {
		CloseHandle(_handle);
	}
	void Resume() {
		ResumeThread(_handle);
	}
	void WaitForDeath() {
		WaitForSingleObject(_handle, 2000);
	}

private:
	HANDLE _handle;
	DWORD _tid; // thread id
};

class Mutex {
	friend class Lock;
public:
	Mutex() {
		InitializeCriticalSection(&_critSection);
	}
	~Mutex() {
		DeleteCriticalSection(&_critSection);
	}
private:
	void Acquire() {
		EnterCriticalSection(&_critSection);
	}
	void Release() {
		LeaveCriticalSection(&_critSection);
	}

	CRITICAL_SECTION _critSection;
};

class Lock {
public:
	Lock(Mutex& mutex): _mutex(mutex) {
		_mutex.Acquire();
	}
	~Lock() {
		_mutex.Release();
	}
private:
	Mutex &_mutex;
};

class Event {
public:
	Event() {
		// start in non-signaled state (red light)
		// auto reset after every Wait
		_handle = CreateEvent(0, FALSE, FALSE, 0);
	}
	~Event() {
		CloseHandle(_handle);
	}
	void Release() {
		SetEvent(_handle);
	}
	void Wait() {
		WaitForSingleObject(_handle, INFINITE);
	}
	operator HANDLE() {
		return _handle;
	}
	
private:
	HANDLE _handle;
};