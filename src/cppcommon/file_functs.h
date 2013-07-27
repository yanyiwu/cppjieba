/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#ifndef CPPCOMMON_FILE_FUNCTS_H
#define CPPCOMMON_FILE_FUNCTS_H

#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
namespace CPPCOMMON
{
	using namespace std;
	bool checkFileExist(const string& filePath);
	bool createDir(const string& dirPath, bool p = true);
	bool checkDirExist(const string& dirPath);

}

#endif
