#ifndef CPPJIEBA_KEYWORD_EXTRACTOR_H
#define CPPJIEBA_KEYWORD_EXTRACTOR_H

#include "MPSegment.hpp"

namespace CppJieba
{
    using namespace Limonp;

    struct KeyWordInfo
    {
        
        uint freq;
        double weight;
    };

    class KeywordExtractor//: public MPSegment
    {
        private:
            MPSegment _segment;
        private:
            unordered_map<string, uint> _wordIndex;
            vector<KeyWordInfo> _words;
        protected:
            bool _isInited;
            bool _getInitFlag()const{return _isInited;};
            bool _setInitFlag(bool flag){return _isInited = flag;};
        public:
            operator bool(){return _getInitFlag();};
        public:
            KeywordExtractor(){_setInitFlag(false);};
            explicit KeywordExtractor(const string& dictPath){_setInitFlag(init(dictPath));};
            ~KeywordExtractor(){};
        public:
            bool init(const string& dictPath)
            {
                ifstream ifs(dictPath.c_str());
                if(!ifs)
                {
                    LogError("open %s failed.", dictPath.c_str());
                    return false;
                }
                string line ;
                vector<string> buf;
                for(uint lineno = 0; getline(ifs, line); lineno++)
                {
                    buf.clear();
                }
                return _setInitFlag(_segment.init(dictPath));
            };
        public:
            bool extract(const string& str, vector<string>& keywords, uint topN)
            {
                assert(_getInitFlag());

                vector<string> words;
                if(!_segment.cut(str, words))
                {
                    LogError("segment cut(%s) failed.", str.c_str());
                    return false;
                }

                unordered_map<string, uint> wordcnt;
                for(uint i = 0; i < words.size(); i ++)
                {
                    wordcnt[ words[i] ] ++;
                }

                vector<pair<string, uint> > topWords(topN);
                partial_sort_copy(wordcnt.begin(), wordcnt.end(), topWords.begin(), topWords.end(), _cmp);

                keywords.clear();
                for(uint i = 0; i < topWords.size(); i++)
                {
                    keywords.push_back(topWords[i].first);
                }
                return true;
            }
        private:
            static bool _cmp(const pair<string, uint>& lhs, const pair<string, uint>& rhs)
            {
                return lhs.second > rhs.second;
            }
            
    };
}

#endif


