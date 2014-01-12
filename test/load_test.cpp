#include <iostream>
#include <fstream>
#include "../src/Limonp/ArgvContext.hpp"
#include "../src/MPSegment.hpp"
#include "../src/HMMSegment.hpp"
#include "../src/MixSegment.hpp"

using namespace CppJieba;

void cut(const ISegment * seg, const char * const filePath, size_t times = 10)
{
    ifstream ifile(filePath);
    if(!ifile)
    {
        LogFatal("open file[%s] failed.", filePath);
        return;
    }
    LogInfo("open file[%s].", filePath);
    vector<string> res;
    string doc;
    loadFile2Str(filePath, doc);
    for(uint i = 0; i < times; i ++)
    {
        LogInfo("times[%u]", i);
        res.clear();
        seg->cut(doc, res);
    }
}

int main(int argc, char ** argv)
{
    {
        MixSegment seg("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8");
        if(!seg)
        {
            cout<<"seg init failed."<<endl;
            return EXIT_FAILURE;
        }
        cut(&seg, "../test/testdata/weicheng.utf8");
    }
    return EXIT_SUCCESS;
}
