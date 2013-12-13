#include <iostream>
#include <fstream>
#include "../src/Limonp/ArgvContext.hpp"
#include "../src/MPSegment.hpp"
#include "../src/HMMSegment.hpp"
#include "../src/MixSegment.hpp"

using namespace CppJieba;

void cut(const ISegment * seg, const char * const filePath)
{
    ifstream ifile(filePath);
    vector<string> res;
    string line;
    while(getline(ifile, line))
    {
        if(!line.empty())
        {
            res.clear();
            seg->cut(line, res);
            cout<<join(res.begin(), res.end(),"/")<<endl;
        }
    }
}

int main(int argc, char ** argv)
{
    //demo
    {
        HMMSegment seg("../dicts/hmm_model.utf8");
        if(!seg.init())
        {
            cout<<"seg init failed."<<endl;
            return EXIT_FAILURE;
        }
        cut(&seg, "testlines.utf8");
        seg.dispose();
    }
    {
        MixSegment seg("../dicts/jieba.dict.utf8", "../dicts/hmm_model.utf8");
        if(!seg.init())
        {
            cout<<"seg init failed."<<endl;
            return EXIT_FAILURE;
        }
        cut(&seg, "testlines.utf8");
        seg.dispose();
    }
    {
        MPSegment seg("../dicts/jieba.dict.utf8");
        if(!seg.init())
        {
            cout<<"seg init failed."<<endl;
            return false;
        }
        cut(&seg, "testlines.utf8");
        seg.dispose();
    }
    return EXIT_SUCCESS;
}
