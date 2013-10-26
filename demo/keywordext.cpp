#include <iostream>
#include <fstream>
#include <ArgvContext.hpp>
#include "../cppjieba/headers.h"

using namespace CppJieba;


void testKeyWordExt(const char * dictPath, const char * filePath)
{
    KeyWordExt ext;
    if(!ext.init(dictPath))
    {
        return;
    }

    ifstream ifile(filePath);
    vector<KeyWordInfo> res;
    string line;
    while(getline(ifile, line))
    {
        res.clear();
        if(!line.empty())
        {
            ext.extract(line, res, 20);
            cout<<line<<'\n'<<joinWordInfos(res)<<endl;
        }

    }
    ext.dispose();
}

const char * const DEFAULT_DICTPATH = "../dicts/jieba.dict.utf8";

int main(int argc, char ** argv)
{
    if(2 > argc)
    {
        cout<<"usage: \n\t"<<argv[0]<<" [options] <filename>\n"
            <<"options:\n"
            <<"\t--dictpath\tIf not specified, the default is "<<DEFAULT_DICTPATH<<"\n"
            <<"examples:\n"
            <<"\t"<<argv[0]<<" testlines.utf8 --encoding utf-8 --dictpath ../dicts/jieba.dict.utf8\n"
            <<endl;
        return -1;
    }

    ArgvContext arg(argc, argv);
    string dictPath = arg["--dictpath"];
    if("" == dictPath)
    {
        dictPath = DEFAULT_DICTPATH;
    }
    testKeyWordExt(dictPath.c_str(), arg[1].c_str());
    return 0;
}
