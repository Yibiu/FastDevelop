#include "pipe.h"


// CPipeExec
CPipeExec::CPipeExec()
{
	_file_ptr = NULL;
}

CPipeExec::~CPipeExec()
{
}

bool CPipeExec::open(const std::string &cmd)
{
	_file_ptr = popen(cmd.c_str(), "r");
	if (NULL == _file_ptr)
		return false;
	
	return true;
}

void CPipeExec::close()
{
	if (NULL != _file_ptr) {
		pclose(_file_ptr);
		_file_ptr = NULL;
	}
}

bool CPipeExec::readline(std::string &line)
{
	if (NULL == _file_ptr)
		return false;

	char result[256];
	memset(result, 0x00, sizeof(result));
	fgets(result, sizeof(result), _file_ptr);
	line = result;

	return true;
}
