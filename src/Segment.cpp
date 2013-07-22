/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#include "Segment.h"

namespace CppJieba
{
	Segment::Segment()
	{
	}
	
	Segment::~Segment()
	{
	}

	bool Segment::init()
	{
		bool retFlag = _trie.init();
		if(!retFlag)
		{
			LogError("_trie.init failed.");
			return false;
		}
		return true;
	}
	
	bool Segment::loadSegDict(const string& filePath)
	{
		LogInfo(string_format("_trie.loadDict(%s) start...", filePath.c_str()));
		bool retFlag = _trie.loadDict(filePath);
		LogInfo("_trie.loadDict end.");
		return retFlag;
	}


	bool Segment::dispose()
	{
		return _trie.dispose();
	}

	bool Segment::cutDAG(const string& str, vector<string>& res)
	{
		res.clear();

		bool retFlag;
		Unicode unicode;
		retFlag = gEncoding.decode(str, unicode);
		if(!retFlag)
		{
			LogError("gEncoding.decode failed.");
			return false;
		}
		
		//calc DAG
		vector<vector<uint> > dag;
		retFlag = _calcDAG(unicode, dag);
		if(!retFlag)
		{
			LogError("_calcDAG failed.");
			return false;
		}

		vector<pair<int, double> > dp;
		retFlag = _calcDP(unicode, dag, dp);
		if(!retFlag)
		{
			LogError("_calcDP failed.");
			return false;
		}

		retFlag = _cutDAG(unicode, dp, res);
		if(!retFlag)
		{
			LogError("_cutDAG failed.");
			return false;
		}
		
		return true;
	}

	bool Segment::_calcDAG(const Unicode& unicode, vector<vector<uint> >& dag)
	{
		if(unicode.empty())
		{
			return false;
		}
		typedef UnicodeConstIterator UCI;
		UCI beginIter = unicode.begin();
		for(UCI iterI = unicode.begin(); iterI != unicode.end(); iterI++)
		{
			vector<uint> vec;
			vec.push_back(iterI - beginIter);
			for(UCI iterJ = iterI + 1;  iterJ != unicode.end(); iterJ++)
			{
				//care: the iterJ exceed iterEnd
				if(NULL != _trie.find(iterI, iterJ + 1))
				{
					vec.push_back(iterJ - beginIter);
				}
			}
			dag.push_back(vec);
		}
		return true;
	}

	bool Segment::_calcDP(const Unicode& unicode, const vector<vector<uint> >& dag, vector<pair<int, double> >& res)
	{
		if(unicode.empty())
		{
			LogError("unicode illegal");
			return false;
		}

		if(unicode.size() != dag.size())
		{
			LogError("dag is illegal!");
			return false;
		}

		res.clear();
		res.assign(unicode.size() + 1, pair<int, double>(-1, 0.0));
		res[unicode.size()].first = -1;
		res[unicode.size()].second = 0.0;

		UnicodeConstIterator iterBegin = unicode.begin();

		for(int i = unicode.size() - 1; i >= 0; i--)
		{
			// calc max
			res[i].first = -1;
			res[i].second = -(numeric_limits<double>::max());
			for(int j = 0; j < dag[i].size(); j++)
			{
				//cout<<(i/2)<<","<<dag[i/2].size()<<","<<j<<endl;
				int pos = dag[i][j];
				double val = _trie.getWeight(iterBegin + i, iterBegin + pos + 1) + res[pos + 1].second;
				//double val = _trie.getWeight(uniStr.substr(i, pos * 2 - i + 2)) + res[pos + 1].second;
				//cout<<pos<<","<<pos * 2 - i + 2<<","<<val<<endl;
				if(val > res[i].second)
				{
					res[i].first = pos;
					res[i].second = val;
				}
			}
		}
		res.pop_back();
		return true;
	}
	bool Segment::_cutDAG(const Unicode& unicode, const vector<pair<int, double> >& dp, vector<string>& res)
	{
		if(dp.size() != unicode.size())
		{
			LogError("dp or unicode illegal!");
			return false;
		}

		res.clear();

		uint begin = 0;
		UnicodeConstIterator iterBegin = unicode.begin();
		for(uint i = 0; i < dp.size(); i++)
		{
			//cout<<dp[i].first<<","
			//	<<dp[i].second<<endl;
			uint end = dp[i].first + 1;
			if(end <= begin)
			{
				continue;
			}
			//string tmp = gEncoding.encode(uniStr.substr(begin, end - begin));
			string tmp = gEncoding.encode(iterBegin + begin, iterBegin + end);
			if(tmp.empty())
			{
				LogError("gEncoding.encode failed.");
				return false;
			}
			res.push_back(tmp);
			begin = end;
		}
		return true;
	}

}


#ifdef SEGMENT_UT
using namespace CppJieba;

int main()
{
	Segment segment;
	segment.init();
	if(!segment.loadSegDict("../dicts/segdict.utf8.v2.1"))
	{
		cerr<<"1"<<endl;
		return 1;
	}
	//segment.init("dicts/jieba.dict.utf8");
	ifstream ifile("testtitle");
	vector<string> res;
	string line;
	while(getline(ifile, line))
	{
		res.clear();
		segment.cutDAG(line, res);
		PRINT_VECTOR(res);
		getchar();
	}

	segment.dispose();
	return 0;
}

#endif
