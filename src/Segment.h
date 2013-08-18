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

namespace CppJieba
{
	class Segment
	{
		private:
			Trie _trie;
		public:
			Segment();
			~Segment();
		public:
			bool init();
			bool loadSegDict(const char * const filePath);
			bool dispose();
		public:
			bool cutDAG(const string& str, vector<TrieNodeInfo>& segWordInfos);
			bool cutDAG(const string& str, vector<string>& res);

		private:
			bool _calcDAG(SegmentContext& segContext);
			bool _calcDP(SegmentContext& segContext);
			bool _cutDAG(SegmentContext& segContext, vector<TrieNodeInfo>& res);

			//bool _fill(const string& )

	};
}

#endif
