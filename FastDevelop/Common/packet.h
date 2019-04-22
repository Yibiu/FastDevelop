#pragma once
#include <stdint.h>
#include <list>
#include <thread>
#include "locker.h"
#include "event.h"


class CPacket;


/**
* @breif:
* Packet source for pull data from upper module
*/
class ICPacketSource
{
public:
	virtual CPacket* pull_media_packet() = 0;
};
typedef std::list<ICPacketSource *> ICPacketSourceList;


/**
* @breif:
* Packet sink for push data to lower module
*/
class ICPacketSink
{
public:
	virtual bool push_media_packet(CPacket *pkt_ptr) = 0;
};
typedef std::list<ICPacketSink *> ICPacketSinkList;


/**
* @breif:
* Packet source and sink for pull and push
*/
class ICPacketSourceSink : public ICPacketSource, public ICPacketSink
{
public:
};
typedef std::list<ICPacketSourceSink *> ICPacketSourceSinkList;


/**
* @breif:
* Packet for transfer from one module to another
*/
class CPacket
{
public:
	typedef enum _packet_type
	{
		PACKET_UNKNOWN = 0x00,
		PACKET_VIDEO_RAW,
		PACKET_VIDEO_H264,
		PACKET_VIDEO_HEVC,
		PACKET_AUDIO_RAW,
		PACKET_AUDIO_AAC
	} packet_type_t;

public:
	CPacket()
	{
		_packet_type = PACKET_UNKNOWN;

		_sink_ptr = NULL;
		_flags = 0;
		_frame_index = 0;
		_pts = 0;
		_dts = 0;
		_src_ptr = NULL;
		_dst_ptr = NULL;

		_id = 0;
		_buffer_ptr = NULL;
		_buffer_size = 0;
		_valid_size = 0;

		_extra_buffer_ptr = NULL;
		_extra_buffer_size = 0;

		_ref = 0;
	}
	virtual ~CPacket() {}

	// Packet type
	void set_type(packet_type_t type)
	{
		_packet_type = type;
	}
	packet_type_t get_type()
	{
		return _packet_type;
	}
	bool is_video()
	{
		return (PACKET_VIDEO_RAW == _packet_type
			|| PACKET_VIDEO_H264 == _packet_type
			|| PACKET_VIDEO_HEVC == _packet_type);
	}

	// Params get/set
	ICPacketSink* get_sink()
	{
		return _sink_ptr;
	}
	uint64_t get_flags()
	{
		return _flags;
	}
	uint64_t get_frame_index()
	{
		return _frame_index;
	}
	uint64_t get_pts()
	{
		return _pts;
	}
	uint64_t get_dts()
	{
		return _dts;
	}
	void* get_src_ptr()
	{
		return _src_ptr;
	}
	void* get_dst_ptr()
	{
		return _dst_ptr;
	}
	void set_sink(ICPacketSink *sink_ptr)
	{
		_sink_ptr = sink_ptr;
	}
	void set_flags(uint64_t flags)
	{
		_flags = flags;
	}
	void set_frame_index(uint64_t frame_index)
	{
		_frame_index = frame_index;
	}
	void set_pts(uint64_t pts)
	{
		_pts = pts;
	}
	void set_dts(uint64_t dts)
	{
		_dts = dts;
	}
	void set_src_ptr(void *src_ptr)
	{
		_src_ptr = src_ptr;
	}
	void set_dst_ptr(void *dst_ptr)
	{
		_dst_ptr = dst_ptr;
	}

	// Memory
	bool create(uint32_t id, uint32_t size)
	{
		_buffer_ptr = new uint8_t[size];
		if (NULL == _buffer_ptr)
			return false;

		_id = id;
		_buffer_size = size;
		_valid_size = 0;
		return true;
	}
	void destroy()
	{
		if (NULL != _buffer_ptr) {
			delete[] _buffer_ptr;
			_buffer_ptr = NULL;
		}
		_buffer_size = 0;
		_valid_size = 0;
	}
	uint8_t* get_buffer_ptr()
	{
		return _buffer_ptr;
	}
	void set_valid_size(uint32_t size)
	{
		_valid_size = size;
	}
	uint32_t get_valid_size()
	{
		return _valid_size;
	}

