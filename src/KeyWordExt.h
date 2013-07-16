#ifndef CPPJIEBA_KEYWORDEXT_H
#define CPPJIEBA_KEYWORDEXT_H

#include "Segment.h"

namespace CppJieba
{

	struct WordInfo
	{
		string word;
		double weight;
		double idf;
		WordInfo()
		{
			word = "";
			weight = 0.0;
			idf = 0.0;
		}
		string getInfoStr() const
		{
			return string_format("{word:%s, weight:%lf, idf:%lf}", word.c_str(), weight, idf);
		}
	};

	inline string joinWordInfos(const vector<WordInfo>& vec)
	{
		vector<string> tmp;
		for(uint i = 0; i < vec.size(); i++)
		{
			tmp.push_back(vec[i].getInfoStr());
		};
		return joinStr(tmp, ",");
	}

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
			static bool _wordInfoCompare(const WordInfo& a, const WordInfo& b);
		private:
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
