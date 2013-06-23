#include <iostream>
#include <iconv.h>
#include <stdint.h>
#include <fstream>
#include <memory.h>
#include "cppcommon/str_functs.h"
using namespace std;
using namespace CPPCOMMON;

int main()
{
    ifstream ifile("dict.txt");
    string line;
    while(getline(ifile, line))
    {
        uint16_t strbuf[1024];

        size_t unilen = utf8ToUnicode(line.c_str(), line.size(), strbuf);
        for(int i = 0; i < unilen; i++)
        {
            // printf("%x\n", strbuf[i]);
        }
        char utf8str[512]={0};
        unicodeToUtf8(strbuf, unilen, utf8str);
        //cout<<strlen(utf8str);
        cout<<utf8str<<endl;
    }
    return 0;
}
