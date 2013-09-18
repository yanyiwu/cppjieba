#ifndef CPPJIEBA_SEGMENTBASE_H
#define CPPJIEBA_SEGMENTBASE_H

#include "globals.h"

namespace CppJieba
{
    class ISegment
    {
        public:
            virtual ~ISegment(){};
        public:
            virtual bool cut(const string& str, vector<string>& res) const = 0;
    };
}

#endif
