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
        return _get(_unico.begin());
    }

    ChineseFilter::iterator ChineseFilter::end()
    {
        return iterator(this, _unico.end(), _unico.end(), false);
    }
    
    ChineseFilter::iterator ChineseFilter::_get(UniConIter iter)
    {
        UniConIter begin = iter;
        const UniConIter& end = _unico.end();
        if(iter == end)
        {
            return this->end();
        }
        bool isChWord = _isChWord(*iter);
        iter ++;
        while(iter != end && _isChWord(*iter))
        {
            iter++;
        }
        return ChFilterIterator(this, begin, iter, isChWord);
    }
}

#ifdef UT
using namespace CppJieba;


int main()
{
    ChineseFilter chFilter;
    ifstream ifs("testlines.utf8");
    string line;
    while(getline(ifs, line))
    {
        chFilter.feed(line);
    }
    for(chFilter::iterator it = chFilter.begin(); it != chFilter.end(); it++)
    {
        cout<<
    }
    return 0;
}

#endif
