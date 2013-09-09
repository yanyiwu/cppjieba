#ifndef CPPJIEBA_MIXSEGMENT_H
#define CPPJIEBA_MIXSEGMENT_H

#include "MPSegment.h"
#include "HMMSegment.h"
#include "../cppcommon/headers.h"

namespace CppJieba
{
	class MixSegment
	{
        private:
            MPSegment _mpSeg;
            HMMSegment _hmmSeg;
		public:
			MixSegment();
			~MixSegment();
		public:
			bool init(const char* const _mpSegDict, const char* const _hmmSegDict);
			bool dispose();
        public:
            bool cut(const string& str, vector<string>& res);
	};
}

#endif
