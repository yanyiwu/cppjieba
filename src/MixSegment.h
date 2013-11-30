#ifndef CPPJIEBA_MIXSEGMENT_H
#define CPPJIEBA_MIXSEGMENT_H

#include "MPSegment.hpp"
#include "HMMSegment.hpp"
#include "Limonp/str_functs.hpp"

namespace CppJieba
{
    class MixSegment: public SegmentBase
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
            //virtual bool cut(const string& str, vector<string>& res) const;
            bool cut(const string& str, vector<string>& res)const;
            bool cut(Unicode::const_iterator begin, Unicode::const_iterator end, vector<string>& res)const;
    };
}

#endif
