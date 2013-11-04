#include <iostream>
#include <fstream>
#include <CppJieba/Limonp/ArgvContext.hpp>
#include <CppJieba/MPSegment.h>
#include <CppJieba/HMMSegment.h>
#include <CppJieba/MixSegment.h>

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
        HMMSegment seg;
        if(!seg.init("../dicts/hmm_model.utf8"))
        {
            cout<<"seg init failed."<<endl;
            return EXIT_FAILURE;
        }
        cut(&seg, "testlines.utf8");
        seg.dispose();
    }
    {
        MixSegment seg;
        if(!seg.init("../dicts/jieba.dict.utf8", "../dicts/hmm_model.utf8"))
        {
            cout<<"seg init failed."<<endl;
            return EXIT_FAILURE;
        }
        cut(&seg, "testlines.utf8");
        seg.dispose();
    }
    {
        MPSegment seg;
        if(!seg.init("../dicts/jieba.dict.utf8"))
        {
            cout<<"seg init failed."<<endl;
            return false;
        }
        cut(&seg, "testlines.utf8");
        seg.dispose();
    }
    return EXIT_SUCCESS;
}
