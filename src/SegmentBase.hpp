#ifndef CPPJIEBA_SEGMENTBASE_H
#define CPPJIEBA_SEGMENTBASE_H

#include "globals.h"
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
        private:
            bool _isInited;
        protected:
            bool _getInitFlag()const{return _isInited;};
            bool _setInitFlag(bool flag){return _isInited = flag;};
            bool cut(const string& str, vector<string>& res)const
            {
                if(!_getInitFlag())
                {
                    LogError("not inited.");
                    return false;
                }
                ChineseFilter filter;
                filter.feed(str);
                for(ChineseFilter::iterator it = filter.begin(); it != filter.end(); it++)
                {
                    if(it.charType == CHWORD)
                    {
                        cut(it.begin, it.end, res);
                    }
                    else
                    {
                        string tmp;
                        if(TransCode::encode(it.begin, it.end, tmp))
                        {
                            res.push_back(tmp);
                        }
                    }
                }
                return true;
            }
            bool cut(Unicode::const_iterator begin, Unicode::const_iterator end, vector<string>& res)const = 0;

    };
}

#endif
