/************************************
 * author   : wuyanyi09@gmail.com
 ************************************/
#ifndef CPPJIEBA_TRANSCODE_H
#define CPPJIEBA_TRANSCODE_H


#include "globals.h"
#include "../cppcommon/str_functs.h"
#include "../cppcommon/vec_functs.h"

namespace CppJieba
{

    namespace TransCode
    {

        inline bool encode(vector<uint16_t>::const_iterator begin, vector<uint16_t>::const_iterator end, string& res)
        {
            if(begin >= end)
            {
                return false;
            }
            res.clear();
            pair<char, char> pa;
            while(begin != end)
            {
                pa = CPPCOMMON::uint16ToChar2(*begin);
                if(pa.first & 0x80)
                {
                    res += pa.first;
                    res += pa.second;
                }
                else
                {
                    res += pa.second;
                }
                begin++;
            }
            return true;
        }

        inline bool encode(const vector<uint16_t>& sentence, string& res)
        {
            return encode(sentence.begin(), sentence.end(), res);
        }

        inline bool decode(const string& str, vector<uint16_t>& vec)
        {
            vec.clear();
            if(str.empty())
            {
                return false;
            }
            uint i = 0;
            while(i < str.size())
            {
                if(0 == (str[i] & 0x80))
                {
                    vec.push_back(uint16_t(str[i]));
                    i++;
                }
                else
                {
                    if(i + 1 < str.size()) //&& (str[i+1] & 0x80))
                    {
                        vec.push_back(CPPCOMMON::twocharToUint16(str[i], str[i + 1]));
                        i += 2;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            return true;
        }

    }
}

#endif