	// Extra memory
	bool set_extra_buffer(uint32_t size, uint8_t *data_ptr)
	{
		if (0 != size && NULL == data_ptr)
			return false;
		if (NULL != _extra_buffer_ptr)
			return false;

		_extra_buffer_ptr = new uint8_t[size];
		if (NULL == _extra_buffer_ptr)
			return false;

		memcpy(_extra_buffer_ptr, data_ptr, size);
		_extra_buffer_size = size;
		return true;
	}
	const uint8_t* get_extra_buffer_ptr()
	{
		return _extra_buffer_ptr;
	}
	uint32_t get_extra_buffer_size()
	{
		return _extra_buffer_size;
	}
	void clear_extra_buffer()
	{
		if (NULL != _extra_buffer_ptr) {
			delete[] _extra_buffer_ptr;
			_extra_buffer_ptr = NULL;
		}
		_extra_buffer_size = 0;
	}

	// Ref/Release count
	void add_ref()
	{
		CAutoLocker auto_locker(_locker);
		_ref++;
	}
	void release()
	{
		CAutoLocker auto_locker(_locker);
		if (0 == _ref)
			return;
		
		_ref--;
		if (0 == _ref) {
			if (NULL != _sink_ptr)
				_sink_ptr->push_media_packet(this);
		}
	}

protected:
	packet_type_t _packet_type;

	ICPacketSink *_sink_ptr;
	uint64_t _flags;
	uint64_t _frame_index;
	uint64_t _pts;
	uint64_t _dts;
	void *_src_ptr;
	void *_dst_ptr;

	uint32_t _id;
	uint8_t *_buffer_ptr;
	uint32_t _buffer_size;
	uint32_t _valid_size;

	uint8_t *_extra_buffer_ptr;
	uint32_t _extra_buffer_size;

	CLocker _locker;
	uint32_t _ref;
};
typedef std::list<CPacket *> CPacketList;


/**
* @breif:
* Packet connector for multi-sinks pushing
*
*           Connector
*        ---------------
*        |    Sink0    |
*        ---------------
* ---->  |    Sink1    |  ---->
*        ---------------
*        |    ....     |
*        ---------------
*
*/
class CPacketConnector :public ICPacketSink
{
public:
	CPacketConnector() 
	{
		_packet_sinks.clear();
	}
	virtual ~CPacketConnector() {}

	void add_sink(ICPacketSink *sink_ptr)
	{
		CAutoLocker auto_locker(_locker);

		ICPacketSinkList::iterator iter;
		for (iter = _packet_sinks.begin(); iter != _packet_sinks.end(); iter++) {
			if ((*iter) == sink_ptr)
				break;
		}
		if (iter == _packet_sinks.end()) {
			_packet_sinks.push_back(sink_ptr);
		}
	}
	void delete_sink(ICPacketSink *sink_ptr)
	{
		CAutoLocker auto_locker(_locker);

		ICPacketSinkList::iterator iter;
		for (iter = _packet_sinks.begin(); iter != _packet_sinks.end(); iter++) {
			if ((*iter) == sink_ptr) {
				_packet_sinks.erase(iter);
				break;
			}
		}
	}
	void clear()
	{
		CAutoLocker auto_locker(_locker);
		_packet_sinks.clear();
	}

	// ICPacketSink
	virtual bool push_media_packet(CPacket *pkt_ptr)
	{
		CAutoLocker auto_locker(_locker);

		ICPacketSinkList::iterator iter;
		for (iter = _packet_sinks.begin(); iter != _packet_sinks.end(); iter++) {
			(*iter)->push_media_packet(pkt_ptr);
		}
		return true;
	}

private:
	CLocker _locker;
	ICPacketSinkList _packet_sinks;
};


/**
* Packet pool for packet reusing
*
*         ----- In Use-----
*         |               |
*         |               \/
*       ----------------------
* front |    |    | ... |    |  back
*       ----------------------
*
*/
class CPacketPool : public ICPacketSourceSink
{
public:
	CPacketPool()
	{
		_unused_packets.clear();
	}
	virtual ~CPacketPool() {}

