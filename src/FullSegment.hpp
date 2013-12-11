#ifndef CPPJIEBA_FULLSEGMENT_H
#define CPPJIEBA_FULLSEGMENT_H

#include <algorithm>
#include <set>
#include <cassert>
#include "Limonp/logger.hpp"
#include "Trie.hpp"
#include "ISegment.hpp"
#include "SegmentBase.hpp"
#include "TransCode.hpp"

namespace CppJieba
{
    class FullSegment: public SegmentBase
    {
    private:
        Trie _trie;
        const string _dictPath;

    public:
        FullSegment(const char* dictPath): _dictPath(dictPath){};
        virtual ~FullSegment(){dispose();};
    public:
        bool init()
        {
#ifndef NO_CODING_LOG
            if(_getInitFlag())
            {
                LogError("already inited before now.");
                return false;
            }
#endif
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
        bool dispose()
        {
#ifndef NO_CODING_LOG
            if(!_getInitFlag())
            {
                return true;
            }
#endif
            _trie.dispose();
            _setInitFlag(false);
            return true;
        }

    public:
        using SegmentBase::cut;

    public:
        bool cut(Unicode::const_iterator begin, Unicode::const_iterator end, vector<Unicode>& res) const
        {
            assert(_getInitFlag());
#ifndef NO_CODING_LOG
            //if (!_getInitFlag())
            //{
            //    LogError("not inited.");
            //    return false;
            //}
            if (begin >= end)
            {
                LogError("begin >= end");
                return false;
            }
#endif
            //resut of searching in trie tree
            vector<pair<uint, const TrieNodeInfo*> > tRes;

            //max index of res's words
            int maxIdx = 0;

            // always equals to (uItr - begin)
            int uIdx = 0;

            //tmp variables
            int wordLen = 0;
            for (Unicode::const_iterator uItr = begin; uItr != end; uItr++)
            {
                //find word start from uItr
                if (_trie.find(uItr, end, tRes))
                {
                    for (vector<pair<uint, const TrieNodeInfo*> >::const_iterator itr = tRes.begin(); itr != tRes.end(); itr++)
                    {
                        wordLen = itr->second->word.size();
                        if (wordLen >= 2 || tRes.size() == 1 && maxIdx <= uIdx)
                        {
                            res.push_back(itr->second->word);
                        }
                        maxIdx = uIdx+wordLen > maxIdx ? uIdx+wordLen : maxIdx;
                    }
                    tRes.clear();
                }
                else // not found word start from uItr
                {
                    if (maxIdx <= uIdx) // never exist in prev results
                    {
                        //put itr itself in res
                        res.push_back(Unicode(1, *uItr));

                        //mark it exits
                        ++maxIdx;
                    }
                }
                ++uIdx;
            }

            return true;
        }

        bool cut(Unicode::const_iterator begin, Unicode::const_iterator end, vector<string>& res) const
        {
#ifndef NO_CODING_LOG
            if (!_getInitFlag())
            {
                LogError("not inited.");
                return false;
            }
            if (begin > end)
            {
                LogError("begin > end");
                return false;
            }
#endif
            vector<Unicode> uRes;
            if (!cut(begin, end, uRes))
            {
                LogError("get unicode cut result error.");
                return false;
            }
            string tmp;

            for (vector<Unicode>::const_iterator uItr = uRes.begin(); uItr != uRes.end(); uItr++)
            {
                if (TransCode::encode(*uItr, tmp))
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
    };
}

#endif
