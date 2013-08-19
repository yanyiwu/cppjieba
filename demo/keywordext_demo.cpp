#include <iostream>
#include <fstream>
#include "../src/headers.h"

using namespace CppJieba;


void testKeyWordExt(const char * dictPath, const char * filePath)
{
	KeyWordExt ext;
	ext.init();

	if(!ext.loadSegDict(dictPath))
	{
		cerr<<__FILE__<<__LINE__<<endl;
		return ;
	}
	if(!ext.loadStopWords("../dicts/stopwords.gbk.v1.0"))
	{
		cerr<<__FILE__<<__LINE__<<endl;
		return ;
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

void testKeyWordExt2(const char * dictPath, const char * filePath)
{
	Segment seg;
	seg.init();
	KeyWordExt ext;
	ext.init();

	if(!seg.loadSegDict(dictPath))
	{
		cerr<<__FILE__<<__LINE__<<endl;
		return ;
	}

	if(!ext.loadSegDict(dictPath));
	{
		cerr<<__FILE__<<__LINE__<<endl;
		return ;
	}

	ifstream ifile(filePath);
	vector<string> words;
	vector<KeyWordInfo> res;
	string line;
	while(getline(ifile, line))
	{
		if(!line.empty())
		{
			seg.cutDAG(line, words);
			ext.extract(words, res, 20);
			cout<<line<<"\n"<<joinWordInfos(res)<<endl;
		}

	}
	seg.dispose();
	ext.dispose();
}

const char * const DEFAULT_DICTPATH = "../dicts/jieba.dict.gbk";

int main(int argc, char ** argv)
{
	ArgvContext arg(argc, argv);
	string dictPath = arg["--dictpath"];
	if("" == dictPath)
	{
		dictPath = DEFAULT_DICTPATH;
	}
	if("" == arg[1])
	{
		cout<<"usage: \n\t"<<argv[0]<<" [options] <filename>\n"
		    <<"options:\n"
		    <<"\t--dictpath\tIf is not specified, the default is "<<DEFAULT_DICTPATH<<"\n"
		    <<"\t--encoding\tSupported encoding methods are [gbk, utf-8] for now. \n\t\t\tIf is not specified, the default is gbk."
			<<endl;
		return -1;
	}
	
	testKeyWordExt(dictPath.c_str(), arg[1].c_str());
	return 0;
}
