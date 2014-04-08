#include <iostream>
#include <ctime>
#include <fstream>
#include "../src/Limonp/ArgvContext.hpp"
#include "../src/Limonp/io_functs.hpp"
#include "../src/MPSegment.hpp"
#include "../src/HMMSegment.hpp"
#include "../src/MixSegment.hpp"

using namespace CppJieba;

void cut(const ISegment * seg, const char * const filePath, size_t times = 30)
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
        printf("process [%3.0lf %%]\r", 100.0*(i+1)/times);
        fflush(stdout);
        res.clear();
        seg->cut(doc, res);
    }
    printf("\n");
}

int main(int argc, char ** argv)
{
    MixSegment seg("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8");
    if(!seg)
    {
        cout<<"seg init failed."<<endl;
        return EXIT_FAILURE;
    }
    long beginTime = clock();
    cut(&seg, "../test/testdata/weicheng.utf8");
    long endTime = clock();
    printf("[%.3lf seconds]time consumed.\n", double(endTime - beginTime)/CLOCKS_PER_SEC);
    return EXIT_SUCCESS;
}
