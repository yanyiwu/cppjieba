#include "ChineseFilter.h"

namespace CppJieba
{
    ChineseFilter::ChineseFilter()
    {
    }
    
    ChineseFilter::~ChineseFilter()
    {
    }

    bool ChineseFilter::feed(const string& str)
    {
        TransCode::decode(str, _unico);
        return true;
    }

    ChineseFilter::iterator ChineseFilter::begin()
    {
        return iterator(&_unico);
    }

    ChineseFilter::iterator ChineseFilter::end()
    {
        return iterator(&_unico, _unico.end(), _unico.end(), ChFilterIterator::OTHERS);
    }
    
}

#ifdef UT
using namespace CppJieba;


int main()
{
    ChineseFilter chFilter;
    ifstream ifs("../demo/testlines.utf8");
    string line;
    while(getline(ifs, line))
    {
        chFilter.feed(line);
    }
    for(ChineseFilter::iterator it = chFilter.begin(); it != chFilter.end(); it++)
    {
        //cout<<__FILE__<<__LINE__<<endl;
        string tmp;
        TransCode::encode(it.begin, it.end, tmp);
        cout<<tmp<<endl;
    }
    return 0;
}

#endif
