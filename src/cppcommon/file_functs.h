#ifndef CPPCOMMON_FILE_FUNCTS_H
#define CPPCOMMON_FILE_FUNCTS_H

#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
namespace CPPCOMMON
{
	bool checkFileExist(const char * filepath);
	bool createDir(const char * dir_path, bool p = true);
	bool checkDirExist(const char * dir_path);

}

#endif
