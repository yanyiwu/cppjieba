/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#include "io_functs.h"

namespace CPPCOMMON
{
    string loadFile2Str(const char * const filepath)
    {
        ifstream in(filepath);
        if(!in)
        {
            return "";
        }
        istreambuf_iterator<char> beg(in), end;
        string str(beg, end);
        in.close();
        return str;
    }
    
    void loadStr2File(const char * const filename, ios_base::openmode mode, const string& str)
    {
        ofstream out(filename, mode);
        ostreambuf_iterator<char> itr (out);
        copy(str.begin(), str.end(), itr);
        out.close();
    }

    int ReadFromFile(const char * fileName, char* buf, int maxCount, const char* mode)
    {                                        
        FILE* fp = fopen(fileName, mode);    
        if (!fp)                             
          return 0;                          
        int ret;                             
        fgets(buf, maxCount, fp) ? ret = 1 : ret = 0;                                       
        fclose(fp);                          
        return ret;                          
    }                                        

    int WriteStr2File(const char* fileName, const char* buf, const char* mode)
    {                                          
        FILE* fp = fopen(fileName, mode);      
        if (!fp)                               
          return 0;                            
        int n = fprintf(fp, "%s", buf);        
        fclose(fp);                            
        return n;                              
    }                                          
}
#ifdef TEST_IO_FUNCTS
#include <iostream>
using namespace CPPCOMMON;
using namespace std;
int main()
{
//    char filename[] = "1/2/3";
//    cout<<loadFile2Str("1")<<endl;
    //string s = "hello world";
    //loadStr2File("testfile", ofstream::app, "hello world\n");
    //loadStr2File("testfile", ofstream::app, "hello world\n");
    //loadStr2File("testfile", ofstream::app, "hello world\n");
    cout<<loadFile2Str("testfile")<<endl;
    return 0;
}
#endif
