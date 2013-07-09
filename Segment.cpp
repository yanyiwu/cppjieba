#include "Segment.h"

namespace CppJieba
{
	Segment::Segment():_trie()
	{
	}
	
	Segment::~Segment()
	{
	}

	bool Segment::init(const char* const dictFilePath)
	{
		_trie.init(dictFilePath);
	}

	bool Segment::destroy()
	{
		return _trie.destroy();
	}

	bool Segment::cutDAG(const string& chStr, vector<string>& res)
	{
		res.clear();
		string uniStr = _utf8ToUni(chStr);
		if(uniStr.empty())
		{
			LogError("_utf8ToUni failed.");
			return false;
		}

		//calc DAG
		vector<vector<uint> > dag;
		for(uint i = 0; i < uniStr.size(); i+=2)
		{
			vector<uint> vec;
			vec.push_back(i/2);
			for(uint j = i + 4; j <= uniStr.size(); j+=2)
			{
				if(NULL != _trie.find(uniStr.substr(i, j - i)))
				{
					vec.push_back((j - 2)/2);
				}
			}
			dag.push_back(vec);
		}

		cout<<__FILE__<<__LINE__<<endl;
		PRINT_MATRIX(dag);
		getchar();

		vector<pair<int, double> > dp;
		_calcDP(uniStr, dag, dp);

		cout<<__FILE__<<__LINE__<<endl;
		for(int i = 0 ;i< dp.size(); i++)
		{
			cout<<dp[i].first<<","
				<<dp[i].second<<endl;
		}

		

		

		//calc dp
		


		
		return true;
	}


	/*
	bool Segment::cutMM(const string& chStr, vector<string>& res)
	{
		res.clear();
		string uniStr = _utf8ToUni(chStr);
		if(uniStr.empty())
		{
			LogError("_utf8ToUni failed.");
			return false;
		}

		int i = 0;
		while(i < len)
		{
			cout<<__FILE__<<__LINE__<<i<<endl;
			int pos = _trie.findMaxMatch(uniStr + i, len - i);
			if(-1 != pos)
			{
				int utfLen = unicodeToUtf8(uniStr + i, pos, utfBuf);
				if(0 == utfLen)
				{
					LogError("unicodeToUtf8 failed!");
					return false;
				}
				res.push_back(utfBuf);

				i += pos;
			}
			else
			{
				i++;
			}
		}
		return true;
	}
	*/

	/*
	bool Segment::cutRMM(const string& chStr, vector<string>& res)
	{
		res.clear();
		char utfBuf[bufSize];

		string uniStr = _utf8ToUni(chStr);
		if(0 == len)
		{
			LogError("_utf8ToUni failed.");
			return false;
		}

		int i = len - 1;
		while(i >= 0)
		{
			bool flag = false;
			for(int j = 0; j <= i; j++)
			{
				size_t uniLen = i - j + 1;
				if(NULL != _trie.find(uniStr + j, uniLen))
				{
					memset(utfBuf, 0 ,sizeof(utfBuf));
					size_t ret = unicodeToUtf8(uniStr + j, uniLen, utfBuf);
					if(0 == ret)
					{
						LogError("unicodeToUtf8 failed!");
						return false;
					}
					res.push_back(utfBuf);
					flag = true;
					i -= uniLen;
					break;
				}
			}
			if(!flag)
			{
				i--;
			}
		}
		return true;
	}
	*/

	string Segment::_utf8ToUni(const string& utfStr)
	{
		char logBuf[bufSize];
		string uniStr = utf8ToUnicode(utfStr);

		if(uniStr.empty())
		{
			sprintf(logBuf, "utf8ToUnicode [%s] failed!", utfStr.c_str());
			LogError(logBuf);
			return "";
		}
		return uniStr;
	}

	bool Segment::_calcDP(const string& uniStr, const vector<vector<uint> >& dag, vector<pair<int, double> >& res)
	{
		/*
		for(int i =0;i<dag.size();i++)
		{
			cout<<i<<","<<dag[i].size()<<",";
			for(int j =0;j<dag[i].size();j++)
			{
				cout<<j<<","<<dag[i][j]<<",";
			}
			cout<<endl;
		}
		getchar();
		*/

		if(uniStr.size() / 2 != dag.size())
		{
			LogError("dag is illegal!");
			return false;
		}
		if(uniStr.size() < 2)
		{
			LogError("uniStr illegal");
			return false;
		}

		res.clear();
		res.assign(uniStr.size()/2 + 1, pair<int, double>(-1, 0.0));
		res[uniStr.size()/2].first = -1;
		res[uniStr.size()/2].second = 0.0;
		for(int i = uniStr.size() - 2; i >= 0; i-=2)
		{
			// calc max
			res[i/2].first = -1;
			res[i/2].second = -(numeric_limits<double>::max());
			for(int j = 0; j < dag[i/2].size(); j++)
			{
				//cout<<(i/2)<<","<<dag[i/2].size()<<","<<j<<endl;
				//getchar();
				int pos = dag[i/2][j];
				double val = _trie.getWeight(uniStr.substr(i, pos * 2 - i + 2)) + res[pos + 1].second;
				//cout<<pos<<","<<pos * 2 - i + 2<<","<<val<<endl;
				if(val > res[i/2].second)
				{
					res[i/2].first = pos;
					res[i/2].second = val;
				}
			}
		}
		return true;
	}

}


#ifdef SEGMENT_UT
using namespace CppJieba;

int main()
{
	Segment segment;
	segment.init("jieba.dict.utf8");
	
	vector<string> res;
	string title = "我来到北京清华大学";
	/*segment.cutMM(title, res);
	for(int i = 0; i < res.size(); i++)
	{
		cout<<res[i]<<endl;
	}*/
	segment.cutDAG(title, res);
	for(int i = 0; i < res.size(); i++)
	{
		cout<<res[i]<<endl;
	}

	segment.destroy();
	return 0;
}

#endif
