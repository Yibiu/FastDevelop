#include "directory.h"


CDirHelpFunctions::CDirHelpFunctions()
{
}

CDirHelpFunctions::~CDirHelpFunctions()
{
}

bool CDirHelpFunctions::is_dir(const std::string &file_path)
{
#ifdef WIN32
	return (TRUE == PathIsDirectoryA(file_path.c_str())) ? true : false;
#else
	struct stat mode;
	if (lstat(file_path.c_str(), &mode) < 0)
		return false;

	return (S_ISDIR(mode.st_mode));
#endif
}

bool CDirHelpFunctions::is_access_exist(const std::string &file_path)
{
#ifdef WIN32
	return (0 == _access(file_path.c_str(), 0));
#else
	return (0 == access(file_path.c_str(), F_OK));
#endif
}

bool CDirHelpFunctions::is_access_read(const std::string &file_path)
{
#ifdef WIN32
	return (0 == _access(file_path.c_str(), 4));
#else
	return (0 == access(file_path.c_str(), R_OK));
#endif
}

bool CDirHelpFunctions::is_access_write(const std::string &file_path)
{
#ifdef  WIN32
	return (0 == _access(file_path.c_str(), 2));
#else
	return (0 == access(file_path.c_str(), W_OK));
#endif
}

bool CDirHelpFunctions::is_access_execute(const std::string &file_path)
{
#ifdef WIN32
	return (0 == _access(file_path.c_str(), 6));
#else
	return (0 == access(file_path.c_str(), X_OK));
#endif
}

