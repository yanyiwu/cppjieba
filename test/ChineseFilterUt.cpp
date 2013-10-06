#include <ChineseFilter.h>

#ifdef UT
using namespace CppJieba;


int main(int argc, char** argv)
{
    ChineseFilter chFilter;
    ifstream ifs("../demo/testlines.utf8");
    string line;
    while(getline(ifs, line))
    {
        chFilter.feed(line);
        for(ChineseFilter::iterator it = chFilter.begin(); it != chFilter.end(); it++)
        {
            //cout<<__FILE__<<__LINE__<<endl;
            string tmp;
            TransCode::encode(it.begin, it.end, tmp);
            cout<<tmp<<endl;
        }
    }
    return 0;
}

#endif
