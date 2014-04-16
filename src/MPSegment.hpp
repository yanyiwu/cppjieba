/************************************
 * file enc : ASCII
 * author   : wuyanyi09@gmail.com
 ************************************/
#ifndef CPPJIEBA_MPSEGMENT_H
#define CPPJIEBA_MPSEGMENT_H

#include <algorithm>
#include <set>
#include <cassert>
#include "Limonp/logger.hpp"
#include "DictTrie.hpp"
#include "DictTrie.hpp"
#include "ISegment.hpp"
#include "SegmentBase.hpp"

namespace CppJieba
{

    struct SegmentChar 
    {
        uint16_t uniCh;
        DagType dag;
        const DictUnit * pInfo;
        double weight;
        size_t nextPos;
        SegmentChar():uniCh(0), pInfo(NULL), weight(0.0), nextPos(0)
        {}
    };

    class MPSegment: public SegmentBase
    {
        protected:
            DictTrie _dictTrie;

        public:
            MPSegment(){_setInitFlag(false);};
            explicit MPSegment(const string& dictPath)
            {
                _setInitFlag(init(dictPath));
            };
            virtual ~MPSegment(){};
        public:
            bool init(const string& dictPath)
            {
                if(_getInitFlag())
                {
                    LogError("already inited before now.");
                    return false;
                }
                _dictTrie.init(dictPath);
                assert(_dictTrie);
                LogInfo("MPSegment init(%s) ok", dictPath.c_str());
                return _setInitFlag(true);
            }
        public:
            using SegmentBase::cut;
            virtual bool cut(Unicode::const_iterator begin, Unicode::const_iterator end, vector<string>& res)const
            {
                assert(_getInitFlag());
                if(begin == end)
                {
                    return false;
                }

                vector<Unicode> words;
                if(!cut(begin, end, words))
                {
                    return false;
                }
                string word;
                for(size_t i = 0; i < words.size(); i++)
                {
                    if(TransCode::encode(words[i], word))
                    {
                        res.push_back(word);
                    }
                    else
                    {
                        LogError("encode failed.");
                    }
                }
                return true;
            }

            bool cut(Unicode::const_iterator begin , Unicode::const_iterator end, vector<Unicode>& res) const
            {
                if(!_getInitFlag())
                {
                    LogError("not inited.");
                    return false;
                }
                vector<SegmentChar> SegmentChars;
                //calc DAG
                if(!_calcDAG(begin, end, SegmentChars))
                {
                    LogError("_calcDAG failed.");
                    return false;
                }

                if(!_calcDP(SegmentChars))
                {
                    LogError("_calcDP failed.");
                    return false;
                }

                if(!_cut(SegmentChars, res))
                {
                    LogError("_cut failed.");
                    return false;
                }

                return true;
            }

        private:
            bool _calcDAG(Unicode::const_iterator begin, Unicode::const_iterator end, vector<SegmentChar>& SegmentChars) const
            {
                SegmentChar schar;
                size_t offset;
                for(Unicode::const_iterator it = begin; it != end; it++)
                {
                    schar.uniCh = *it;
                    offset = it - begin;
                    schar.dag.clear();
                    _dictTrie.find(it, end, schar.dag, offset);
                    if(!isIn(schar.dag, offset))
                    {
                        schar.dag[offset] = NULL;
                    }
                    SegmentChars.push_back(schar);
                }
                return true;
            }
            bool _calcDP(vector<SegmentChar>& SegmentChars)const
            {
                if(SegmentChars.empty())
                {
                    LogError("SegmentChars empty");
                    return false;
                }

                size_t nextPos;
                const DictUnit* p;
                double val;

                for(int i = SegmentChars.size() - 1; i >= 0; i--)
                {
                    SegmentChars[i].pInfo = NULL;
                    SegmentChars[i].weight = MIN_DOUBLE;
                    for(DagType::const_iterator it = SegmentChars[i].dag.begin(); it != SegmentChars[i].dag.end(); it++)
                    {
                        nextPos = it->first;
                        p = it->second;
                        val = 0.0;
                        if(nextPos + 1 < SegmentChars.size())
                        {
                            val += SegmentChars[nextPos + 1].weight;
                        }

                        if(p)
                        {
                            val += p->logFreq; 
                        }
                        else
                        {
                            val += _dictTrie.getMinLogFreq();
                        }
                        if(val > SegmentChars[i].weight)
                        {
                            SegmentChars[i].pInfo = p;
                            SegmentChars[i].weight = val;
                        }
                    }
                }
                return true;

            }
            bool _cut(vector<SegmentChar>& SegmentChars, vector<Unicode>& res)const
            {
                size_t i = 0;
                while(i < SegmentChars.size())
                {
                    const DictUnit* p = SegmentChars[i].pInfo;
                    if(p)
                    {
                        res.push_back(p->word);
                        i += p->word.size();
                    }
                    else//single chinese word
                    {
                        res.push_back(Unicode(1, SegmentChars[i].uniCh));
                        i++;
                    }
                }
                return true;
            }


    };
}

#endif
