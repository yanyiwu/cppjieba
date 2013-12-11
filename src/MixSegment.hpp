#ifndef CPPJIEBA_MIXSEGMENT_H
#define CPPJIEBA_MIXSEGMENT_H

#include <cassert>
#include "MPSegment.hpp"
#include "HMMSegment.hpp"
#include "Limonp/str_functs.hpp"

namespace CppJieba
{
    class MixSegment: public SegmentBase
    {
        private:
            MPSegment _mpSeg;
            HMMSegment _hmmSeg;
        public:
            MixSegment(const char * const mpSegDict, const char * const hmmSegDict): _mpSeg(mpSegDict), _hmmSeg(hmmSegDict)
            {
            }
            virtual ~MixSegment()
            {
                dispose();
            }
        public:
            virtual bool init()
            {
                if(_getInitFlag())
                {
                    LogError("inited.");
                    return false;
                }
                if(!_mpSeg.init())
                {
                    LogError("_mpSeg init");
                    return false;
                }
                if(!_hmmSeg.init())
                {
                    LogError("_hmmSeg init");
                    return false;
                }
                return _setInitFlag(true);
            }
            virtual bool dispose()
            {
                if(!_getInitFlag())
                {
                    return true;
                }
                _mpSeg.dispose();
                _hmmSeg.dispose();
                _setInitFlag(false);
                return true;
            }
        public:
            using SegmentBase::cut;
        public:
            virtual bool cut(Unicode::const_iterator begin, Unicode::const_iterator end, vector<string>& res)const
            {
                //if(!_getInitFlag())
                //{
                //    LogError("not inited.");
                //    return false;
                //}
                assert(_getInitFlag());
                if(begin == end)
                {
                    return false;
                }
                vector<TrieNodeInfo> infos;
                if(!_mpSeg.cut(begin, end, infos))
                {
                    LogError("mpSeg cutDAG failed.");
                    return false;
                }
                Unicode unico;
                vector<Unicode> hmmRes;
                string tmp;
                for(uint i= 0; i < infos.size(); i++)
                {
                    TransCode::encode(infos[i].word,tmp);
                    if(1 == infos[i].word.size())
                    {
                        unico.push_back(infos[i].word[0]);
                    }
                    else
                    {
                        if(!unico.empty())
                        {
                            hmmRes.clear();
                            if(!_hmmSeg.cut(unico.begin(), unico.end(), hmmRes))
                            {
                                LogError("_hmmSeg cut failed.");
                                return false;
                            }
                            for(uint j = 0; j < hmmRes.size(); j++)
                            {
                                TransCode::encode(hmmRes[j], tmp);
                                res.push_back(tmp);
                            }
                        }
                        unico.clear();
                        TransCode::encode(infos[i].word, tmp);
                        res.push_back(tmp);
                    }
                }
                if(!unico.empty())
                {
                    hmmRes.clear();
                    if(!_hmmSeg.cut(unico.begin(), unico.end(), hmmRes))
                    {
                        LogError("_hmmSeg cut failed.");
                        return false;
                    }
                    for(uint j = 0; j < hmmRes.size(); j++)
                    {
                        TransCode::encode(hmmRes[j], tmp);
                        res.push_back(tmp);
                    }
                }

                return true;
            }
    };
}

#endif
