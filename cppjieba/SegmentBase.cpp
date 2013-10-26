#include "SegmentBase.hpp"

namespace CppJieba
{
    bool SegmentBase::cut(const string& str, vector<string>& res)const
    {
        if(!_getInitFlag())
        {
            LogError("not inited.");
            return false;
        }
        ChineseFilter filter;
        filter.feed(str);
        for(ChineseFilter::iterator it = filter.begin(); it != filter.end(); it++)
        {
            if(it.charType == CHWORD)
            {
                cut(it.begin, it.end, res);
            }
            else
            {
                string tmp;
                if(TransCode::encode(it.begin, it.end, tmp))
                {
                    res.push_back(tmp);
                }
            }
        }
        return true;
    }
}


#ifdef UT
using namespace CppJieba;

int main()
{
    return 0;
}

#endif
