/************************************
 * file enc : ASCII
 * author   : wuyanyi09@gmail.com
 ************************************/
#ifndef CPPJIEBA_KEYWORDEXT_H
#define CPPJIEBA_KEYWORDEXT_H

#include <logger.hpp>
#include "MPSegment.h"
#include "structs.h"

namespace CppJieba
{

    class KeyWordExt
    {
        private:
            MPSegment _segment;
            //vector<string> _priorSubWords;
            set<Unicode> _stopWords;
        public:
            KeyWordExt();
            ~KeyWordExt();
            bool init(const char* const segDictFile);
            bool dispose();
            bool loadStopWords(const char * const filePath);
        private:
            //bool _loadPriorSubWords(const char * const filePath);


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
            inline bool _isSubIn(const vector<Unicode>& words, const Unicode& word)const
            {

                for(uint j = 0; j < words.size(); j++)
                {
                    if(word != words[j] && words[j].end() != search(words[j].begin(), words[j].end(), word.begin(), word.end()))
                    {
                        return true;
                    }
                }
                return false;
            }
            //bool _prioritizeSubWords(vector<KeyWordInfo>& wordInfos);
            //bool _isContainSubWords(const string& word);

    };

}

#endif
