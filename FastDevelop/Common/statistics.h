#pragma once
#include <stdint.h>
#include <atomic>
#include <Windows.h>


/**
* @breif:
* Frame statistics
*
* For frame in and out count
*/
class CFrameStatistics
{
public:
	CFrameStatistics()
	{
		_frame_in_count = 0;
		_frame_out_count = 0;
	}
	virtual ~CFrameStatistics() {}

	inline void reset()
	{
		_frame_in_count = 0;
		_frame_out_count = 0;
	}
	inline void set_frame_in()
	{
		_frame_in_count++;
	}
	inline void set_frame_out()
	{
		_frame_out_count++;
	}
	inline uint64_t get_frame_in()
	{
		return _frame_in_count;
	}
	inline uint64_t get_frame_out()
	{
		return _frame_out_count;
	}
	inline uint64_t get_frame_diff()
	{
		return (_frame_in_count - _frame_out_count);
	}

protected:
	std::atomic<uint64_t> _frame_in_count;
	std::atomic<uint64_t> _frame_out_count;
};


/**
* @breif:
* For time measurement
*/
class CTimeStatistics
{
public:
	CTimeStatistics()
	{
		_freq_ms = 0.0;
		_start_time.QuadPart = 0;
		_stop_time.QuadPart = 0;

		LARGE_INTEGER temp;
		QueryPerformanceFrequency((LARGE_INTEGER*)&temp);
		_freq_ms = ((double)temp.QuadPart) / 1000.0;
	}
	virtual ~CTimeStatistics() {}

	inline void reset()
	{
		_start_time.QuadPart = 0;
		_stop_time.QuadPart = 0;
	}
	inline void start()
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&_start_time);
	}
	inline void stop()
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&_stop_time);
	}
	inline float get_time_ms()
	{
		return (float)(((double)_stop_time.QuadPart - (double)_start_time.QuadPart) / _freq_ms);
	}
	inline float get_delta_ms()
	{
		if (0 != _start_time.QuadPart) {
			LARGE_INTEGER temp;
			QueryPerformanceCounter((LARGE_INTEGER*)&temp);
			return (float)(((double)temp.QuadPart - (double)_start_time.QuadPart) / _freq_ms);
		}
		else {
			return 0.f;
		}
	}

protected:
	double _freq_ms;
	LARGE_INTEGER _start_time;
	LARGE_INTEGER _stop_time;
};


/**
* @breif:
* For avg frame and time measurements
*/
class CAvgStatistics
{
public:
	CAvgStatistics()
	{
		_frame_statistics.reset();
		_time_statistics.reset();
		_pkt_missing = 0;
		_started = false;
	}
	virtual ~CAvgStatistics() {}

	inline void reset()
	{
		_frame_statistics.reset();
		_time_statistics.reset();
		_pkt_missing = 0;
		_started = false;
	}
	inline void start()
	{
		reset();
		_time_statistics.start();
		_started = true;
	}
	inline void stop()
	{
		_time_statistics.stop();
		_started = false;
	}
	inline void tick_in()
	{
		if (_started) {
			_frame_statistics.set_frame_in();
		}
	}
	inline void tick_out()
	{
		if (_started) {
			_frame_statistics.set_frame_out();
		}
	}
	inline float get_avg_in_fps()
	{
		if (_started) {
			return (0 == _time_statistics.get_delta_ms() ? 0.0f :
				1000.0f * ((float)_frame_statistics.get_frame_in() / _time_statistics.get_delta_ms()));
		}
		else {
			return (0 == _time_statistics.get_time_ms() ? 0.0f :
				1000.0f * ((float)_frame_statistics.get_frame_in() / _time_statistics.get_time_ms()));
		}
	}
	inline float get_avg_out_fps()
	{
		if (_started) {
			return (0 == _time_statistics.get_delta_ms() ? 0.0f :
				1000.0f * ((float)_frame_statistics.get_frame_out() / _time_statistics.get_delta_ms()));
		}
		else {
			return (0 == _time_statistics.get_time_ms() ? 0.0f :
				1000.0f * ((float)_frame_statistics.get_frame_out() / _time_statistics.get_time_ms()));
		}
	}
	inline float get_avg_in_time_ms()
	{
		if (_started) {
			return (0 == _frame_statistics.get_frame_in() ? 0.0f :
				_time_statistics.get_delta_ms() / (float)_frame_statistics.get_frame_in());
		}
		else {
			return (0 == _frame_statistics.get_frame_in() ? 0.0f :
				_time_statistics.get_time_ms() / (float)_frame_statistics.get_frame_in());
		}
	}
	inline float get_avg_out_time_ms()
	{
		if (_started) {
			return (0 == _frame_statistics.get_frame_out() ? 0.0f :
				_time_statistics.get_delta_ms() / _frame_statistics.get_frame_out());
		}
		else {
			return (0 == _frame_statistics.get_frame_out() ? 0.0f :
				_time_statistics.get_time_ms() / (float)_frame_statistics.get_frame_out());
		}
	}

public:
	CFrameStatistics _frame_statistics;
	CTimeStatistics _time_statistics;
	std::atomic<uint32_t> _pkt_missing;

protected:
	bool _started;
};

