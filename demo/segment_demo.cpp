#include <iostream>
#include <fstream>
#include "../src/headers.h"

using namespace CppJieba;

Segment seg;
bool init()
{
	if(!seg.init())
	{
		cout<<"seg init failed."<<endl;
		return false;
	}

	if(!seg.loadSegDict("../dicts/jieba.dict.gbk"))
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

int main(int argc, char ** argv)
{
	//map<string, string> mpss;
	//getArgvMap(argc, argv, mpss);
	//string enc = getMap<string, string>(mpss, "--encoding", "");
	
	if( argc < 2)
	{
		cout<<"usage: "<<"\n\t"<<argv[0]<<" <filename>"<<endl;
		return -1;
	}
	init();
	run(argv[1]);
	dispose();
	return 0;
}
