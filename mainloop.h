#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>


typedef void (*mainloop_event_func)(int fd, uint32_t events, void *param_ptr);
typedef void (*mainloop_timeout_func)(int id, void *param_ptr);
typedef void (*mainloop_signal_func)(int signum, void *param_ptr);
typedef void (*mainloop_destroy_func)(void *param_ptr);

typedef struct _mainloop_event_data
{
	int fd;
	uint32_t events;
	mainloop_event_func ev_func;
	mainloop_destroy_func ds_func;
	void *param_ptr;
} mainloop_event_data_t;

typedef struct _mainloop_timer_data
{
	int id;
	uint32_t timeout_ms;
	mainloop_timeout_func tm_func;
	mainloop_destroy_func ds_func;
	void *param_ptr;
} mainloop_timer_data_t;

typedef struct _mainloop_signal_data
{
	int signum;
	sigset_t mask;
	mainloop_signal_func sg_func;
	mainloop_destroy_func ds_func;
	void *param_ptr;
} mainloop_signal_data_t;


class CMainLoop
{
public:
	CMainLoop();
	virtual ~CMainLoop();

	bool create();
	void destroy();
	bool run();

	bool add_event(int fd, uint32_t events, mainloop_event_func ev_func, void *param_ptr, mainloop_destroy_func ds_func);
	bool modify_event(int fd, uint32_t events);
	bool remove_event(int fd);

	bool add_timer(int id, uint32_t timeout_ms, mainloop_timer_func tm_func, void *param_ptr, mainloop_destroy_func ds_func);
	bool modify_timer(int id, uint32_t timeout_ms);
	bool remove_timer(int id);

	bool add_signal(sigset_t *mask, mainloop_signal_func sg_func, void *param_ptr, mainloop_destroy_func ds_func);
};
