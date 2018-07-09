#include "io.h"


// ICBaseIO
bool ICBaseIO::readu8(uint8_t &value)
{
	return read(1, &value);
}

bool ICBaseIO::readu16(uint16_t &value)
{
	uint8_t data[2];

	bool ret = read(2, data);
	if (ret) {
		value = data[0];
		value <<= 8;
		value |= data[1];
	}

	return ret;
}

bool ICBaseIO::readu24(uint32_t &value)
{
	uint8_t data[3];

	bool ret = read(3, data);
	if (ret) {
		value = data[0];
		value <<= 8;
		value |= data[1];
		value <<= 8;
		value |= data[2];
	}

	return ret;
}

bool ICBaseIO::readu32(uint32_t &value)
{
	uint8_t data[4];

	bool ret = read(4, data);
	if (ret) {
		value = data[0];
		value <<= 8;
		value |= data[1];
		value <<= 8;
		value |= data[2];
		value <<= 8;
		value |= data[3];
	}

	return ret;
}

bool ICBaseIO::readu64(uint64_t &value)
{
	uint8_t data[8];

	bool ret = read(8, data);
	if (ret) {
		value = data[0];
		value <<= 8;
		value |= data[1];
		value <<= 8;
		value |= data[2];
		value <<= 8;
		value |= data[3];
		value <<= 8;
		value |= data[4];
		value <<= 8;
		value |= data[5];
		value <<= 8;
		value |= data[6];
		value <<= 8;
		value |= data[7];
	}

	return ret;
}

bool ICBaseIO::writeu8(uint8_t value)
{
	return write(1, &value);
}

bool ICBaseIO::writeu16(uint16_t value)
{
	uint8_t data[2] = { (value >> 8) & 0x00ff, value & 0x00ff };

	return write(2, data);
}

bool ICBaseIO::writeu24(uint32_t value)
{
	uint8_t data[3] = { (value >> 16) & 0x00ff, (value >> 8) & 0x00ff, value & 0x00ff };

	return write(3, data);
}

bool ICBaseIO::writeu32(uint32_t value)
{
	uint8_t data[4] = { (value >> 24) & 0x00ff, (value >> 16) & 0x00ff, (value >> 8) & 0x00ff, value & 0x00ff };

	return write(4, data);
}

bool ICBaseIO::writeu64(uint64_t value)
{
	uint8_t data[8] = { (value >> 56) & 0x00ff, (value >> 48) & 0x00ff, (value >> 40) & 0x00ff, (value >> 32) & 0x00ff,
		(value >> 24) & 0x00ff, (value >> 16) & 0x00ff, (value >> 8) & 0x00ff, value & 0x00ff };

	return write(8, data);
}


// CBufferIO 
CBufferIO::CBufferIO()
{
	_valid = false;
    _output_ptr = NULL;
    _buffer_size = 0;
    _buffer_ptr = NULL;
    _offset = 0;
    _end = 0;
}

CBufferIO::~CBufferIO()
{
}

bool CBufferIO::open(ICBaseIO *output_ptr)
{
	_output_ptr = output_ptr;
	_valid = true;
}

void CBufferIO::close()
{
	_valid = false;
	if (NULL != _buffer_ptr) {
		free(_buffer_ptr);
		_buffer_ptr = NULL;
	}
	_buffer_size = 0;
	_offset = 0;
	_end = 0;
}

uint64_t CBufferIO::get_size()
{
	return _end;
}

uint8_t* CBufferIO::get_buffer()
{
	return _buffer_ptr;
}

bool CBufferIO::is_valid()
{
	return _valid;
}

uint64_t CBufferIO::ftell()
{
	return _offset;
}

bool CBufferIO::seek(int64_t offset, int origin)
{
	bool error = false;
	switch(origin) 
	{
	case SEEK_SET:
		if (offset < 0 || offset > _end) {
			error = true;
		}
		else {
			_offset = offset;
		}
		break;
	case SEEK_CUR:
		if (offset + _offset > _end || offset + _offset < 0) {
			error = true;
		}
		else {
			_offset += offset;
		}
		break;
	case SEEK_END:
		if (offset > 0 || offset + _end < 0) {
			error = true;
		}
		else {
			_offset = _end + offset;
		}
		break;
	default:
		error = true;
		break;
	}
	
	return (!error);
}

bool CBufferIO::flush()
{
	bool success = true;
	if (NULL != _output_ptr) {
		uint64_t size_to_write = _end;
		uint8_t *ptr = _buffer_ptr;
		while(size_to_write >= UINT_MAX)
		{
			success = _output_ptr->write(UINT_MAX, ptr);
			if (!success)
				break;
			size_to_write -= UINT_MAX;
			ptr += UINT_MAX;
		}
		if (success && size_to_write > 0) {
			success = _output_ptr->write(size_to_write, ptr);
		}
	}

	_offset = 0;
	_end = 0;
	return success;
}

bool CBufferIO::read(uint32_t size, uint8_t *data_ptr)
{
	if (_offset + size > _end)
		return false;
	
	memcpy(data_ptr, _buffer_ptr + _offset, size);
	_offset += size;
	return true;
}

bool CBufferIO::write(uint32_t size, const uint8_t *data_ptr)
{
	if (!_ensure_size(size))
		return false;
	
	memcpy(_buffer_ptr + _offset, data_ptr, size);
	_offset += size;
	if (_offset > _end)
		_end = _offset;
	return true;
}


 bool CBufferIO::_ensure_size(uint32_t inc_size)
 {
	 uint64_t excp_size = _offset + inc_size;
	 if (_buffer_size >= excp_size)
	 	return true;

	excp_size = IO_ALIGN(excp_size, IO_ALIGN_VALUE);
	_buffer_ptr = (uint8_t *)realloc(_buffer_ptr, excp_size);
	if (NULL == _buffer_ptr) {
		_buffer_size = 0;
		_offset = 0;
		_end = 0;
		return false;
	}

	_buffer_size = excp_size;
	return true;
 }


