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
	ifstream ifile(filePath);
	vector<string> res;
	string line;
	while(getline(ifile, line))
	{
		res.clear();
		if(!line.empty())
		{
			ext.extract(line, res, 20);
			cout<<line<<"\n"<<joinStr(res," ")<<endl;
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
	vector<string> keywords;
	string line;
	while(getline(ifile, line))
	{
		if(!line.empty())
		{
			seg.cutDAG(line, words);
			ext.extract(words, keywords, 20);
			cout<<line<<"\n"<<joinStr(keywords," ")<<endl;
		}

	}
	seg.dispose();
	ext.dispose();
}

int main(int argc, char ** argv)
{
	if(argc != 3)
	{
		cerr<<"usage: "<<argv[0]<<" ../dicts/jieba.dict.gbk filename"<<endl;
		return -1;
	}
	testKeyWordExt(argv[1], argv[2]);
	return 0;
}
