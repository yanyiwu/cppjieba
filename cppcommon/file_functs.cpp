/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#include "file_functs.h"

namespace CPPCOMMON
{
    bool checkFileExist(const string& filePath)
    {
        fstream _file;
        _file.open(filePath.c_str(), ios::in);
        if(_file)
          return true;
        return false;
    }
    bool createDir(const string& dirPath, bool p)
    {
        string dir_str(dirPath);
        string cmd = "mkdir";
        if(p)
        {
            cmd += " -p";
        }
        cmd += " " + dir_str; 
        int res = system(cmd.c_str());
        return res;
    }
    bool checkDirExist(const string& dirPath)
    {
        return checkFileExist(dirPath);
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
