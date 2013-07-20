/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#ifndef CPPJIEBA_KEYWORDEXT_H
#define CPPJIEBA_KEYWORDEXT_H

#include "Segment.h"

namespace CppJieba
{

	struct WordInfo
	{
		string word;
		size_t wLen;
		double weight;
		double idf;
		WordInfo()
		{
			word = "";
			wLen = 0;
			weight = 0.0;
			idf = 0.0;
		}
		string getInfoStr() const
		{
			return string_format("{word:%s,wLen:%d weight:%lf, idf:%lf}", word.c_str(), wLen, weight, idf);
		}
	};

	inline string joinWordInfos(const vector<WordInfo>& vec)
	{
		vector<string> tmp;
		for(uint i = 0; i < vec.size(); i++)
		{
			tmp.push_back(vec[i].getInfoStr());
		}
		return joinStr(tmp, ",");
	}

	class KeyWordExt
	{
		private:
			Segment _segment;
			vector<string> _priorSubWords;
			set<string> _stopWords;
		public:
			KeyWordExt();
			~KeyWordExt();
			bool init(const char * const filePath);

			//load stopwords
			bool loadStopWords(const char * const filePath);

			//load prior words' prefix
			bool loadPriorSubWords( const char * const filePath);

			bool destroy();

		public:
			bool extract(const string& utf8Str, vector<string>& keywords, uint topN);
		private:
			static bool _wordInfoCompare(const WordInfo& a, const WordInfo& b);
		private:
			bool _extractTopN(const vector<string>& words, vector<string>& keywords, uint topN);
		private:
			//sort by word len - idf
			bool _sortWLIDF(vector<WordInfo>& wordInfos);
		private:
			bool _filter(vector<string>& utf8Strs);
			bool _filterDuplicate(vector<string>& utf8Strs);
			bool _filterSingleWord(vector<string>& utf8Strs);
			bool _filterSubstr(vector<string>& utf8Strs);
			bool _filterStopWords(vector<string>& utf8Strs);
		private:
			bool _prioritizeSubWords(vector<WordInfo>& wordInfos);
			bool _isContainSubWords(const string& word);

	};

}

#endif
