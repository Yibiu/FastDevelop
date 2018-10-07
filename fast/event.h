#pragma once
#include <stdint.h>
#ifdef WIN32
#include <Windows.h>


#define MAX_EVENTS_NUM	16

class CEvent
{
public:
	CEvent();
	virtual ~CEvent();

	static int wait_for_single_object(CEvent *event_ptr, int timeout_ms = -1);
	static int wait_for_multi_objects(CEvent **events_ptr, int count, int timeout_ms = -1);

	bool create(bool init_set = false);
	void destroy();
	bool is_valid();
	HANDLE get_handle();
	bool set();
	bool reset();

protected:
	HANDLE _event;
};


#else
#include <unistd.h>
#include <errno.h>
#include <sys/eventfd.h>
#include <sys/select.h>


class CEvent
{
public:
	CEvent();
	virtual ~CEvent();

	static int wait_for_single_object(CEvent *event_ptr, int timeout_ms = -1);
	static int wait_for_multi_objects(CEvent **events_ptr, int count, int timeout_ms = -1);

	bool create(bool init_set = false);
	void destroy();
	bool is_valid();
	int get_fd();
	bool set();
	bool reset();
	bool is_signaled();

protected:
	int _fd;
};

#endif
