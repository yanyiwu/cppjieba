#ifndef CPPJIEBA_KEYWORD_EXTRACTOR_H
#define CPPJIEBA_KEYWORD_EXTRACTOR_H

#include "MPSegment.hpp"
#include <cmath>

namespace CppJieba
{
    using namespace Limonp;

    struct KeyWordInfo
    {
        string word;
        uint freq;
        double idf;
    };

    inline ostream& operator << (ostream& os, const KeyWordInfo & keyword)
    {
        return os << keyword.word << "," << keyword.freq << "," << keyword.idf;
    }

    class KeywordExtractor
    {
        private:
            MPSegment _segment;
        private:
            unordered_map<string, const KeyWordInfo* > _wordIndex;
            vector<KeyWordInfo> _wordinfos;
            size_t _totalFreq;
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
                _totalFreq = 0;
                int tfreq;
                string line ;
                vector<string> buf;
                KeyWordInfo keywordInfo;
                for(uint lineno = 0; getline(ifs, line); lineno++)
                {
                    buf.clear();
                    if(line.empty())
                    {
                        LogError("line[%d] empty. skipped.", lineno);
                        continue;
                    }
                    if(!split(line, buf, " ") || buf.size() != 3)
                    {
                        LogError("line %d [%s] illegal. skipped.", lineno, line.c_str());
                        continue;
                    }
                    keywordInfo.word = buf[0];
                    tfreq= atoi(buf[1].c_str());
                    if(tfreq <= 0)
                    {
                        LogError("line %d [%s] illegal. skipped.", lineno, line.c_str());
                        continue;
                    }
                    keywordInfo.freq = tfreq;
                    _totalFreq += tfreq;
                    _wordinfos.push_back(keywordInfo);
                }

                // calculate idf & make index.
                for(uint i = 0; i < _wordinfos.size(); i++)
                {
                    if(_wordinfos[i].freq <= 0)
                    {
                        LogFatal("freq value is not positive.");
                        return false;
                    }
                    _wordinfos[i].idf = -log(_wordinfos[i].freq);
                    _wordIndex[_wordinfos[i].word] = &(_wordinfos[i]);
                }
                return _setInitFlag(_segment.init(dictPath));
            };
        public:
            bool extract(const string& str, vector<string>& keywords, uint topN) const
            {
                assert(_getInitFlag());

                vector<string> words;
                if(!_segment.cut(str, words))
                {
                    LogError("segment cut(%s) failed.", str.c_str());
                    return false;
                }

                unordered_map<string, double> wordmap;
                for(uint i = 0; i < words.size(); i ++)
                {
                    wordmap[ words[i] ] += 1.0;
                }

                for(unordered_map<string, double>::iterator itr = wordmap.begin(); itr != wordmap.end();)
                {
                    unordered_map<string, const KeyWordInfo*>::const_iterator cit = _wordIndex.find(itr->first);
                    if(cit != _wordIndex.end())
                    {
                        itr->second *= cit->second->idf;
                        itr ++;
                    }
                    else
                    {
                        itr = wordmap.erase(itr);
                    }
                }

                vector<pair<string, double> > topWords(min(topN, wordmap.size()));
                partial_sort_copy(wordmap.begin(), wordmap.end(), topWords.begin(), topWords.end(), _cmp);

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


