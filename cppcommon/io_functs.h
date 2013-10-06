/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#ifndef CPPCOMMON_IO_FUNCTS_H
#define CPPCOMMON_IO_FUNCTS_H
#include <fstream>
#include <iostream>
namespace CPPCOMMON
{
    using namespace std;
    string loadFile2Str(const char * const filepath);
    void loadStr2File(const char * const filename, ios_base::openmode mode, const string& str);
    int ReadFromFile(const char * fileName, char* buf, int maxCount, const char* mode);
    int WriteStr2File(const char* fileName, const char* buf, const char* mode);
}
#endif
