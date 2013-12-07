#ifndef CPPJIEBA_SEGMENTBASE_H
#define CPPJIEBA_SEGMENTBASE_H

#include "ISegment.hpp"
#include "ChineseFilter.hpp"
#include "Limonp/str_functs.hpp"
#include "Limonp/logger.hpp"

namespace CppJieba
{
    using namespace Limonp;
    class SegmentBase: public ISegment
    {
        public:
            SegmentBase(){_setInitFlag(false);};
            virtual ~SegmentBase(){};
        protected:
            bool _isInited;
            bool _getInitFlag()const{return _isInited;};
            bool _setInitFlag(bool flag){return _isInited = flag;};
        public:
            virtual bool init() = 0;
            virtual bool dispose() = 0;
        
        public:
            virtual bool cut(Unicode::const_iterator begin, Unicode::const_iterator end, vector<string>& res)const = 0;
            virtual bool cut(const string& str, vector<string>& res)const
            {
                if(!_getInitFlag())
                {
                    LogError("not inited.");
                    return false;
                }
                Unicode unico;
#ifdef NO_FILTER
                unico.clear();
                if(!TransCode::decode(str, unico))
                {
                    LogFatal("str[%s] decode failed.", str.c_str());
                    return false;
                }
                return cut(unico.begin(), unico.end(), res);
#else
                const char * const cstr = str.c_str();
                uint size = str.size();
                uint offset = 0;
                string subs;
                int ret;
                uint len;
                while(offset < size)
                {
                    const char * const nstr = cstr + offset;
                    uint nsize = size - offset;
                    if(-1 == (ret = filterAscii(nstr, nsize, len)) || 0 == len || len > nsize)
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
#endif
            }

    };
}

#endif
