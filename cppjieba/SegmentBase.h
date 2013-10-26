#ifndef CPPJIEBA_SEGMENTBASE_H
#define CPPJIEBA_SEGMENTBASE_H

#include "globals.h"
#include "SegmentInterface.h"
#include "ChineseFilter.h"
#include <str_functs.hpp>
#include <logger.hpp>

namespace CppJieba
{
    using namespace Limonp;
    class SegmentBase: public SegmentInterface
    {
        public:
            SegmentBase(){_setInitFlag(false);};
            virtual ~SegmentBase(){};
        private:
            bool _isInited;
        protected:
            bool _getInitFlag()const{return _isInited;};
            bool _setInitFlag(bool flag){return _isInited = flag;};
            bool cut(const string& str, vector<string>& res)const;
            bool cut(Unicode::const_iterator begin, Unicode::const_iterator end, vector<string>& res)const = 0;

    };
}

#endif
