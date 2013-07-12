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
			bool init(const char* const dictFilePath);
			bool destroy();
		public:
			bool cutDAG(const string& chStr, vector<string>& res);
			bool extract(const string& utf8Str, vector<string>& keywords, uint topN);
			double getUtf8WordWeight(const string& word);
			double getUniWordWeight(const string& word);

		private:
			string _utf8ToUni(const string& chStr);
			bool _calcDAG(const string& uniStr, vector<vector<uint> >& dag);
			bool _calcDP(const string& uniStr, const vector<vector<uint> >& dag, vector<pair<int, double> >& res);
			bool _cutDAG(const string& uniStr, const vector<pair<int, double> >& dp, vector<string>& res);
			bool _filter(vector<string>& utf8Strs);
			bool _filterDuplicate(vector<string>& utf8Strs);
			bool _filterSingleWord(vector<string>& utf8Strs);
			bool _filterSubstr(vector<string>& utf8Strs);
			static bool _pair_compare(const pair<string, double>& a, const pair<string, double>& b);
			bool _extractTopN(const vector<string>& words, vector<string>& keywords, uint topN);

		private:
			enum {bufSize = 1024};
			
	};
}

#endif
