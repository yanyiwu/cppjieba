#ifndef SEGMENT_H
#define SEGMENT_H

#include "Trie.h"

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
			bool init(const char* const dictFilePath);
			bool destroy();
		public:
			bool cutDAG(const string& chStr, vector<string>& res);
			bool cutMM(const string& chStr, vector<string>& res);
			bool cutRMM(const string& chStr, vector<string>& res);

		private:
			size_t _utf8ToUni(const string& chStr, ChUnicode* uniStr, size_t size);
			bool _calcDP(const ChUnicode* uniStr, size_t len, const vector<vector<uint> >& dag, vector<pair<int, double> >& res);

		private:
			enum {bufSize = 1024};
			
	};
}

#endif