// CCacheFileIO
CCacheFileIO::CCacheFileIO()
{
	_file_ptr = NULL;

	_cache_buffer_size = 0;
    _cache_buffer_ptr = NULL;
}

CCacheFileIO::~CCacheFileIO()
{
}

bool CCacheFileIO::open(const char *file_path_ptr, const char *flag_ptr)
{
	_file_ptr = fopen(file_path_ptr, flag_ptr);
	if (NULL == _file_ptr)
		return false;
	
	return true;
}

void CCacheFileIO::close()
{
	if (NULL != _file_ptr) {
		fclose(_file_ptr);
		_file_ptr = NULL;
	}
	if (NULL != _cache_buffer_ptr) {
		delete[] _cache_buffer_ptr;
		_cache_buffer_ptr = NULL;
	}
	_cache_buffer_size = 0;
}

bool CCacheFileIO::set_cache_size(uint32_t size)
{
	bool success = false;
	do {
		if (_cache_buffer_size != size) {
			if (NULL != _cache_buffer_ptr) {
				delete[] _cache_buffer_ptr;
				_cache_buffer_ptr = NULL;
			}
		}
		if (size > 0) {
			_cache_buffer_ptr = new uint8_t[size];
			if (NULL == _cache_buffer_ptr)
				break;
		}

		if (0 != setvbuf(_file_ptr, (char *)_cache_buffer_ptr, _IOFBF, size))
			break;

		_cache_buffer_size = size;
		success = true;
	} while(false);
	if (!success) {
		if (NULL != _cache_buffer_ptr) {
			delete[] _cache_buffer_ptr;
			_cache_buffer_ptr = NULL;
			_cache_buffer_size = 0;
		}
	}

	return success;
}

bool CCacheFileIO::is_end()
{
	return (0 != feof(_file_ptr));
}

bool CCacheFileIO::is_valid()
{
	return (NULL != _file_ptr);
}

uint64_t CCacheFileIO::ftell()
{
#ifdef WIN32
	return _ftelli64(_file_ptr);
#else
	return ftello64(_file_ptr);
#endif
}

bool CCacheFileIO::seek(int64_t offset, int origin)
{
#ifdef WIN32
	return (0 == _fseeki64(_file_ptr, offset, origin));
#else
	return (0 == fseeko64(_file_ptr, offset, origin));
#endif
}

bool CCacheFileIO::flush()
{
	return (0 == fflush(_file_ptr));
}

bool CCacheFileIO::read(uint32_t size, uint8_t *data_ptr)
{
	return (size == fread(data_ptr, sizeof(uint8_t), size, _file_ptr));
}

bool CCacheFileIO::write(uint32_t size, const uint8_t *data_ptr)
{
	return (size == fwrite(data_ptr, sizeof(uint8_t), size, _file_ptr));
}


// CFileIO
CFileIO::CFileIO()
{
	_file_fp = -1;
}

CFileIO::~CFileIO()
{
}

bool CFileIO::open(const char *file_path_ptr, int flags)
{
	_file_fp = open64(file_path_ptr, flags);
	if (-1 == _file_fp)
		return false;

	return true;
}

void CFileIO::close()
{
	if (-1 != _file_fp) {
		close(_file_fp);
		_file_fp = -1;
	}
}

bool CFileIO::is_valid()
{
	return (-1 != _file_fp);
}

uint64_t CFileIO::ftell()
{
	return lseek64(_file_fp, 0, SEEK_CUR);
}

bool CFileIO::seek(int64_t offset, int origin)
{
	if (lseek64(_file_fp, offset, origin) < 0)
		return false;
	
	return true;
}

bool CFileIO::flush()
{
	return (0 == fdatasync(_file_fp))
}

bool CFileIO::read(uint32_t size, uint8_t *data_ptr)
{
	return (size == read(_file_fd, data_ptr, size));
}

bool CFileIO::write(uint32_t size, const uint8_t *data_ptr)
{
	return (size == write(_file_fd, data_ptr, size));
}


// CDirectFileIO
CDirectFileIO::CDirectFileIO()
{
	_file_fp = -1;

    _page_size = 0;
}

CDirectFileIO::~CDirectFileIO()
{
}

bool CDirectFileIO::open(const char *file_path_ptr, int flags)
{
	if (!(flags & O_DIRECT) || !(flags & O_SYNC))
		flags |= O_DIRECT | O_SYNC;
	_file_fp = open64(file_path_ptr, flags);
	if (-1 == _file_fp)
		return false;

	_page_size = getpagesize();
	return true;
}

void CDirectFileIO::close()
{
	if (-1 != _file_fp) {
		close(_file_fp);
		_file_fp = -1;
	}
}

uint32_t CDirectFileIO::get_pagesize()
{
	return getpagesize();
}

bool CDirectFileIO::is_valid()
{
	return (-1 != _file_fp);
}

bool CDirectFileIO::read(uint32_t size, uint8_t *data_ptr)
{
	if (0 != size % _page_size)
		return false;
	
	return (size == read(_file_fp, data_ptr, size));
}

bool CDirectFileIO::write(uint32_t size, const uint8_t *data_ptr)
{
	if (0 != size % _page_size)
		return false;

	return (size == write(_file_fp, data_ptr, size));
}
