#ifndef CPPJIEBA_SEGMENTINTERFACE_H
#define CPPJIEBA_SEGMENTINTERFACE_H

#include "globals.h"

namespace CppJieba
{
    class SegmentInterface
    {
        public:
            virtual ~SegmentInterface(){};
        public:
            virtual bool cut(const string& str, vector<string>& res) const = 0;
    };
}

#endif
