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

	bool Segment::cutDAG(const string& chStr, vector<string>& res)
	{
		bool retFlag;
		res.clear();
		string uniStr = _utf8ToUni(chStr);
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
		LogDebug("_calcDAG finished.");

		vector<pair<int, double> > dp;
		retFlag = _calcDP(uniStr, dag, dp);
		if(!retFlag)
		{
			LogError("_calcDP failed.");
			return false;
		}
		LogDebug("_calcDP finished.");

		retFlag = _cutDAG(uniStr, dp, res);
		if(!retFlag)
		{
			LogError("_cutDAG failed.");
			return false;
		}
		LogDebug("_cutDAG finished.");
		
		return true;
	}
	bool Segment::extract(const string& utf8Str, vector<string>& keywords, uint topN)
	{
		LogInfo(utf8Str);
		bool retFlag;
		vector<string> tmp;
		retFlag = cutDAG(utf8Str, tmp);
		if(!retFlag)
		{
			LogError(string_format("cutDAG(%s) failed.", utf8Str.c_str()));
			return false;
		}
		// like str.join([]) in python
		LogDebug(string_format("cutDAG result:[%s]", joinStr(tmp, ",").c_str()));

		retFlag = _filter(tmp);
		if(!retFlag)
		{
			LogError("_filter failed.");
			return false;
		}
		LogDebug(string_format("_filter res:[%s]", joinStr(tmp, ",").c_str()));

		retFlag = _extractTopN(tmp, keywords, topN);
		if(!retFlag)
		{
			LogError("_extractTopN failed.");
			return false;
		}
		LogDebug("_extractTopN finished.");

		LogInfo(string_format("ext res:[%s]", joinStr(keywords, ",").c_str()));
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

	bool Segment::_pair_compare(const pair<string, double>& a, const pair<string, double>& b)
	{
		return a.second < b.second;
	}

	bool Segment::_extractTopN(const vector<string>& words, vector<string>& keywords, uint topN)
	{
		keywords.clear();
		vector<pair<string, double> > tmp;

		for(uint i = 0; i < words.size(); i++)
		{
			double w = getUtf8WordWeight(words[i]);
			tmp.push_back(pair<string, double>(words[i], w));
		}
		
		sort(tmp.begin(), tmp.end(), _pair_compare);

		//logging result
		vector<string> logBuf;//for LogDebug
		for(uint i = 0; i < tmp.size(); i++)
		{
			logBuf.push_back(string_format("(%s,%lf)", tmp[i].first.c_str(), tmp[i].second));
		}
		LogDebug(string_format("calc weight:%s",joinStr(logBuf, ",").c_str()));
		
		//extract TopN
		for(uint i = 0; i < topN && i < tmp.size(); i++)
		{
			keywords.push_back(tmp[i].first);
		}
		return true;
	}

	string Segment::_utf8ToUni(const string& utfStr)
	{
		string uniStr = utf8ToUnicode(utfStr);

		if(uniStr.empty())
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
				//getchar();
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

	bool Segment::_filter(vector<string>& utf8Strs)
	{
		bool retFlag;
		retFlag = _filterSingleWord(utf8Strs);
		if(!retFlag)
		{
			LogError("_filterSingleWord failed.");
			return false;
		}
		LogDebug(string_format("_filterSingleWord res:[%s]", joinStr(utf8Strs, ",").c_str()));

		retFlag = _filterSubstr(utf8Strs);
		if(!retFlag)
		{
			LogError("_filterSubstr failed.");
			return false;
		}
		LogDebug(string_format("_filterSubstr res:[%s]", joinStr(utf8Strs, ",").c_str()));

		return true;
	}

	bool Segment::_filterSingleWord(vector<string>& utf8Strs)
	{
		for(vector<string>::iterator it = utf8Strs.begin(); it != utf8Strs.end();)
		{
			string uniStr = utf8ToUnicode(*it);
			if(uniStr.empty() || uniStr.size()%2)
			{
				LogError("utf8ToUnicode error");
				return false;
			}

			// filter single word
			if(uniStr.size() == 2) 
			{
				it = utf8Strs.erase(it);
			}
			else
			{
				it++;
			}
		}
		return true;
	}

	bool Segment::_filterSubstr(vector<string>& utf8Strs)
	{
		vector<string> tmp = utf8Strs;
		set<string> subs;
		for(VSI it = utf8Strs.begin(); it != utf8Strs.end(); it ++)
		{
			for(uint j = 0; j < tmp.size(); j++)
			{
				if(*it != tmp[j] && string::npos != tmp[j].find(*it, 0))
				{
					subs.insert(*it);
				}
			}
		}

		//erase subs from utf8Strs
		for(VSI it = utf8Strs.begin(); it != utf8Strs.end(); )
		{
			if(subs.end() != subs.find(*it))
			{
				LogDebug(string_format("_filterSubstr filter [%s].", it->c_str()));
				it =  utf8Strs.erase(it);
			}
			else
			{
				it ++;
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
	segment.extract(title, res);
	
	title = "特价！camel骆驼 柔软舒适头层牛皮平底凉鞋女 休闲平跟妈妈鞋夏";
	res.clear();
	segment.extract(title, res);

	title = "包邮拉菲草18cm大檐进口草帽子超强遮阳防晒欧美日韩新款夏天 女";
	res.clear();
	segment.extract(title, res);

	title = "2013新款19CM超大檐帽 遮阳草帽子 沙滩帽防晒大檐欧美新款夏天女";
	res.clear();
	segment.extract(title, res);

	segment.destroy();
	return 0;
}

#endif
