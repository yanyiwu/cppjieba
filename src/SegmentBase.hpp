#ifndef CPPJIEBA_SEGMENTBASE_H
#define CPPJIEBA_SEGMENTBASE_H

#include "TransCode.hpp"
#include "Limonp/logger.hpp"
#include "Limonp/InitOnOff.hpp"
#include "ISegment.hpp"
#include <cassert>


namespace CppJieba
{
    using namespace Limonp;

    const char* const SPECIAL_CHARS = " \t\n";

    class SegmentBase: public ISegment, public InitOnOff
    {
        public:
            SegmentBase(){};
            virtual ~SegmentBase(){};

        public:
            virtual bool cut(Unicode::const_iterator begin, Unicode::const_iterator end, vector<string>& res)const = 0;
            virtual bool cut(const string& str, vector<string>& res)const
            {
                assert(_getInitFlag());
                Unicode unico;
                res.clear();
                const char * const cstr = str.c_str();
                size_t size = str.size();
                size_t offset = 0;
                string subs;
                int ret;
                size_t len;
                while(offset < size)
                {
                    const char * const nstr = cstr + offset;
                    size_t nsize = size - offset;
                    if(-1 == (ret = filterSpecialChars(nstr, nsize, len)) || 0 == len || len > nsize)
                    {
                        LogFatal("str[%s] illegal.", cstr);
                        return false;
                    }
                    subs.assign(nstr, len);
                    if(!ret)
                    {
                        res.push_back(subs);
                    }
                    else
                    {
                        unico.clear();
                        if(!TransCode::decode(subs, unico))
                        {
                            LogFatal("str[%s] decode failed.", subs.c_str());
                            return false;
                        }
                        cut(unico.begin(), unico.end(), res);
                    }
                    offset += len;
                }
                return true;
            }
        public:

            /*
             * if char is SPECIAL_CHARS, count the SPECITAL_CHARS string's length and return 0;
             * else count the NO SPECIAL_CHARS string's length and return 1;
             * if errors, return -1;
             * */
            static int filterSpecialChars(const char* str, size_t len, size_t& resLen)
            {
                if(!str || !len)
                {
                    return -1;
                }

                resLen = 1;
                int flag = (strchr(SPECIAL_CHARS, *str) ?  0: 1);
                for(size_t i = 1; i < len && bool(flag) != bool(strchr(SPECIAL_CHARS, str[i])); i++)
                {
                    resLen ++;
                }
                return flag;

            }

            /*
             * if char is ascii, count the ascii string's length and return 0;
             * else count the nonascii string's length and return 1;
             * if errors, return -1;
             * */
            static int filterAscii(const char* str, size_t len, size_t& resLen)
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

    };
}

#endif
