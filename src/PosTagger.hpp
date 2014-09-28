#ifndef CPPJIEBA_POS_TAGGING_H
#define CPPJIEBA_POS_TAGGING_H

#include "MixSegment.hpp"
#include "Limonp/StringUtil.hpp"
#include "DictTrie.hpp"

namespace CppJieba
{
    using namespace Limonp;

    class PosTagger
    {
        private:
            MixSegment _segment;
            DictTrie _dictTrie;

        public:
            PosTagger(){};
            PosTagger(
                const string& dictPath, 
                const string& hmmFilePath
            )
            {
                LIMONP_CHECK(init(dictPath, hmmFilePath));
            };
            ~PosTagger(){};
        public:
            bool init(const string& dictPath, const string& hmmFilePath)
            {
                LIMONP_CHECK(_dictTrie.init(dictPath));
                LIMONP_CHECK(_segment.init(dictPath, hmmFilePath));
                return true;
            };

            bool tag(const string& src, vector<pair<string, string> >& res)
            {
                vector<string> cutRes;
                if (!_segment.cut(src, cutRes))
                {
                    LogError("_mixSegment cut failed");
                    return false;
                }

                const DictUnit *tmp = NULL;
                Unicode unico;
                for (vector<string>::iterator itr = cutRes.begin(); itr != cutRes.end(); ++itr)
                {
                    if (!TransCode::decode(*itr, unico))
                    {
                        LogError("decode failed.");
                        return false;
                    }
                    tmp = _dictTrie.find(unico.begin(), unico.end());
                    res.push_back(make_pair(*itr, tmp == NULL ? "x" : tmp->tag));
                }
                tmp = NULL;
                return !res.empty();
            }
    };
}

#endif
