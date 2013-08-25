#include <iostream>
#include <fstream>
#include "../src/headers.h"

using namespace CppJieba;

Segment seg;
HMMSegment hmmseg;
bool init(const char * const dictPath, const char * const modelPath)
{
	if(!seg.init())
	{
		cout<<"seg init failed."<<endl;
		return false;
	}

	if(!seg.loadSegDict(dictPath))
	{
		cout<<"seg loadDict failed."<<endl;
		return false;
	}
	if(!hmmseg.loadModel(modelPath))
	{
		cout<<"hmmseg loadModel failed."<<endl;
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

void hmmrun(const char * const filePath)
{
	ifstream ifile(filePath);
	vector<string> res;
	string line;
	while(getline(ifile, line))
	{
		res.clear();
		if(!line.empty())
		{
			hmmseg.cut(line, res);
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
const char * const DEFAULT_MODELPATH = "../dicts/hmm_model.utf8";

int main(int argc, char ** argv)
{
	if(argc < 2)
	{
		cout<<"usage: \n\t"<<argv[0]<<"[options] <filename>\n"
		    <<"options:\n"
			<<"\t--algorithm\tSupported encoding methods are [cutDAG, cutHMM] for now. \n\t\t\tIf is not specified, the default is cutDAG\n"
		    <<"\t--dictpath\tIf is not specified, the default is "<<DEFAULT_DICTPATH<<'\n'
		    <<"\t--modelpath\tIf is not specified, the default is "<<DEFAULT_MODELPATH<<'\n'
		    <<"\t--encoding\tSupported encoding methods are [gbk, utf-8] for now. \n\t\t\tIf is not specified, the default is utf8.\n"
			<<"example:\n"
			<<"\t"<<argv[0]<<" testlines.utf8 --encoding utf-8 --dictpath ../dicts/jieba.dict.utf8\n"
			<<"\t"<<argv[0]<<" testlines.utf8 --modelpath ../dicts/hmm_model.utf8 --algorithm cutHMM\n"
			<<"\t"<<argv[0]<<" testlines.gbk --encoding gbk --dictpath ../dicts/jieba.dict.gbk\n"
			<<endl;
		
		return -1;
	}
	ArgvContext arg(argc, argv);
	string dictPath = arg["--dictpath"];
	string modelPath = arg["--modelpath"];
	string encoding = arg["--encoding"];
	string algorithm = arg["--algorithm"];
	if(dictPath.empty())
	{
		dictPath = DEFAULT_DICTPATH;
	}
	if(modelPath.empty())
	{
		modelPath = DEFAULT_MODELPATH;
	}
	if("gbk" == encoding)
	{
		TransCode::setGbkEnc();
	}
	else
	{
		TransCode::setUtf8Enc();
	}

	if(!init(dictPath.c_str(), modelPath.c_str()))
	{
		LogError("init failed.");
		return -1;
	}
	if("cutHMM" == algorithm)
	{
		hmmrun(arg[1].c_str());
	}
	else
	{
		run(arg[1].c_str());
	}
	dispose();
	return 0;
}
