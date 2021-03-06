#include "logger.h"


CLogger* CLogger::_instance_ptr = NULL;


CLogger::CLogger()
{
	_levels = 0;
	_tags.clear();
#ifdef WIN32
	_console = NULL;
#endif
	_console_valid = false;
}

CLogger::~CLogger()
{
	if (NULL != _instance_ptr) {
		delete _instance_ptr;
		_instance_ptr = NULL;
	}
}

CLogger* CLogger::get_instance()
{
	if (NULL == _instance_ptr) {
		_instance_ptr = new CLogger;
	}

	return _instance_ptr;
}

void CLogger::add_level(log_level_t level)
{
	_levels |= level;
}

void CLogger::remove_level(log_level_t level)
{
	_levels &= ~level;
}

bool CLogger::has_level(log_level_t level)
{
	return (0 != (_levels & level));
}

void CLogger::add_tag(const char *tag)
{
	if (!_found_tag(tag)) {
		_tags.push_back(tag);
	}
}

void CLogger::remove_tag(const char *tag)
{
	_found_tag(tag, true);
}

void CLogger::log(log_level_t level, const char *tag, const char *fmt, ...)
{
	if (!_console_valid && !_file.is_open())
		return;
	if (!has_level(level))
		return;
	if (!_found_tag(tag))
		return;

	char str[1024];
	memset(str, 0x00, 1024);
	va_list va;
	va_start(va, fmt);
	vsprintf(str, fmt, va);
	va_end(va);
	strcat(str, "\n");

	std::string logger = "";
	switch (level)
	{
	case LOG_LEVEL_DEBUG:
		logger = std::string("DEBUG");
		break;
	case LOG_LEVEL_WARNING:
		logger = std::string("WARNING");
		break;
	case LOG_LEVEL_ERROR:
		logger = std::string("ERROR");
		break;
	default:
		break;
	}
	logger = "[" + logger + "|" + tag + "]: " + str;
	if (_file.is_open()) {
		_file << logger.c_str();
		_file.flush();
	}
	if (_console_valid) {
#ifdef WIN32
		WriteConsole(_console, logger.c_str(), logger.length(), 0, 0);
#else
		printf(logger.c_str());
#endif
	}
}

bool CLogger::open_file(const char *path_ptr)
{
	_file.open(path_ptr);
	return _file.is_open();
}

void CLogger::close_file()
{
	_file.flush();
	_file.close();
}

bool CLogger::open_console()
{
#ifdef WIN32
	AllocConsole();
	freopen("CON", "w", stdout);
	_console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (NULL != _console) {
		_console_valid = true;
	}
#else
	_console_valid = true;
#endif

	return _console_valid;
}

void CLogger::close_console()
{
	_console_valid = false;
#ifdef WIN32
	if (NULL != _console) {
		FreeConsole();
	}
	_console = NULL;
#endif
}


bool CLogger::_found_tag(const char *tag, bool remove)
{
	bool found = false;
	std::vector<std::string>::iterator iter;
	for (iter = _tags.begin(); iter != _tags.end(); iter++) {
		std::string cur_tag = (*iter);
		if (0 == cur_tag.compare(tag)) {
			if (remove) {
				_tags.erase(iter);
			}
			found = true;
			break;
		}
	}

	return found;
}

