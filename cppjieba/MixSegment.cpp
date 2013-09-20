#include "MixSegment.h"

namespace CppJieba
{
    MixSegment::MixSegment()
    {
    }
    
    MixSegment::~MixSegment()
    {
        dispose();
    }

    bool MixSegment::init(const char* const mpSegDict, const char* const hmmSegDict)
    {
        if(_getInitFlag())
        {
            LogError("inited.");
            return false;
        }
        if(!_mpSeg.init(mpSegDict))
        {
            LogError("_mpSeg init");
            return false;
        }
        if(!_hmmSeg.init(hmmSegDict))
        {
            LogError("_hmmSeg init");
            return false;
        }
        return _setInitFlag(true);
    }
    
    bool MixSegment::dispose()
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

    bool MixSegment::cut(const string& str, vector<string>& res)const
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
        vector<TrieNodeInfo> infos;
        if(!_mpSeg.cut(str, infos))
        {
            LogError("mpSeg cutDAG [%s] failed.", str.c_str());
            return false;
        }
        res.clear();
        Unicode unico;
        vector<Unicode> hmmRes;
        string tmp;
        for(uint i= 0; i < infos.size(); i++)
        {
            if(1 == infos[i].word.size())
            {
                unico.push_back(infos[i].word[0]);
            }
            else
            {
                if(!unico.empty())
                {
                    if(!_hmmSeg.cut(unico, hmmRes))
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
            if(!_hmmSeg.cut(unico, hmmRes))
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
}

#ifdef MIXSEGMENT_UT
using namespace CppJieba;

int main()
{
    return 0;
}

#endif
