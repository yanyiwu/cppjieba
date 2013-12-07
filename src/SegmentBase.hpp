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
                const char * const cstr = str.c_str();
                uint size = str.size();
                uint offset = 0;
                string subs;
                int ret;
                uint len;
                Unicode unico;
                while(offset < size)
                {
                    if(-1 == (ret = filterAscii(cstr + offset, size, len)))
                    {
                        LogFatal("str[%s] illegal.", cstr);
                        return false;
                    }
                    subs.assign(cstr + offset, len);
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
                    if(len == 0)
                    {
                        LogFatal("str[%s] illegal.", cstr);
                        return false;
                    }
                    offset += len;
                }
                return true;
                //ChineseFilter filter;
                //filter.feed(str);
                //for(ChineseFilter::iterator it = filter.begin(); it != filter.end(); it++)
                //{
                //    if(it.charType == CHWORD)
                //    {
                //        cut(it.begin, it.end, res);
                //    }
                //    else
                //    {
                //        string tmp;
                //        if(TransCode::encode(it.begin, it.end, tmp))
                //        {
                //            res.push_back(tmp);
                //        }
                //    }
                //}
                //return true;
            }

    };
}

#endif
