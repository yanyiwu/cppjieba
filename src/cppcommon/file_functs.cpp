/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#include "file_functs.h"

namespace CPPCOMMON
{
	using namespace std;
	bool checkFileExist(const char * filepath)
	{
		fstream _file;
		_file.open(filepath, ios::in);
		if(_file)
		  return true;
		return false;
	}
	bool createDir(const char * dir_path, bool p)
	{
		string dir_str(dir_path);
		string cmd = "mkdir";
		if(p)
		{
			cmd += " -p";
		}
		cmd += " " + dir_str; 
		int res = system(cmd.c_str());
		return res;
	}
	bool checkDirExist(const char * dir_path)
	{
		return checkFileExist(dir_path);
	}
}

#ifdef TEST_FILE_FUNCTS
#include <iostream>
using namespace CPPCOMMON;
using namespace std;
int main()
{
	char filename[] = "1/2/3";
	if(!checkFileExist(filename))
	{
		createDir(filename);
	}
	return 0;
}
#endif
