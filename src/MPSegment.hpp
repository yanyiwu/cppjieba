/************************************
 * file enc : ASCII
 * author   : wuyanyi09@gmail.com
 ************************************/
#ifndef CPPJIEBA_MPSEGMENT_H
#define CPPJIEBA_MPSEGMENT_H

#include <algorithm>
#include <set>
#include "Limonp/logger.hpp"
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

        SegmentChar(uint16_t uni):uniCh(uni), pInfo(NULL), weight(0.0)
        {
        }
    };
    typedef vector<SegmentChar> SegmentContext;

    class MPSegment: public SegmentBase
    {
        private:
            Trie _trie;
        private:
            const string _dictPath;

        public:
            MPSegment(const char * const dictPath): _dictPath(dictPath){};
            virtual ~MPSegment(){dispose();};
        public:
            virtual bool init()
            {
                if(_getInitFlag())
                {
                    LogError("already inited before now.");
                    return false;
                }
                if(!_trie.init())
                {
                    LogError("_trie.init failed.");
                    return false;
                }
                LogInfo("_trie.loadDict(%s) start...", _dictPath.c_str());
                if(!_trie.loadDict(_dictPath.c_str()))
                {
                    LogError("_trie.loadDict faield.");
                    return false;
                }
                LogInfo("_trie.loadDict end.");
                return _setInitFlag(true);
            }
            virtual bool dispose()
            {
                if(!_getInitFlag())
                {
                    return true;
                }
                _trie.dispose();
                _setInitFlag(false);
                return true;
            }
        public:
            virtual bool cut(Unicode::const_iterator begin, Unicode::const_iterator end, vector<string>& res)const
            {
                if(!_getInitFlag())
                {
                    LogError("not inited.");
                    return false;
                }
                vector<TrieNodeInfo> segWordInfos;
                if(!cut(begin, end, segWordInfos))
                {
                    return false;
                }
                string tmp;
                for(uint i = 0; i < segWordInfos.size(); i++)
                {
                    if(TransCode::encode(segWordInfos[i].word, tmp))
                    {
                        res.push_back(tmp);
                    }
                    else
                    {
                        LogError("encode failed.");
                    }
                }
                return true;
            }
            bool cut(const string& str, vector<TrieNodeInfo>& segWordInfos)const
            {
                if(!_getInitFlag())
                {
                    LogError("not inited.");
                    return false;
                }
                if(str.empty())
                {
                    return false;
                }
                Unicode sentence;

                if(!TransCode::decode(str, sentence))
                {
                    LogError("TransCode::decode failed.");
                    return false;
                }
                return cut(sentence.begin(), sentence.end(), segWordInfos);

            }
            bool cut(Unicode::const_iterator begin , Unicode::const_iterator end, vector<TrieNodeInfo>& segWordInfos)const
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

                if(!_cut(segContext, segWordInfos))
                {
                    LogError("_cut failed.");
                    return false;
                }

                return true;
            }

        private:
            bool _calcDAG(Unicode::const_iterator begin, Unicode::const_iterator end, SegmentContext& segContext) const
            {
                if(begin >= end)
                {
                    LogError("begin >= end.");
                    return false;
                }

                vector<pair<uint, const TrieNodeInfo*> > vp;
                for(Unicode::const_iterator it = begin; it != end; it++)
                {
                    segContext.push_back(SegmentChar(*it));
                    SegmentChar& back = segContext.back();
                    int i = it - begin;
                    vp.clear();
                    if(_trie.find(it, end, vp))
                    {
                        for(uint j = 0; j < vp.size(); j++)
                        {
                            uint nextp = vp[j].first + i;
                            back.dag[nextp] = vp[j].second; 
                        }
                    }
                    if(back.dag.end() == back.dag.find(i))
                    {
                        back.dag[i] = NULL;
                    }
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

                for(int i = segContext.size() - 1; i >= 0; i--)
                {
                    segContext[i].pInfo = NULL;
                    segContext[i].weight = MIN_DOUBLE;
                    for(DagType::const_iterator it = segContext[i].dag.begin(); it != segContext[i].dag.end(); it++)
                    {
                        uint nextPos = it->first;
                        const TrieNodeInfo* p = it->second;
                        double val = 0.0;
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
            bool _cut(SegmentContext& segContext, vector<TrieNodeInfo>& res)const
            {
                uint i = 0;
                while(i < segContext.size())
                {
                    const TrieNodeInfo* p = segContext[i].pInfo;
                    if(p)
                    {
                        res.push_back(*p);
                        i += p->word.size();
                    }
                    else//single chinese word
                    {
                        TrieNodeInfo nodeInfo;
                        nodeInfo.word.push_back(segContext[i].uniCh);
                        nodeInfo.freq = 0;
                        nodeInfo.logFreq = _trie.getMinLogFreq();
                        res.push_back(nodeInfo);
                        i++;
                    }
                }
                return true;
            }


    };
}

#endif
