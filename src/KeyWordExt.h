#ifndef CPPJIEBA_KEYWORDEXT_H
#define CPPJIEBA_KEYWORDEXT_H

#include "Segment.h"

namespace CppJieba
{

	class KeyWordExt
	{
		private:
			Segment _segment;
			Trie _priorPrefixTrie;
			set<string> _stopWords;
		public:
			KeyWordExt();
			~KeyWordExt();
			bool init(const char * const filePath);

			//load stopwords
			bool loadStopWords(const char * const filePath);

			//load prior words' prefix
			bool loadPriorWordPrefixes( const char * const filePath);

			bool destroy();

		public:
			bool extract(const string& utf8Str, vector<string>& keywords, uint topN);
		private:
			static bool _pair_compare(const pair<string, double>& a, const pair<string, double>& b);
			bool _extractTopN(const vector<string>& words, vector<string>& keywords, uint topN);
			bool _filter(vector<string>& utf8Strs);
			bool _filterDuplicate(vector<string>& utf8Strs);
			bool _filterSingleWord(vector<string>& utf8Strs);
			bool _filterSubstr(vector<string>& utf8Strs);
			bool _filterStopWords(vector<string>& utf8Strs);
		private:
			bool _priorWordPrefixes(vector<string>& utf8Strs);

	};

}

#endif
