#pragma once
#include <stdint.h>
#include <Windows.h>


/**
* @brief:
* File mapping class for shared memory
*/
class CFileMapping
{
public:
	CFileMapping()
	{
		_map_file = NULL;
	}
	virtual ~CFileMapping() {}

	bool open(const char *path_ptr, uint32_t size, bool create = false)
	{
		if (create) {
			_map_file = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, path_ptr);
		}
		else {
			_map_file = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, path_ptr);
		}

		return (NULL != _map_file);
	}
	void close()
	{
		if (NULL != _map_file) {
			CloseHandle(_map_file);
			_map_file = NULL;
		}
	}
	void* map()
	{
		return MapViewOfFile(_map_file, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	}
	void unmap(void *ptr)
	{
		UnmapViewOfFile(ptr);
	}
	bool is_opened()
	{
		return (NULL != _map_file);
	}

protected:
	HANDLE _map_file;
};

