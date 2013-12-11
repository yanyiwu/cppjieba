#ifndef CPPJIEBA_QUERYSEGMENT_H
#define CPPJIEBA_QUERYSEGMENT_H

#include <algorithm>
#include <set>
#include <cassert>
#include "Limonp/logger.hpp"
#include "Trie.hpp"
#include "ISegment.hpp"
#include "SegmentBase.hpp"
#include "HMMSegment.hpp"
#include "FullSegment.hpp"
#include "TransCode.hpp"

namespace CppJieba
{
    class QuerySegment: public SegmentBase
    {
    private:
        HMMSegment _hmmSeg;
        FullSegment _fullSeg;
        int _maxWordLen;

    public:
        QuerySegment(const char* fullSegDict, const char* hmmSegDict, int maxWordLen): _hmmSeg(hmmSegDict), _fullSeg(fullSegDict), _maxWordLen(maxWordLen){};
        virtual ~QuerySegment(){dispose();};
    public:
        bool init()
        {
#ifndef NO_CODING_LOG
            if (_getInitFlag())
            {
                LogError("inited.");
            }
#endif
            if (!_hmmSeg.init())
            {
                LogError("_hmmSeg init");
                return false;
            }
            if (!_fullSeg.init())
            {
                LogError("_fullSeg init");
                return false;
            }
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
            _fullSeg.dispose();
            _hmmSeg.dispose();
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
            //use hmm cut first
            vector<Unicode> hmmRes;
            if (!_hmmSeg.cut(begin, end, hmmRes))
            {
                LogError("_hmmSeg cut failed.");
                return false;
            }

            vector<Unicode> fullRes;
            for (vector<Unicode>::const_iterator hmmResItr = hmmRes.begin(); hmmResItr != hmmRes.end(); hmmResItr++)
            {
                
                // if it's too long, cut with _fullSeg, put fullRes in res
                if (hmmResItr->size() > _maxWordLen)
                {
                    if (_fullSeg.cut(hmmResItr->begin(), hmmResItr->end(), fullRes))
                    {
                       for (vector<Unicode>::const_iterator fullResItr = fullRes.begin(); fullResItr != fullRes.end(); fullResItr++)
                       {
                           res.push_back(*fullResItr);
                       }
                    }
                }
                else // just use the hmm result
                {
                    res.push_back(*hmmResItr);
                }
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
