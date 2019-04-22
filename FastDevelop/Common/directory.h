#pragma once
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#ifdef WIN32
#include <io.h>
#include <sys\types.h>
#include <Windows.h>
#include<Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#else
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#endif


class CDirHelpFunctions
{
public:
	CDirHelpFunctions();
	virtual ~CDirHelpFunctions();

	static bool is_dir(const std::string &file_path);
	static bool is_access_exist(const std::string &file_path);
	static bool is_access_read(const std::string &file_path);
	static bool is_access_write(const std::string &file_path);
	static bool is_access_execute(const std::string &file_path);
	static bool create_folder(const std::string &dir_path);
	static bool get_folder_size(const std::string &dir_path, uint32_t &total_size_kb, uint32_t &avail_size_kb);
	static bool walk_through_folder(const std::string &folder_path, std::vector<std::string> file_paths);
	static bool get_file_size(const std::string &file_path, uint64_t &size_in_byte);
	static bool get_file_type(const std::string &file_path);
	static bool truncate_file(const std::string &file_path, uint64_t offset);
	static bool remove_file(const std::string &file_path);
};

