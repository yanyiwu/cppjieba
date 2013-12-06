#include <iostream>
#include <fstream>
#include "Limonp/ArgvContext.hpp"
#include "MPSegment.hpp"
#include "HMMSegment.hpp"
#include "MixSegment.hpp"

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
            <<"\t--algorithm\tSupported methods are [cutDAG, cutHMM, cutMix] for now. \n\t\t\tIf not specified, the default is cutMix\n"
            <<"\t--dictpath\tsee example\n"
            <<"\t--modelpath\tsee example\n"
            <<"example:\n"
            <<"\t"<<argv[0]<<" ../test/testlines.utf8 --dictpath ../dicts/jieba.dict.utf8 --algorithm cutDAG\n"
            <<"\t"<<argv[0]<<" ../test/testlines.utf8 --modelpath ../dicts/hmm_model.utf8 --algorithm cutHMM\n"
            <<"\t"<<argv[0]<<" ../test/testlines.utf8 --dictpath ../dicts/jieba.dict.utf8 --modelpath ../dicts/hmm_model.utf8 --algorithm cutMix\n"
            <<endl;
        
        return EXIT_FAILURE;
    }
    ArgvContext arg(argc, argv);
    string dictPath = arg["--dictpath"];
    string modelPath = arg["--modelpath"];
    string algorithm = arg["--algorithm"];

    if("cutHMM" == algorithm)
    {
        HMMSegment seg(modelPath.c_str());
        if(!seg.init())
        {
            cout<<"seg init failed."<<endl;
            return EXIT_FAILURE;
        }
        cut(&seg, arg[1].c_str());
        seg.dispose();
    }
    else if("cutDAG" == algorithm)
    {
        MPSegment seg(dictPath.c_str());
        if(!seg.init())
        {
            cout<<"seg init failed."<<endl;
            return false;
        }
        cut(&seg, arg[1].c_str());
        seg.dispose();
    }
    else
    {
        MixSegment seg(dictPath.c_str(), modelPath.c_str());
        if(!seg.init())
        {
            cout<<"seg init failed."<<endl;
            return EXIT_FAILURE;
        }
        cut(&seg, arg[1].c_str());
        seg.dispose();
    }
    return EXIT_SUCCESS;
}
