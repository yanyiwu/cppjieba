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
#include "Trie.hpp"
#include "Trie.hpp"
#include "ISegment.hpp"
#include "SegmentBase.hpp"

namespace CppJieba
{

    struct SegmentChar 
    {
        uint16_t uniCh;
        DagType dag;
        const TrieNodeInfo * pInfo;
        double weight;

        SegmentChar():uniCh(0), pInfo(NULL), weight(0.0)
        {}
    };
    typedef vector<SegmentChar> SegmentContext;

    class MPSegment: public SegmentBase
    {
        protected:
            Trie _trie;

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
                _trie.init(dictPath);
                assert(_trie);
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
                SegmentContext segContext;
                //calc DAG
                if(!_calcDAG(begin, end, segContext))
                {
                    LogError("_calcDAG failed.");
                    return false;
                }

                if(!_calcDP(segContext))
                {
                    LogError("_calcDP failed.");
                    return false;
                }

                if(!_cut(segContext, res))
                {
                    LogError("_cut failed.");
                    return false;
                }

                return true;
            }

        private:
            bool _calcDAG(Unicode::const_iterator begin, Unicode::const_iterator end, SegmentContext& segContext) const
            {
                SegmentChar schar;
                size_t offset;
                for(Unicode::const_iterator it = begin; it != end; it++)
                {
                    schar.uniCh = *it;
                    offset = it - begin;
                    schar.dag.clear();
                    _trie.find(it, end, schar.dag, offset);
                    if(!isIn(schar.dag, offset))
                    {
                        schar.dag[offset] = NULL;
                    }
                    segContext.push_back(schar);
                }
                return true;
            }
            bool _calcDP(SegmentContext& segContext)const
            {
                if(segContext.empty())
                {
                    LogError("segContext empty");
                    return false;
                }

                size_t nextPos;
                const TrieNodeInfo* p;
                double val;

                for(int i = segContext.size() - 1; i >= 0; i--)
                {
                    segContext[i].pInfo = NULL;
                    segContext[i].weight = MIN_DOUBLE;
                    for(DagType::const_iterator it = segContext[i].dag.begin(); it != segContext[i].dag.end(); it++)
                    {
                        nextPos = it->first;
                        p = it->second;
                        val = 0.0;
                        if(nextPos + 1 < segContext.size())
                        {
                            val += segContext[nextPos + 1].weight;
                        }

                        if(p)
                        {
                            val += p->logFreq; 
                        }
                        else
                        {
                            val += _trie.getMinLogFreq();
                        }
                        if(val > segContext[i].weight)
                        {
                            segContext[i].pInfo = p;
                            segContext[i].weight = val;
                        }
                    }
                }
                return true;

            }
            bool _cut(SegmentContext& segContext, vector<Unicode>& res)const
            {
                size_t i = 0;
                while(i < segContext.size())
                {
                    const TrieNodeInfo* p = segContext[i].pInfo;
                    if(p)
                    {
                        res.push_back(p->word);
                        i += p->word.size();
                    }
                    else//single chinese word
                    {
                        res.push_back(Unicode(1, segContext[i].uniCh));
                        i++;
                    }
                }
                return true;
            }


    };
}

#endif
