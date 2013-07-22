/************************************
 * file enc : utf8
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
			bool loadSegDict(const string& filePath);
			bool dispose();
			double getWordWeight(const string& str);
		public:
			bool cutDAG(const string& chStr, vector<string>& res);

		private:
			bool _calcDAG(const VUINT16& unicode, vector<vector<uint> >& dag);
			bool _calcDP(const VUINT16& unicode, const vector<vector<uint> >& dag, vector<pair<int, double> >& res);
			bool _cutDAG(const VUINT16& unicode, const vector<pair<int, double> >& dp, vector<string>& res);

	};
}

#endif
