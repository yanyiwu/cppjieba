/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#include "Segment.h"

namespace CppJieba
{
	Segment::Segment()
	{
		_encVec.push_back(Trie::UTF8);
		_encVec.push_back(Trie::GBK);
		//default encoding : utf8
		_encoding = Trie::UTF8;
	}
	
	Segment::~Segment()
	{
	}

	bool Segment::init(const string& dictFilePath)
	{
		bool retFlag;
		LogInfo(string_format("_trie.init(%s) start...", dictFilePath));
		retFlag = _trie.init(dictFilePath);
		LogInfo("_trie.init end.");
		return retFlag;
	}

	bool Segment::destroy()
	{
		return _trie.destroy();
	}

	bool Segment::cutDAG(const string& str, vector<string>& res)
	{
		bool retFlag;
		res.clear();
		string uniStr = _utf8ToUni(str;
		if(uniStr.empty())
		{
			LogError("_utf8ToUni failed.");
			return false;
		}

		//calc DAG
		vector<vector<uint> > dag;
		retFlag = _calcDAG(uniStr, dag);
		if(!retFlag)
		{
			LogError("_calcDAG failed.");
			return false;
		}
		//LogDebug("_calcDAG finished.");

		vector<pair<int, double> > dp;
		retFlag = _calcDP(uniStr, dag, dp);
		if(!retFlag)
		{
			LogError("_calcDP failed.");
			return false;
		}
		//LogDebug("_calcDP finished.");

		retFlag = _cutDAG(uniStr, dp, res);
		if(!retFlag)
		{
			LogError("_cutDAG failed.");
			return false;
		}
		//LogDebug("_cutDAG finished.");
		
		return true;
	}

	double Segment::getUtf8WordWeight(const string& word)
	{
		return _trie.getWeight(utf8ToUnicode(word));
	}

	double Segment::getUniWordWeight(const string& word)
	{
		return _trie.getWeight(word);
	}

	string Segment::_utf8ToUni(const string& utfStr)
	{
		string uniStr = utf8ToUnicode(utfStr);

		if(uniStr.empty() || uniStr.size() % 2)
		{
			LogError(string_format("utf8ToUnicode [%s] failed!", utfStr.c_str()));
			return "";
		}
		return uniStr;
	}

	bool Segment::_calcDAG(const string& uniStr, vector<vector<uint> >& dag)
	{
		for(uint i = 0; i < uniStr.size(); i+=2)
		{
			vector<uint> vec;
			vec.push_back(i/2);
			for(uint j = i + 4; j <= uniStr.size(); j+=2)
			{
				cout<<uniStr.substr(i, j - i)<<endl;
				if(NULL != _trie.find(uniStr.substr(i, j - i)))
				{
					vec.push_back((j - 2)/2);
				}
			}
			dag.push_back(vec);
		}
		return true;
	}

	bool Segment::_calcDP(const string& uniStr, const vector<vector<uint> >& dag, vector<pair<int, double> >& res)
	{
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
				int pos = dag[i/2][j];
				double val = getUniWordWeight(uniStr.substr(i, pos * 2 - i + 2)) + res[pos + 1].second;
				//cout<<pos<<","<<pos * 2 - i + 2<<","<<val<<endl;
				if(val > res[i/2].second)
				{
					res[i/2].first = pos;
					res[i/2].second = val;
				}
			}
		}
		res.pop_back();
		return true;
	}
	bool Segment::_cutDAG(const string& uniStr, const vector<pair<int, double> >& dp, vector<string>& res)
	{
		if(dp.size() != uniStr.size()/2)
		{
			LogError("dp or uniStr illegal!");
			return false;
		}

		res.clear();

		uint begin = 0;
		for(uint i = 0; i < dp.size(); i++)
		{
			//cout<<dp[i].first<<","
			//	<<dp[i].second<<endl;
			uint end = dp[i].first * 2 + 2;
			if(end <= begin)
			{
				continue;
			}
			string tmp = unicodeToUtf8(uniStr.substr(begin, end - begin));
			if(tmp.empty())
			{
				LogError("unicodeToUtf8 failed.");
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
	if(!segment.init("../dicts/segdict.utf8.v2.1"))
	{
		cerr<<"1"<<endl;
		return 1;
	}
	//segment.init("dicts/jieba.dict.utf8");
	
	vector<string> res;
	string title;
	title = "我来到北京清华大学";
	res.clear();
	segment.cutDAG(title, res);
	PRINT_VECTOR(res);
	getchar();
	
	title = "特价！camel骆驼 柔软舒适头层牛皮平底凉鞋女 休闲平跟妈妈鞋夏";
	res.clear();
	segment.cutDAG(title, res);
	PRINT_VECTOR(res);
	getchar();

	title = "包邮拉菲草18cm大檐进口草帽子超强遮阳防晒欧美日韩新款夏天 女";
	res.clear();
	segment.cutDAG(title, res);
	PRINT_VECTOR(res);
	getchar();

	title = "2013新款19CM超大檐帽 遮阳草帽子 沙滩帽防晒大檐欧美新款夏天女";
	res.clear();
	segment.cutDAG(title, res);
	PRINT_VECTOR(res);
	getchar();

	segment.destroy();
	return 0;
}

#endif
