#include <iostream>
#include <fstream>
#include <ArgvContext.hpp>
#include "MPSegment.h"
#include "HMMSegment.h"
#include "MixSegment.h"

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
            cout<<line<<"\n"<<join(res.begin(), res.end(),"/")<<endl;
        }
    }
}

int main(int argc, char ** argv)
{
    if(argc < 2)
    {
        cout<<"usage: \n\t"<<argv[0]<<" [options] <filename>\n"
            <<"options:\n"
            <<"\t--algorithm\tSupported methods are [cutDAG, cutHMM, cutMix] for now. \n\t\t\tIf not specified, the default is cutDAG\n"
            <<"\t--dictpath\tsee example\n"
            <<"\t--modelpath\tsee example\n"
            <<"example:\n"
            <<"\t"<<argv[0]<<" testlines.utf8 --dictpath dicts/jieba.dict.utf8\n"
            <<"\t"<<argv[0]<<" testlines.utf8 --modelpath dicts/hmm_model.utf8 --algorithm cutHMM\n"
            <<"\t"<<argv[0]<<" testlines.utf8 --dictpath dicts/jieba.dict.utf8 --modelpath dicts/hmm_model.utf8 --algorithm cutMix\n"
            <<endl;
        
        return EXIT_FAILURE;
    }
    ArgvContext arg(argc, argv);
    string dictPath = arg["--dictpath"];
    string modelPath = arg["--modelpath"];
    string algorithm = arg["--algorithm"];
    if(dictPath.empty())
    {
        return EXIT_FAILURE;
    }
    if(modelPath.empty())
    {
        return EXIT_FAILURE;
    }

    if("cutHMM" == algorithm)
    {
        HMMSegment seg;
        if(!seg.init(modelPath.c_str()))
        {
            cout<<"seg init failed."<<endl;
            return EXIT_FAILURE;
        }
        cut(&seg, arg[1].c_str());
        seg.dispose();
    }
    else if("cutMix" == algorithm)
    {
        MixSegment seg;
        if(!seg.init(dictPath.c_str(), modelPath.c_str()))
        {
            cout<<"seg init failed."<<endl;
            return EXIT_FAILURE;
        }
        cut(&seg, arg[1].c_str());
        seg.dispose();
    }
    else
    {
        MPSegment seg;
        if(!seg.init(dictPath.c_str()))
        {
            cout<<"seg init failed."<<endl;
            return false;
        }
        cut(&seg, arg[1].c_str());
        seg.dispose();
    }
    return EXIT_SUCCESS;
}
