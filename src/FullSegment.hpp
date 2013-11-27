#ifndef CPPJIEBA_FULLSEGMENT_H
#define CPPJIEBAi_FULLSEGMENT_H

#include <algorithm>
#include <set>
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

    public:
        FullSegment(){};
        virtual ~FullSegment(){dispose();};
    public:
        bool init(const char* const filePath)
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
            LogInfo("_trie.loadDict(%s) start...", filePath);
            if(!_trie.loadDict(filePath))
            {
                LogError("_trie.loadDict faield.");
                return false;
            }
            LogInfo("_trie.loadDict end.");
            return _setInitFlag(true);
        }
        bool dispose()
        {
            if(!_getInitFlag())
            {
                return true;
            }
            _trie.dispose();
            _setInitFlag(false);
            return true;
        }

        bool cut(const string& str, vector<string>& res) const 
        {
            return SegmentBase::cut(str, res);
        }

        bool cut(Unicode::const_iterator begin, Unicode::const_iterator end, vector<string>& res) const
        {
            //resut of searching in trie tree
            vector<pair<uint, const TrieNodeInfo*> > tRes;

            //max index of res's words
            int maxIdx = 0;

            // always equals to (uItr - begin)
            int uIdx = 0;

            //tmp variables
            int wordLen = 0;
            string tmp;
            for (Unicode::const_iterator uItr = begin; uItr != end; uItr++)
            {
                //find word start from itr
                if (_trie.find(uItr, end, tRes))
                {
                    for (vector<pair<uint, const TrieNodeInfo*> >::const_iterator itr = tRes.begin(); itr != tRes.end(); itr++)
                    {
                        wordLen = itr->second->word.size();
                        if (wordLen >= 2 || tRes.size() == 1 && maxIdx <= uIdx)
                        {
                            if (TransCode::encode(itr->second->word, tmp))
                                res.push_back(tmp);
                            else
                                LogError("encode failed.");
                            tmp.clear();
                        }
                        maxIdx = uIdx+wordLen > maxIdx ? uIdx+wordLen : maxIdx;
                    }
                    tRes.clear();
                }
                else // not found word start from itr
                {
                    if (maxIdx <= uIdx) // never exist in prev results
                    {
                        //put itr itself in res
                        Unicode uTmp(1, *uItr);
                        if (TransCode::encode(uTmp, tmp))
                        {
                            res.push_back(tmp);
                        }
                        tmp.clear();
                        ++maxIdx;
                    }
                }
                ++uIdx;
            }
        }
    };
}

#endif
