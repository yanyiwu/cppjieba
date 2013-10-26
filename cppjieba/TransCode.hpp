/************************************
 * file enc : utf-8
 * author   : wuyanyi09@gmail.com
 ************************************/
#ifndef CPPJIEBA_TRANSCODE_H
#define CPPJIEBA_TRANSCODE_H


#include "globals.h"
#include <str_functs.hpp>
#include <vec_functs.hpp>

namespace CppJieba
{

    using namespace Limonp;
    namespace TransCode
    {
        inline bool decode(const string& str, vector<uint16_t>& vec)
        {
            char ch1, ch2;
            if(str.empty())
            {
                return false;
            }
            vec.clear();
            size_t siz = str.size();
            for(uint i = 0;i < siz;)
            {
                if(!(str[i] & 0x80)) // 0xxxxxxx
                {
                    vec.push_back(str[i]);
                    i++;
                }
                else if ((unsigned char)str[i] <= 0xdf && i + 1 < siz) // 110xxxxxx
                {
                    ch1 = (str[i] >> 2) & 0x07;
                    ch2 = (str[i+1] & 0x3f) | ((str[i] & 0x03) << 6 );
                    vec.push_back(twocharToUint16(ch1, ch2));
                    i += 2;
                }
                else if((unsigned char)str[i] <= 0xef && i + 2 < siz)
                {
                    ch1 = (str[i] << 4) | ((str[i+1] >> 2) & 0x0f );
                    ch2 = ((str[i+1]<<6) & 0xc0) | (str[i+2] & 0x3f); 
                    vec.push_back(twocharToUint16(ch1, ch2));
                    i += 3;
                }
                else
                {
                    return false;
                }
            }
            return true;
        }


        inline bool encode(vector<uint16_t>::const_iterator begin, vector<uint16_t>::const_iterator end, string& res)
        {
            if(begin >= end)
            {
                return false;
            }
            res.clear();
            uint16_t ui;
            while(begin != end)
            {
                ui = *begin;
                if(ui <= 0x7f)
                {
                    res += char(ui);
                }
                else if(ui <= 0x7ff)
                {
                    res += char(((ui>>6) & 0x1f) | 0xc0);
                    res += char((ui & 0x3f) | 0x80);
                }
                else
                {
                    res += char(((ui >> 12) & 0x0f )| 0xe0);
                    res += char(((ui>>6) & 0x3f )| 0x80 );
                    res += char((ui & 0x3f) | 0x80);
                }
                begin ++;
            }
            return true;
        }
        inline bool encode(const vector<uint16_t>& sentence, string& res)
        {
            return encode(sentence.begin(), sentence.end(), res);
        }
    }
}

#endif
