#pragma once
#include <stdint.h>
#include <list>
#include "locker.h"


class ICPacketSink;
class CPacket
{
public:
	typedef enum _packet_type
	{
		PACKET_UNKNOWN = 0,
		PACKET_VIDEO_RAW,
		PACKET_VIDEO_H264,
		PACKET_VIDEO_HEVC,
		PACKET_AUDIO_RAW,
		PACKET_AUDIO_AAC
	} packet_type_t;

public:
	CPacket()
	{
		_id = 0;
		_packet_type = PACKET_UNKNOWN;
		_sink_ptr = NULL;

		_buffer_size = 0;
		_valid_size = 0;
		_buffer_ptr = NULL;

		_extra_buffer_size = 0;
		_extra_buffer_ptr = NULL;
		_flag = 0;
	}
	virtual ~CPacket() {}

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
		return (VR_PACKET_VIDEO_RAW == _packet_type
			|| VR_PACKET_VIDEO_H264 == _packet_type
			|| VR_PACKET_VIDEO_HEVC == _packet_type);
	}

	bool create(uint32_t id, uint32_t size, ICPacketSink *sink_ptr)
	{
		if (NULL != _buffer_ptr)
			return false;

		_buffer_ptr = new uint8_t[size];
		if (NULL == _buffer_ptr)
			return false;

		_id = id;
		_sink_ptr = sink_ptr;
		_buffer_size = size;
		_valid_size = 0;
		return true;
	}
	void destroy()
	{
		clear_extra_buffer();
		if (NULL != _buffer_ptr) {
			delete[] _buffer_ptr;
			_buffer_ptr = NULL;
			_buffer_size = 0;
			_valid_size = 0;
		}
	}
	uint8_t* get_buffer_ptr()
	{
		return _buffer_ptr;
	}
	void set_valid_size(uint32_t valid_size)
	{
		_valid_size = valid_size;
	}
	uint32_t get_valid_size()
	{
		return _valid_size;
	}

	bool set_extra_buffer(uint32_t size, uint8_t *data_ptr)
	{
		if (NULL == data_ptr || NULL != _extra_buffer_ptr)
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
			_extra_buffer_size = 0;
		}
	}
	void set_extra_flag(uint64_t flag)
	{
		_flag = flag;
	}
	uint64_t get_extra_flag()
	{
		return _flag;
	}

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
	uint32_t _id;
	packet_type_t _packet_type;
	ICPacketSink *_sink_ptr;

	uint32_t _buffer_size;
	uint32_t _valid_size;
	uint8_t *_buffer_ptr;

	uint32_t _extra_buffer_size;
	uint8_t *_extra_buffer_ptr;
	uint64_t _flag;

	CLocker _locker;
	uint32_t _ref;
};
typedef std::list<CPacket *> CPacketList;


class ICPacketSource
{
public:
	virtual CPacket* pull_media_packet() = 0;
};
typedef std::list<ICPacketSource *> ICPacketSourceList;


class ICPacketSink
{
public:
	virtual void push_media_packet(CPacket *packet_ptr) = 0;
};
typedef std::list<ICPacketSink *> ICPacketSinkList;


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
		if (iter == _packet_sinks.end())
			_packet_sinks.push_back(sink_ptr);
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

	//ICPacketSink
	virtual void push_media_packet(CPacket *packet_ptr)
	{
		CAutoLocker auto_locker(_locker);

		ICPacketSinkList::iterator iter;
		for (iter = _packet_sinks.begin(); iter != _packet_sinks.end(); iter++) {
			(*iter)->push_media_packet(packet_ptr);
		}
	}

private:
	CLocker _locker;
	ICPacketSinkList _packet_sinks;
};


class CPacketPool : public ICPacketSource, public ICPacketSink
{
public:
	CPacketPool()
	{

	}
	virtual ~CPacketPool() {}

	bool create(uint32_t count, uint32_t size)
	{
		bool error = false;
		for (uint32_t i = 0;i < count;i++) {
			CPacket *packet_ptr = new CPacket;
			if (NULL == packet_ptr || !packet_ptr->create(i, size, this)) {
				error = true;
				break;
			}
			_unused_packets.push_back(packet_ptr);
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
            CPacket *packet_ptr = (*iter);
            packet_ptr->destroy();
            delete packet_ptr;
        }
        _unused_packets.clear();
	}
	// ICPacketSource
	virtual CPacket* pull_media_packet()
	{
		CAutoLocker auto_locker(_locker);
		if (0 == _unused_packets.size())
			return NULL;

        CPacket *packet_ptr = _unused_packets.back();
		_unused_packets.pop_back();
		packet_ptr->add_ref();
		return packet_ptr;
	}
	// ICPacketSink
	virtual void push_media_packet(CPacket *packet_ptr)
	{
		CAutoLocker auto_locker(_locker);
		_unused_packets.push_back(packet_ptr);
		return true;
	}

protected:
	CLocker _locker;
	CPacketList _unused_packets;
};
