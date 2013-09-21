#ifndef CPPJIEBA_SEGMENTBASE_H
#define CPPJIEBA_SEGMENTBASE_H

#include "globals.h"
#include "SegmentInterface.h"
#include "ChineseFilter.h"
#include "../cppcommon/headers.h"

namespace CppJieba
{
    using namespace CPPCOMMON;
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

    };
}

#endif
