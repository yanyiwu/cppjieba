/************************************
 * file enc : AISCII
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
	
	bool Segment::loadSegDict(const char * const filePath)
	{
		LogInfo(string_format("_trie.loadDict(%s) start...", filePath));
		bool retFlag = _trie.loadDict(filePath);
		LogInfo("_trie.loadDict end.");
		return retFlag;
	}


	bool Segment::dispose()
	{
		return _trie.dispose();
	}

	double Segment::getWordWeight(const string& str)
	{
		return _trie.getWeight(str);
	}

	bool Segment::cutDAG(const string& str, vector<string>& res)
	{
		if(str.empty())
		{
			return false;
		}
		res.clear();

		bool retFlag;
		VUINT16 unicode;
		retFlag = TransCode::strToVec(str, unicode);
		if(!retFlag)
		{
			LogError("TransCode::strToVec failed.");
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

#ifdef DEBUG
		{
			string tmp("{");
			FOR_VECTOR(dag, i)
			{
				tmp += "[";
				FOR_VECTOR(dag[i], j)
				{
					tmp += string_format("%d,", dag[i][j]);
				}
				tmp += "],";
			}
			tmp += "}";
			LogDebug(tmp);
		}
#endif

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

	bool Segment::_calcDAG(const VUINT16& unicode, vector<vector<uint> >& dag)
	{
		if(unicode.empty())
		{
			return false;
		}
		VUINT16_CONST_ITER beginIter = unicode.begin();
		for(VUINT16_CONST_ITER iterI = unicode.begin(); iterI != unicode.end(); iterI++)
		{
			vector<uint> vec;
			vec.push_back(iterI - beginIter);
			for(VUINT16_CONST_ITER iterJ = iterI + 1;  iterJ != unicode.end(); iterJ++)
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

	bool Segment::_calcDP(const VUINT16& unicode, const vector<vector<uint> >& dag, vector<pair<int, double> >& res)
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

		VUINT16_CONST_ITER iterBegin = unicode.begin();

		for(int i = unicode.size() - 1; i >= 0; i--)
		{
			// calc max
			res[i].first = -1;
			res[i].second = -(numeric_limits<double>::max());
			for(uint j = 0; j < dag[i].size(); j++)
			{
				//cout<<(i/2)<<","<<dag[i/2].size()<<","<<j<<endl;
				int pos = dag[i][j];
				double val = _trie.getWeight(iterBegin + i, iterBegin + pos + 1) + res[pos + 1].second;
				//cout<<i<<","<<pos<<","<<val<<endl;
				//double val = _trie.getWeight(uniStr.substr(i, pos * 2 - i + 2)) + res[pos + 1].second;
				//cout<<pos<<","<<pos * 2 - i + 2<<","<<val<<endl;
				if(val > res[i].second)
				{
					res[i].first = pos;
					res[i].second = val;
				}
			}
		}
		//FOR_VECTOR(res, i)
		//{
		//	cout<<i<<","<<res[i].first<<","<<res[i].second<<endl;
		//}
		res.pop_back();
		return true;
	}
	bool Segment::_cutDAG(const VUINT16& unicode, const vector<pair<int, double> >& dp, vector<string>& res)
	{
		if(dp.size() != unicode.size())
		{
			LogError("dp or unicode illegal!");
			return false;
		}

		res.clear();

		uint begin = 0, end = 0;
		VUINT16_CONST_ITER iterBegin = unicode.begin();
		//for(uint i = 0; i < dp.size(); i++)
		while(begin < dp.size() && end <= dp.size())
		{
			//cout<<begin<<","
			//	<<dp[i].first<<","
			//	<<dp[i].second<<endl;
			end = dp[begin].first + 1;
			//cout<<begin<<","<<end<<endl;
			//if(end <= begin)
			//{
		//		continue;
		//	}
			//cout<<begin<<","<<end<<endl;
			//string tmp = TransCode::vecToStr(uniStr.substr(begin, end - begin));
			string tmp = TransCode::vecToStr(iterBegin + begin, iterBegin + end);
			if(tmp.empty())
			{
				LogError("TransCode::vecToStr failed.");
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
	if(!segment.loadSegDict("../dicts/segdict.gbk.v3.0"))
	{
		cerr<<"1"<<endl;
		return 1;
	}
	//segment.init("dicts/jieba.dict.utf8");
	//ifstream ifile("testtitle.gbk");
	ifstream ifile("badcase");
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
