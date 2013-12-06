#include "../src/ChineseFilter.hpp"

using namespace CppJieba;

int main(int argc, char** argv)
{
    //ChineseFilter chFilter;
    ifstream ifs(argv[1]);
    string line;
    string s;
    while(getline(ifs, line))
    {
        const char * str = line.c_str();
        uint size = line.size();
        uint offset = 0;
        while(offset < size)
        {
            uint len;
            const char* t =  str+offset;
        int ret = filterAscii(t, size, len);
        s.assign(t, len);
        cout<<s<<","<<ret<<","<<len<<endl;
        offset += len;
        }
        
        
        //chFilter.feed(line);
        //for(ChineseFilter::iterator it = chFilter.begin(); it != chFilter.end(); it++)
        //{
        //    //cout<<__FILE__<<__LINE__<<endl;
        //    string tmp;
        //    TransCode::encode(it.begin, it.end, tmp);
        //    cout<<tmp<<endl;
        //}
    }
    return 0;
}

