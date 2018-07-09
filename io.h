#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <limits.h>
#ifdef WIN32
#include <sys\types.h>
#else
#include <unistd.h> 
#include <fcntl.h>
#endif


#define IO_ALIGN_VALUE  1024
#define IO_ALIGN(size, align)   ((size + align - 1) & ~(align - 1))


class ICBaseIO
{
public:
    ICBaseIO() {}
    virtual ~ICBaseIO() {}

    virtual bool is_valid() = 0;
    virtual uint64_t ftell() = 0;
    virtual bool seek(int64_t offset, int origin) = 0;
    virtual bool flush() = 0;

    virtual bool read(uint32_t size, uint8_t *data_ptr) = 0;
    virtual bool write(uint32_t size, const uint8_t *data_ptr) = 0;
    bool readu8(uint8_t &value);
    bool readu16(uint16_t &value);
    bool readu24(uint32_t &value);
    bool readu32(uint32_t &value);
    bool readu64(uint64_t &value);
    bool writeu8(uint8_t value);
    bool writeu16(uint16_t value);
    bool writeu24(uint32_t value);
    bool writeu32(uint32_t value);
    bool writeu64(uint64_t value);
};


class CBufferIO : public ICBaseIO
{
public:
    CBufferIO();
    virtual ~CBufferIO();

    bool open(ICBaseIO *output_ptr);
    void close();
    uint64_t get_size();
    uint8_t *get_buffer();

    // ICBaseIO
    virtual bool is_valid();
    virtual uint64_t ftell();
    virtual bool seek(int64_t offset, int origin);
    virtual bool flush();
    virtual bool read(uint32_t size, uint8_t *data_ptr);
    virtual bool write(uint32_t size, const uint8_t *data_ptr);

protected:
    bool _ensure_size(uint32_t inc_size);

protected:
    bool _valid;
    ICBaseIO *_output_ptr;
    uint64_t _buffer_size;
    uint8_t *_buffer_ptr;
    uint64_t _offset;
    uint64_t _end;
};


class CCacheFileIO : public ICBaseIO
{
public:
    CCacheFileIO();
    virtual ~CCacheFileIO();

    bool open(const char *file_path_ptr, const char *flag_ptr);
    void close();
    bool set_cache_size(uint32_t size);
    bool is_end();

    // ICBaseIO
    virtual bool is_valid();
    virtual uint64_t ftell();
    virtual bool seek(int64_t offset, int origin);
    virtual bool flush();
    virtual bool read(uint32_t size, uint8_t *data_ptr);
    virtual bool write(uint32_t size, const uint8_t *data_ptr);

protected:
    FILE *_file_ptr;

    uint32_t _cache_buffer_size;
    uint8_t *_cache_buffer_ptr;
};


#ifdef WIN32
#else
#define _GNU_SOURCE


class CFileIO : public ICBaseIO
{
public:
    CFileIO();
    virtual ~CFileIO();

    bool open(const char *file_path_ptr, int flags);
    void close();

    // ICBaseIO
    virtual bool is_valid();
    virtual uint64_t ftell();
    virtual bool seek(int64_t offset, int origin);
    virtual bool flush();
    virtual bool read(uint32_t size, uint8_t *data_ptr);
    virtual bool write(uint32_t size, const uint8_t *data_ptr);

protected:
    int _file_fd;
};


class CDirectFileIO
{
public:
    CDirectFileIO();
    virtual ~CDirectFileIO();

    bool open(const char *file_path_ptr, int flags);
    void close();
    uint32_t get_pagesize();

    bool is_valid();
    bool read(uint32_t size, uint8_t *data_ptr);
    bool write(uint32_t size, const uint8_t *data_ptr);

protected:
    int _file_fp;

    uint32_t _page_size;
};
#endif