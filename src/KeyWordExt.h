/************************************
 * file enc : ASCII
 * author   : wuyanyi09@gmail.com
************************************/
#ifndef CPPJIEBA_KEYWORDEXT_H
#define CPPJIEBA_KEYWORDEXT_H

#include "Segment.h"
#include "structs.h"

namespace CppJieba
{

	class KeyWordExt
	{
		private:
			Segment _segment;
			vector<string> _priorSubWords;
			set<string> _stopWords;
		public:
			KeyWordExt();
			~KeyWordExt();
			bool init();

			bool loadSegDict(const char * const filePath);

			//load stopwords
			bool loadStopWords(const char * const filePath);

			//load prior words' prefix
			bool loadPriorSubWords(const char * const filePath);

			bool dispose();

		public:
			bool extract(const string& title, vector<string>& keywords, uint topN);
			bool extract(const vector<string>& words, vector<string>& keywords, uint topN);
		private:
			static bool _wordInfoCompare(const WordInfo& a, const WordInfo& b);
		private:
			bool _extractTopN(const vector<string>& words, vector<string>& keywords, uint topN);
		private:
			//sort by word len - idf
			bool _sortWLIDF(vector<WordInfo>& wordInfos);
		private:
			bool _filter(vector<string>& strs);
			bool _filterDuplicate(vector<string>& strs);
			bool _filterSingleWord(vector<string>& strs);
			bool _filterSubstr(vector<string>& strs);
			bool _filterStopWords(vector<string>& strs);
		private:
			bool _prioritizeSubWords(vector<WordInfo>& wordInfos);
			bool _isContainSubWords(const string& word);

	};

}

#endif
