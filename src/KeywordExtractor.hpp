#ifndef CPPJIEBA_KEYWORD_EXTRACTOR_H
#define CPPJIEBA_KEYWORD_EXTRACTOR_H

#include "MixSegment.hpp"
#include <cmath>
#include <unordered_set>
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

namespace CppJieba
{
    using namespace Limonp;

    /*utf8*/
    const char * const BLACK_LIST[] = {"我们", "他们"};

    class KeywordExtractor: public InitOnOff
    {
        private:
            MixSegment _segment;
        private:
            unordered_map<string, double> _idfMap;
            double _idfAverage;

            unordered_set<string> _blackSet;
        public:
            KeywordExtractor(){_setInitFlag(false);};
            explicit KeywordExtractor(const string& dictPath, const string& hmmFilePath, const string& idfPath)
            {
                _setInitFlag(init(dictPath, hmmFilePath, idfPath));
            };
            ~KeywordExtractor(){};
        public:
            bool init(const string& dictPath, const string& hmmFilePath, const string& idfPath)
            {
                ifstream ifs(idfPath.c_str());
                if(!ifs)
                {
                    LogError("open %s failed.", idfPath.c_str());
                    return false;
                }
                string line ;
                vector<string> buf;
                double idf = 0.0;
                double idfSum = 0.0;
                size_t lineno = 0;
                for(;getline(ifs, line); lineno++)
                {
                    buf.clear();
                    if(line.empty())
                    {
                        LogError("line[%d] empty. skipped.", lineno);
                        continue;
                    }
                    if(!split(line, buf, " ") || buf.size() != 2)
                    {
                        LogError("line %d [%s] illegal. skipped.", lineno, line.c_str());
                        continue;
                    }
                    idf = atof(buf[1].c_str());
                    _idfMap[buf[0]] = idf;
                    idfSum += idf;

                } 

                std::copy(
                            BLACK_LIST, BLACK_LIST + sizeof(BLACK_LIST)/sizeof(BLACK_LIST[0]), 
                            std::inserter(_blackSet, _blackSet.begin()));
                
                assert(lineno);
                _idfAverage = idfSum / lineno;

                assert(_idfAverage > 0.0);
                
                return _setInitFlag(_segment.init(dictPath, hmmFilePath));
            };
        public:

            bool extract(const string& str, vector<string>& keywords, uint topN) const
            {
                assert(_getInitFlag());
                vector<pair<string, double> > topWords;
                if(!extract(str, topWords, topN))
                {
                    return false;
                }
                for(uint i = 0; i < topWords.size(); i++)
                {
                    keywords.push_back(topWords[i].first);
                }
                return true;
            }

            bool extract(const string& str, vector<pair<string, double> >& keywords, uint topN) const
            {
                vector<string> words;
                if(!_segment.cut(str, words))
                {
                    LogError("segment cut(%s) failed.", str.c_str());
                    return false;
                }

                // filtering single word.
                for(vector<string>::iterator iter = words.begin(); iter != words.end(); )
                {
                    if(_isSingleWord(*iter))
                    {
                        iter = words.erase(iter);
                    }
                    else
                    {
                        iter++;
                    }
                }

                unordered_map<string, double> wordmap;
                for(uint i = 0; i < words.size(); i ++)
                {
                    wordmap[ words[i] ] += 1.0;
                }

                for(unordered_map<string, double>::iterator itr = wordmap.begin(); itr != wordmap.end(); )
                {
                    if(_blackSet.end() != _blackSet.find(itr->first))
                    {
                        itr = wordmap.erase(itr);
                        continue;
                    }

                    unordered_map<string, double>::const_iterator cit = _idfMap.find(itr->first);
                    if(cit != _idfMap.end())
                    {
                        itr->second *= cit->second;
                    }
                    else
                    {
                        itr->second *= _idfAverage;
                    }
                    itr ++;
                }

                keywords.clear();
                std::copy(wordmap.begin(), wordmap.end(), std::inserter(keywords, keywords.begin()));
                topN = MIN(topN, keywords.size());
                partial_sort(keywords.begin(), keywords.begin() + topN, keywords.end(), _cmp);
                keywords.resize(topN);
                return true;
            }
        private:
            bool _isSingleWord(const string& str) const
            {
                Unicode unicode;
                TransCode::decode(str, unicode);
                if(unicode.size() == 1)
                  return true;
                return false;
            }

        private:
            static bool _cmp(const pair<string, double>& lhs, const pair<string, double>& rhs)
            {
                return lhs.second > rhs.second;
            }
            
    };
}

#endif


