#ifndef CPPJIEBA_CHINESEFILTER_H
#define CPPJIEBA_CHINESEFILTER_H

#include "TransCode.hpp"

namespace CppJieba
{

    /*
     * if char is ascii, count the ascii string's length and return 0;
     * else count the nonascii string's length and return 1;
     * if errors, return -1;
     * */
    inline int filterAscii(const char* str, uint len, uint& resLen)
    {
        if(!str || !len)
        {
            return -1;
        }
        char x = 0x80;
        int resFlag = (str[0] & x ? 1 : 0);
        resLen = 0;
        if(!resFlag)
        {
            while(resLen < len && !(str[resLen] & x))
            {
                resLen ++;
            }
        }
        else
        {
            while(resLen < len && (str[resLen] & x))
            {
#ifdef CPPJIEBA_GBK
                resLen += 2;
#else
                resLen ++;
#endif
            }
        }
        if(resLen > len)
        {
            return -1;
        }
        return resFlag;
    }
}

#endif
