#include <iostream>
#include <fstream>
#include "../src/headers.h"

using namespace CppJieba;


int main(int argc, char ** argv)
{
	if(argc < 2)
	{
		cerr<<"usage: "<<argv[0]<<" filename"<<endl;
		return 1;
	}
	Segment seg;
	if(!seg.init())
	{
		cerr<<"seg init failed."<<endl;
		return 1;
	}

	if(!seg.loadSegDict("../dicts/jieba.dict.gbk"))
	{
		cerr<<"seg loadDict failed."<<endl;
		return 1;
	}

	ifstream ifile(argv[1]);
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

	seg.dispose();
	return 0;
}
