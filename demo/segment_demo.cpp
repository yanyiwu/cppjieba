#include <iostream>
#include <fstream>
#include "../src/headers.h"

using namespace CppJieba;

Segment seg;
bool init(const char * const filePath)
{
	if(!seg.init())
	{
		cout<<"seg init failed."<<endl;
		return false;
	}

	if(!seg.loadSegDict(filePath))
	{
		cout<<"seg loadDict failed."<<endl;
		return false;
	}
	return true;
}

void run(const char * const filePath)
{
	ifstream ifile(filePath);
	vector<string> res;
	string line;
	while(getline(ifile, line))
	{
		res.clear();
		if(!line.empty())
		{
			seg.cutDAG(line, res);
			cout<<line<<"\n"<<joinStr(res,"/")<<endl;
		}
	}
}

bool dispose()
{
	if(!seg.dispose())
	{
		cout<<"seg dispose failed."<<endl;
		return false;
	}
	return true;
}

const char * const DEFAULT_DICTPATH = "../dicts/jieba.dict.utf8";

int main(int argc, char ** argv)
{
	/*map<string, string> mpss;
	getArgvMap(argc, argv, mpss);
	string enc = getMap<string, string>(mpss, "--encoding", "");
	string dictPath = getMap<string, string>(mpss, "--dictpath", "../dicts/jieba.dict.gbk");
	*/
	if(argc < 2)
	{
		cout<<"usage: \n\t"<<argv[0]<<" <filename> [options]\n"
		    <<"options:\n"
		    <<"\t--dictpath\tIf is not specified, the default is ../dicts/jieba.dict.utf8\n"
		    <<"\t--encoding\tSupported encoding methods are [gbk, utf-8] for now. \n\t\t\tIf is not specified, the default is utf8.\n"
			<<"example:\n"
			<<"\t"<<argv[0]<<" testlines.utf8 --encoding utf-8 --dictpath ../dicts/jieba.dict.utf8\n"
			<<"\t"<<argv[0]<<" testlines.gbk --encoding gbk --dictpath ../dicts/jieba.dict.gbk\n"
			<<endl;
		
		return -1;
	}
	ArgvContext arg(argc, argv);
	string dictPath = arg["--dictpath"];
	string encoding = arg["--encoding"];
	if("" == dictPath)
	{
		dictPath = DEFAULT_DICTPATH;
	}
	if("gbk" == encoding)
	{
		TransCode::setGbkEnc();
	}
	else
	{
		TransCode::setUtf8Enc();
	}

	init(dictPath.c_str());
	run(argv[1]);
	dispose();
	return 0;
}
