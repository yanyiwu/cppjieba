#include "SegmentBase.h"

namespace CppJieba
{
    SegmentBase::SegmentBase()
    {
        _setInitFlag(false);
    }
    
    SegmentBase::~SegmentBase()
    {
    }

}


#ifdef UT
using namespace CppJieba;

int main()
{
    return 0;
}

#endif