bool CDirHelpFunctions::create_folder(const std::string &dir_path)
{
#ifdef WIN32
	if (TRUE != CreateDirectoryA(dir_path.c_str(), 0))
		return false;
#else
	if (-1 == mkdir(dir_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
		return false;
#endif

	return true;
}

bool CDirHelpFunctions::get_folder_size(const std::string &dir_path, uint32_t &total_size_kb, uint32_t &avail_size_kb)
{
#ifdef WIN32
	char disk_name[16];
	memset(disk_name, 0x00, sizeof(disk_name));
	strncpy(disk_name, dir_path.c_str(), 3);

	bool success = false;
	char logical_string_buffer[MAX_PATH];
	GetLogicalDriveStringsA(MAX_PATH, logical_string_buffer);
	const char *ptr = logical_string_buffer;
	while (*ptr) {
		std::string disk_string = ptr;
		if (disk_string.empty()) {
			break;
		}

		if (0 == disk_string.compare(disk_name)) {
			uint64_t free_bytes_available;
			uint64_t total_number_of_bytes;
			uint64_t total_number_of_free_bytes;
			if (GetDiskFreeSpaceExA(
				ptr,
				(PULARGE_INTEGER)&free_bytes_available,
				(PULARGE_INTEGER)&total_number_of_bytes,
				(PULARGE_INTEGER)&total_number_of_free_bytes
				)) {
				total_size_kb = (uint32_t)(total_number_of_bytes / 1024);
				avail_size_kb = (uint32_t)(free_bytes_available / 1024);
				success = true;
				break;
			}
		}

		ptr += (strlen(ptr) + 1);
	}

	if (!success) {
		return false;
	}

#else
	// df -T dir
	// Filesystem	Type	1K-blocks	Used	Available	Use%	Mounted on
	// /dev/sda2	ext2	9076396		5344224	3732172		59%		/
	/*bool error = false;
	FILE *fp = NULL;
	do {
		char command[64], result[256];
		memset(command, 0x00, sizeof(command));
		if (-1 == sprintf(command, "df -T %s", dir_path.c_str())) {
			error = true;
			break;
		}
		fp = popen(command, "r");
		if (NULL == fp) {
			error = true;
			break;
		}
		for (int i = 0; i < 2; i++) {
			memset(result, 0x00, sizeof(result));
			if (NULL == fgets(result, sizeof(result), fp)) {
				error = true;
				break;
			}
		}
		if (error)
			break;

		char file_system[64], type[64], one_kb_blocks[64], used_blocks[64],
			available_blocks[64], used_percent[64], mounted_on[64];
		if (-1 == sscanf(result, "%s%s%s%s%s%s%s", file_system, type, one_kb_blocks,
			used_blocks, available_blocks, used_percent, mounted_on)) {
			error = true;
			break;
		}

		avail_size_in_kb = atoi(available_blocks);
	} while (false);
	if (NULL != fp)
		pclose(fp);

	if (error) {
		return false;
	}*/
	struct statfs folder_info;
	if (0 != statfs(dir_path.c_str(), &folder_info))
		return false;
	total_size_kb = (uint64_t)folder_info.f_blocks * folder_info.f_bsize / 1024;
	avail_size_kb = (uint64_t)folder_info.f_bavail * folder_info.f_bsize / 1024;
#endif

	return true;
}

bool CDirHelpFunctions::walk_through_folder(const std::string &folder_path, std::vector<std::string> file_paths)
{
	bool error = false;
	file_paths.clear();
	do {
#ifdef WIN32
		WIN32_FIND_DATAA find_file_data;
		HANDLE file_handle = FindFirstFileA(folder_path.c_str(), &find_file_data);
		if (INVALID_HANDLE_VALUE == file_handle) {
			error = true;
			break;
		}

		do {
			if (0 == strcmp(find_file_data.cFileName, ".")) {
			}
			else if (0 == strcmp(find_file_data.cFileName, "..")) {
			}
			else {
				if (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					continue;

				std::string file_path = folder_path + find_file_data.cFileName;
				file_paths.push_back(file_path);
			}
		} while (FindNextFileA(file_handle, &find_file_data));
		CloseHandle(file_handle);

#else
		struct dirent *file_info_ptr;
		DIR *dir_ptr = opendir(folder_path.c_str());
		if (NULL == dir_ptr) {
			error = true;
			break;
		}

		while (NULL != (file_info_ptr = readdir(dir_ptr)))
		{
			if (0 == strcmp(file_info_ptr->d_name, ".")) {
			}
			else if (0 == strcmp(file_info_ptr->d_name, "..")) {
			}
			else {
				if (DT_DIR == file_info_ptr->d_type)
					continue;

				std::string file_path = folder_path + file_info_ptr->d_name;
				file_paths.push_back(file_path);
			}
		}
		closedir(dir_ptr);
#endif
	} while (false);

	return (!error);
}

bool CDirHelpFunctions::get_file_size(const std::string &file_path, uint64_t &size_in_byte)
{
#ifdef WIN32
	HANDLE handle = CreateFileA(file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == handle) {
		return false;
	}
	LARGE_INTEGER size;
	::GetFileSizeEx(handle, &size);
	size_in_byte = size.QuadPart;
	CloseHandle(handle);

#else
	struct stat statbuf;
	if (-1 == lstat(file_path.c_str(), &statbuf)) {
		return false;
	}
	size_in_byte = statbuf.st_size;
#endif

	return true;
}

bool CDirHelpFunctions::get_file_type(const std::string &file_path)
{
#ifdef WIN32
	return false;
#else
	struct stat statbuf;
	if (lstat(file_path.c_str(), statbuf) < 0)
		return false;
	
	if (S_ISREG(statbuf.st_mode)) {
		printf("regular\n");
	}
	else if (S_ISDIR(statbuf.st_mode)) {
		printf("directory\n");
	}
	else if (S_ISCHR(statbuf.st_mode)) {
		printf("character special\n");
	}
	else if (S_ISBLK(statbuf.st_mode)) {
		printf("block special\n");
	}
	else if (S_ISFIFO(statbuf.st_mode)) {
		printf("fifo\n");
	}
	else if (S_ISLNK(statbuf.st_mode)) {
		printf("symbolic link\n");
	}
	else if (S_ISSOCK(statbuf.st_mode)) {
		printf("socket\n");
	}

	return true;
#endif
}

bool CDirHelpFunctions::truncate_file(const std::string &file_path, uint64_t offset)
{
#ifdef WIN32
	HANDLE handle = CreateFileA(file_path.c_str(), GENERIC_WRITE | GENERIC_READ,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == handle)
		return false;

	LARGE_INTEGER li;
	li.QuadPart = offset;
	SetFilePointerEx(handle, li, NULL, FILE_BEGIN);
	bool bRet = SetEndOfFile(handle);
	CloseHandle(handle);
	return bRet;
#else
	return (0 == truncate(file_path.c_str(), offset));
#endif
}

bool CDirHelpFunctions::remove_file(const std::string &file_path)
{
	return (0 == remove(file_path.c_str()));
}

