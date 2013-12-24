#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include "Limonp/ArgvContext.hpp"
#include "MPSegment.hpp"
#include "HMMSegment.hpp"
#include "MixSegment.hpp"
#include "FullSegment.hpp"
#include "QuerySegment.hpp"

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
            cout << line << endl;
            res.clear();
            if(!seg->cut(line, res))
            {
                LogError("seg cut failed.");
            }
            else
            {
                print(join(res.begin(), res.end(), "/"));
            }
        }
    }
}

int main(int argc, char ** argv)
{
    if(argc < 2)
    {
        cout<<"usage: \n\t"<<argv[0]<<" [options] <filename>\n"
            <<"options:\n"
            <<"\t--algorithm\tSupported methods are [cutDAG, cutHMM, cutFull, cutQuery, cutMix] for now. \n\t\t\tIf not specified, the default is cutMix\n"
            <<"\t--dictpath\tsee example\n"
            <<"\t--modelpath\tsee example\n"
            <<"\t--maxlen\tspecify the granularity of cut used in cutQuery. \n\t\t\tIf not specified, the default is 3\n"
            <<"example:\n"
            <<"\t"<<argv[0]<<" ../test/testdata/testlines.utf8 --dictpath ../dict/jieba.dict.utf8 --algorithm cutDAG\n"
            <<"\t"<<argv[0]<<" ../test/testdata/testlines.utf8 --dictpath ../dict/jieba.dict.utf8 --algorithm cutFull\n"
            <<"\t"<<argv[0]<<" ../test/testdata/testlines.utf8 --modelpath ../dict/hmm_model.utf8 --algorithm cutHMM\n"
            <<"\t"<<argv[0]<<" ../test/testdata/testlines.utf8 --dictpath ../dict/jieba.dict.utf8 --modelpath ../dict/hmm_model.utf8 --algorithm cutMix\n"
            <<"\t"<<argv[0]<<" ../test/testdata/testlines.utf8 --dictpath ../dict/jieba.dict.utf8 --modelpath ../dict/hmm_model.utf8 --algorithm cutQuery --maxlen 3\n"
            <<endl;
        
        return EXIT_FAILURE;
    }
    ArgvContext arg(argc, argv);
    string dictPath = arg["--dictpath"];
    string modelPath = arg["--modelpath"];
    string algorithm = arg["--algorithm"];
    int maxLen = atoi(arg["--maxlen"] == "" ? arg["--maxlen"].c_str() : "3");

    if("cutHMM" == algorithm)
    {
        HMMSegment seg(modelPath.c_str());
        if(!seg)
        {
            cout<<"seg init failed."<<endl;
            return EXIT_FAILURE;
        }
        cut(&seg, arg[1].c_str());
    }
    else if("cutDAG" == algorithm)
    {
        MPSegment seg(dictPath.c_str());
        if(!seg)
        {
            cout<<"seg init failed."<<endl;
            return false;
        }
        cut(&seg, arg[1].c_str());
    }
    else if ("cutFull" == algorithm)
    {
        FullSegment seg(dictPath.c_str());
        if (!seg)
        {
            cout << "seg init failed" << endl;
            return false;
        }
        cut(&seg, arg[1].c_str());
    }
    else if ("cutQuery" == algorithm)
    {
        QuerySegment seg(dictPath.c_str(), modelPath.c_str(), maxLen);
        if (!seg)
        {
            cout << "seg init failed" << endl;
            return false;
        }
        cut(&seg, arg[1].c_str());
    }
    else 
    {
        MixSegment seg(dictPath.c_str(), modelPath.c_str());
        if(!seg)
        {
            cout<<"seg init failed."<<endl;
            return EXIT_FAILURE;
        }
        cut(&seg, arg[1].c_str());
    }
    return EXIT_SUCCESS;
}
