/************************************
 * file enc : ASCII
 * author   : wuyanyi09@gmail.com
************************************/
#ifndef CPPJIEBA_KEYWORDEXT_H
#define CPPJIEBA_KEYWORDEXT_H

#include "MPSegment.h"
#include "structs.h"

namespace CppJieba
{

	class KeyWordExt
	{
		private:
			MPSegment _segment;
			vector<string> _priorSubWords;
			set<string> _stopWords;
		public:
			KeyWordExt();
			~KeyWordExt();
			bool init(const char* const segDictFile, const char* const stopWordDictFile);
			bool dispose();

		private:
			bool _loadStopWords(const char * const filePath);
			bool _loadPriorSubWords(const char * const filePath);


		public:
			bool extract(const string& title, vector<KeyWordInfo>& keyWordInfos, uint topN);
			bool extract(const vector<string>& words, vector<KeyWordInfo>& keyWordInfos, uint topN);
		private:
			static bool _wordInfoCompare(const KeyWordInfo& a, const KeyWordInfo& b);
		private:
			bool _extract(vector<KeyWordInfo>& keyWordInfos, uint topN);
			bool _extTopN(vector<KeyWordInfo>& wordInfos, uint topN);
		private:
			//sort by word len - idf
			bool _sortWLIDF(vector<KeyWordInfo>& wordInfos);
		private:
			bool _filter(vector<KeyWordInfo>& );
			bool _filterDuplicate(vector<KeyWordInfo>& );
			bool _filterSingleWord(vector<KeyWordInfo>& );
			bool _filterSubstr(vector<KeyWordInfo>& );
			bool _filterStopWords(vector<KeyWordInfo>& );
		private:
			bool _prioritizeSubWords(vector<KeyWordInfo>& wordInfos);
			bool _isContainSubWords(const string& word);

	};

}

#endif
