#pragma once
#include <stdint.h>
#include <vector>
#ifdef WIN32
#include <Windows.h>


/**
* @breif:
* Event for Windows
*/
class CEvent
{
public:
	CEvent();
	virtual ~CEvent();

	static int wait_for_single_object(CEvent *evt_ptr, int timeout_ms = -1);
	static int wait_for_multi_objects(CEvent **evts_ptr, int count, int timeout_ms = -1);

	bool create(const char *name_ptr = NULL, bool manual_reset = false, bool init_set = false);
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


/**
* @breif:
* Event for Linux
*/
class CEvent
{
public:
	CEvent();
	virtual ~CEvent();

	static int wait_for_single_object(CEvent *evt_ptr, int timeout_ms = -1);
	static int wait_for_multi_objects(CEvent **evts_ptr, int count, int timeout_ms = -1);

	bool create(const char *name_ptr = NULL, bool manual_reset = false, bool init_set = false);
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