	bool create(uint32_t count, uint32_t size)
	{
		bool error = false;
		for (uint32_t i = 0;i < count;i++) {
			CPacket *pkt_ptr = new CPacket;
			if (NULL == pkt_ptr || !pkt_ptr->create(i, size)) {
				error = true;
				break;
			}
			pkt_ptr->set_sink(this);
			_unused_packets.push_back(pkt_ptr);
		}

		if (error) {
			destroy();
		}
		return (!error);
	}
	void destroy()
	{
		CAutoLocker auto_lock(_locker);
        CPacketList::iterator iter;
        for (iter = _unused_packets.begin(); iter != _unused_packets.end(); iter++) {
            CPacket *pkt_ptr = (*iter);
			pkt_ptr->destroy();
            delete pkt_ptr;
        }
        _unused_packets.clear();
	}

	// ICPacketSourceSink
	virtual CPacket* pull_media_packet()
	{
		CAutoLocker auto_locker(_locker);
		if (0 == _unused_packets.size())
			return NULL;

        CPacket *pkt_ptr = _unused_packets.front();
		_unused_packets.pop_front();
		pkt_ptr->add_ref();
		return pkt_ptr;
	}
	virtual bool push_media_packet(CPacket *pkt_ptr)
	{
		CAutoLocker auto_locker(_locker);
		_unused_packets.push_back(pkt_ptr);
		return true;
	}

protected:
	CLocker _locker;
	CPacketList _unused_packets;
};


/**
* @brief:
* Packet thread for packet dispatching
*
* Thread + Connector
*
* NOTE: the packet to this module is commonly from packet pool,
*       so call release instead of destroy of packet.
*/
class CPacketThread : public CPacketConnector
{
public:
	CPacketThread()
	{
		_running = false;
		_thread_ptr = NULL;

    	_packet_list.clear();
	}
	virtual ~CPacketThread() {}

	bool is_running()
	{
		return _running;
	}
	uint32_t packet_count()
	{
		CAutoLocker auto_locker(_locker);
		return _packet_list.size();
	}
	bool create()
	{
		bool success = false;

		do {
			if (!_event.create(NULL, false, false))
				break;

			_running = true;
			_thread_ptr = new std::thread(thread_proc, this);
			if (NULL == _thread_ptr)
				break;

			success = true;
		} while(false);
		
		if (!success) {
			destroy();
		}
		return success;
	}
	void destroy()
	{
		_running = false;
		if (_event.is_valid()) {
			_event.set();
			if (NULL != _thread_ptr) {
				_thread_ptr->join();
				delete _thread_ptr;
				_thread_ptr = NULL;
			}
			_event.destroy();
		}
		if (NULL != _thread_ptr) {
			_thread_ptr->join();
			delete _thread_ptr;
			_thread_ptr = NULL;
		}

		{
			CAutoLocker auto_locker(_locker);
			CPacketList::iterator iter;
			for (iter = _packet_list.begin(); iter != _packet_list.end(); iter++) {
				CPacket *pkt_ptr = (*iter);
				pkt_ptr->release();
			}		
			_packet_list.clear();
		}
	}

	static void thread_proc(void *param_ptr)
	{
		CPacketThread *this_ptr = (CPacketThread *)param_ptr;
		if (NULL != this_ptr)
			this_ptr->thread_proc_internal();
	}
	void thread_proc_internal()
	{
		while (_running) 
		{
			int ret = CEvent::wait_for_single_object(&_event);
			if (-1 == ret)
				break;

			while (_running) 
			{
				CPacket *pkt_ptr = NULL;
				{
					CAutoLocker locker(_locker);
					if (0 == _packet_list.size())
						break;
					pkt_ptr = _packet_list.front();
					_packet_list.pop_front();
				}

				CPacketConnector::push_media_packet(pkt_ptr);
				
				pkt_ptr->release();
			}
		}
	}

	// Re-write for CPacketConnector
	virtual bool push_media_packet(CPacket *pkt_ptr)
	{
		if (!_running)
			return false;

		{
			CAutoLocker locker(_locker);
			pkt_ptr->add_ref();
			_packet_list.push_back(pkt_ptr);
			_event.set();
		}
		return true;
	}

protected:
	bool _running;
	std::thread *_thread_ptr;
	CEvent _event;

	CLocker _locker;
    CPacketList	_packet_list;
};

