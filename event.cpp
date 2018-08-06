#include "event.h"

#ifdef WIN32
CEvent::CEvent()
{
	_event = NULL;
}

CEvent::~CEvent()
{
}

int CEvent::wait_for_single_object(CEvent *event_ptr, int timeout_ms)
{
	return WaitForSingleObject(event_ptr->get_handle(), 
		timeout_ms == -1 ? INFINITE : timeout_ms);
}

int CEvent::wait_for_multi_objects(CEvent **events_ptr, int count, int timeout_ms)
{
	if (count >= MAX_EVENTS_NUM)
		return -1;
	
	HANDLE events[MAX_EVENTS_NUM];
	for (int i = 0;i < count;i++)
		events[i] = events_ptr[i]->get_handle();
	return WaitForMultipleObjects(events, count, FALSE, timeout_ms == -1 ? INFINITE : timeout_ms);
}

bool CEvent::create(bool init_set = false)
{
	_event = CreateEvent(NULL, FALSE, init_set ? TRUE : FALSE, NULL);
	if (NULL == _event)
		return false;
	
	return true;
}

void CEvent::destroy()
{
	if (NULL != _event) {
		CloseHandle(_event);
		_event = NULL;
	}
}

bool CEvent::is_valid()
{
	return (NULL != _event);
}

HANDLE CEvent::get_handle()
{
	return _event;
}

bool CEvent::set()
{
	if (!is_valid())
		return false;

	SetEvent(_event);
	return true;
}

bool CEvent::reset()
{
	if (!is_valid())
		return false;
	
	ResetEvent(_event);
	return true;
}


#else


CEvent::CEvent()
{
	_fd = -1;
}

CEvent::~CEvent()
{
}

// error: -1, timeout: 0, value: others
int CEvent::wait_for_single_object(CEvent *event_ptr, int timeout_ms = -1)
{
	int fd = event_ptr->get_fd();
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);
	
	struct timeval tv;
	struct timeval *ptv = NULL;
	if (timeout_ms < 0) {
		ptv = NULL;
	} 
	else {
		tv.tv_sec = timeout_ms / 1000;
		tv.tv_usec = (timeout_ms % 1000) * 1000;
		ptv = &tv;
	}
	int ret = select(fd + 1, &rfds, NULL, NULL, ptv);
	return ret;
}

int CEvent::wait_for_multi_objects(CEvent **events_ptr, int count, int timeout_ms = -1)
{
	fd_set rfds;
	FD_ZERO(&rfds);
	int maxfd = 0;
	for (int index = 0; index < count; index++) {
		int fd = events_ptr[index]->get_fd();
		FD_SET(fd, &rfds);
		if (fd > maxfd)
			maxfd = fd;
	}
	
	struct timeval tv;
	struct timeval *ptv = NULL;
	if (timeout_ms < 0) {
		ptv = NULL;
	} 
	else {
		tv.tv_sec = timeout_ms / 1000;
		tv.tv_usec = (timeout_ms % 1000) * 1000;
		ptv = &tv;
	}
	int ret = select(maxfd + 1, &rfds, NULL, NULL, ptv);
	return ret;
}

bool CEvent::create(bool init_signal)
{
	if (-1 != _fd) {
		destroy();
	}
		
	_fd = eventfd(init_signal ? 1 : 0, EFD_NONBLOCK);
	if (-1 == _fd) {
		return false;
	}

	return true;
}

void CEvent::destroy()
{
	if (-1 != _fd) {
		close(_fd);
		_fd = -1;			
	}
}

bool CEvent::is_valid()
{
	return (-1 != _fd);
}

int CEvent::get_fd()
{
	return _fd;
}

bool CEvent::set()
{
	if (-1 == _fd) {
		return false;
	}
	uint64_t u = 1;
	int ret = write(_fd, &u, sizeof(uint64_t));
	return (ret == sizeof(uint64_t));
}

bool CEvent::reset()
{
	uint64_t u = 0;
	bool success = false;
	do {
		if (-1 == _fd)
			break;

		int ret = write(_fd, &u, sizeof(uint64_t));
		if (ret != sizeof(uint64_t))
			break;
		ret = read(_fd, &u, sizeof(uint64_t));
		if (ret != sizeof(uint64_t) && EAGAIN != errno))
			break;

		success = true;
	} while(false);
	
	return success;
}

bool CEvent::is_signaled()
{
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(_fd, &rfds);
	
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	int ret = select(_fd + 1, &rfds, NULL, NULL, &tv);
	if (ret > 0 && FD_ISSET(_fd, &rfds))
		return true;
		
	return false;
}

#endif