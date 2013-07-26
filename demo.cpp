#include <iostream>
#include <fstream>
#include "src/KeyWordExt.h"
#include "src/cppcommon/str_functs.h"
#include "src/cppcommon/vec_functs.h"

using namespace std;

using namespace CppJieba;

int main(int argc, char ** argv)
{
	if(argc < 2)
	{
		cerr<<"usage: "<<argv[0]<<" filename"<<endl;
		return 1;
	}
	KeyWordExt ext;
	ext.init();
	
	if(!ext.loadSegDict("./dicts/segdict.gbk.v3.0"))
	{
		cerr<<"1"<<endl;
		return 1;
	}
	ifstream ifile(argv[1]);
	vector<string> res;
	string line;
	while(getline(ifile, line))
	{
		res.clear();
		ext.extract(line, res, 20);
		cout<<line<<"\n"<<joinStr(res," ")<<endl;
	}
	
	ext.dispose();
	
    return 0;
}
