#ifndef CPPJIEBA_MIXSEGMENT_H
#define CPPJIEBA_MIXSEGMENT_H

#include "MPSegment.h"
#include "HMMSegment.h"
#include "../cppcommon/headers.h"

namespace CppJieba
{
    class MixSegment: public SegmentBase, public SegmentInterface
    {
        private:
            MPSegment _mpSeg;
            HMMSegment _hmmSeg;
        public:
            MixSegment();
            virtual ~MixSegment();
        public:
            bool init(const char* const _mpSegDict, const char* const _hmmSegDict);
            bool dispose();
        public:
            virtual bool cut(const string& str, vector<string>& res) const;
        private:
    };
}

#endif
