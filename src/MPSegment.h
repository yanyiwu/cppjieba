/************************************
 * file enc : ASCII
 * author   : wuyanyi09@gmail.com
************************************/
#ifndef CPPJIEBA_SEGMENT_H
#define CPPJIEBA_SEGMENT_H

#include <algorithm>
#include <set>
#include "Trie.h"
#include "globals.h"
#include "HMMSegment.h"

namespace CppJieba
{

    typedef vector<SegmentChar> SegmentContext;

	class MPSegment
	{
		private:
			Trie _trie;
			
		public:
			MPSegment();
			~MPSegment();
		public:
			bool init(const char* const filePath);
			bool dispose();
		public:
			bool cut(const string& str, vector<TrieNodeInfo>& segWordInfos);
			bool cut(const string& str, vector<string>& res);

		private:
			bool _calcDAG(SegmentContext& segContext);
			bool _calcDP(SegmentContext& segContext);
			bool _cut(SegmentContext& segContext, vector<TrieNodeInfo>& res);


	};
}

#endif
