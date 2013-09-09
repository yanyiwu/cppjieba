#include "MixSegment.h"

namespace CppJieba
{
	MixSegment::MixSegment()
	{
	}
	
	MixSegment::~MixSegment()
	{
	}

	bool MixSegment::init(const char* const mpSegDict, const char* const hmmSegDict)
	{
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
		return true;
	}
	
	bool MixSegment::dispose()
	{
        _mpSeg.dispose();
        _hmmSeg.dispose();
		return true;
	}

    bool MixSegment::cut(const string& str, vector<string>& res)
    {
        vector<TrieNodeInfo> infos;
        if(!_mpSeg.cut(str, infos))
        {
            LogError("_mpSeg cutDAG failed.");
            return false;
        }
        for(uint i= 0; i < infos.size(); i++)
        {
            
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
