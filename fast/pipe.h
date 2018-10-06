#include <stdio.h>
#include <string>


class CPipeExec
{
public:
	CPipeExec();
	virtual ~CPipeExec();

	bool open(const std::string &cmd);
	void close();
	bool readline(std::string &line);

protected:
	FILE *_file_ptr;
};
