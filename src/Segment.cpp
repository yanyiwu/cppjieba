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

	bool Segment::init(const char* const filePath)
	{
		if(!_trie.init())
		{
			LogError("_trie.init failed.");
			return false;
		}
		LogInfo(string_format("_trie.loadDict(%s) start...", filePath));
		if(!_trie.loadDict(filePath))
		{
			LogError("_trie.loadDict faield.");
			return false;
		}
		LogInfo("_trie.loadDict end.");
		return true;
	}
	
	bool Segment::dispose()
	{
		return _trie.dispose();
	}

	bool Segment::cutDAG(const string& str, vector<string>& res)
	{
		vector<TrieNodeInfo> segWordInfos;
		if(!cutDAG(str, segWordInfos))
		{
			return false;
		}
		res.clear();
		for(uint i = 0; i < segWordInfos.size(); i++)
		{
			res.push_back(segWordInfos[i].word);
		}
		return true;
	}

	bool Segment::cutDAG(const string& str, vector<TrieNodeInfo>& segWordInfos)
	{
		if(str.empty())
		{
			return false;
		}
		segWordInfos.clear();
		SegmentContext segContext;
		
		if(!TransCode::strToVec(str, segContext.uintVec))
		{
			LogError("TransCode::strToVec failed.");
			return false;
		}
		
		//calc DAG
		if(!_calcDAG(segContext))
		{
			LogError("_calcDAG failed.");
			return false;
		}

		if(!_calcDP(segContext))
		{
			LogError("_calcDP failed.");
			return false;
		}

		if(!_cutDAG(segContext, segWordInfos))
		{
			LogError("_cutDAG failed.");
			return false;
		}

		return true;
	}

	bool Segment::_calcDAG(SegmentContext& segContext)
	{
		if(segContext.uintVec.empty())
		{
			return false;
		}
		vector<pair<uint, const TrieNodeInfo*> > vec;
		Unicode::const_iterator beginIter = segContext.uintVec.begin();
		for(Unicode::const_iterator iterI = segContext.uintVec.begin(); iterI != segContext.uintVec.end(); iterI++)
		{
			vec.clear();
			vec.push_back(pair<uint, const TrieNodeInfo*>(iterI - beginIter, NULL));
			for(Unicode::const_iterator iterJ = iterI + 1;  iterJ != segContext.uintVec.end(); iterJ++)
			{
				//care: the iterJ exceed iterEnd
				const TrieNodeInfo* ptNodeInfo = _trie.find(iterI, iterJ + 1);
				if(NULL != ptNodeInfo)
				{
					vec.push_back(pair<uint, const TrieNodeInfo*>(iterJ - beginIter, ptNodeInfo));
				}
			}
			segContext.dag.push_back(vec);
		}
		return true;
	}

	bool Segment::_calcDP(SegmentContext& segContext)
	{
		if(segContext.uintVec.empty())
		{
			LogError("uintVec illegal");
			return false;
		}

		if(segContext.uintVec.size() != segContext.dag.size())
		{
			LogError("dag is illegal!");
			return false;
		}

		segContext.dp.assign(segContext.uintVec.size() + 1, pair<const TrieNodeInfo*, double>(NULL, 0.0));
		segContext.dp[segContext.uintVec.size()].first = NULL;
		segContext.dp[segContext.uintVec.size()].second = 0.0;

		for(int i = segContext.uintVec.size() - 1; i >= 0; i--)
		{
			// calc max
			segContext.dp[i].first = NULL;
			segContext.dp[i].second = MIN_DOUBLE;
			for(uint j = 0; j < segContext.dag[i].size(); j++)
			{
				const pair<uint , const TrieNodeInfo*>& p = segContext.dag[i][j];
				int pos = p.first;
				double val = segContext.dp[pos+1].second;
				if(NULL != p.second)
				{
					val += (p.second)->logFreq; 
				}
				else
				{
				    val += _trie.getMinLogFreq();
				}

				if(val > segContext.dp[i].second)
				{
					segContext.dp[i].first = p.second;
					segContext.dp[i].second = val;
				}
			}
		}
		segContext.dp.pop_back();
		return true;
	}

	bool Segment::_cutDAG(SegmentContext& segContext, vector<TrieNodeInfo>& res)
	{
		if(segContext.dp.empty() || segContext.uintVec.empty() || segContext.dp.size() != segContext.uintVec.size())
		{
			LogError("dp or uintVec illegal!");
			return false;
		}
		res.clear();

		Unicode::const_iterator iterBegin = segContext.uintVec.begin();
		uint i = 0;
		while(i < segContext.dp.size())
		{
			const TrieNodeInfo* p = segContext.dp[i].first;
			if(NULL == p)
			{
				TrieNodeInfo nodeInfo;
				nodeInfo.word = TransCode::vecToStr(iterBegin + i, iterBegin + i +1);
				nodeInfo.wLen = 1;
				nodeInfo.freq = 0;
				nodeInfo.logFreq = _trie.getMinLogFreq();
				res.push_back(nodeInfo);
				i ++;
			}
			else
			{
				res.push_back(*p);
				if(0 == p->wLen)
				{
					LogFatal("TrieNodeInfo's wLen is 0!");
					return false;
				}
				i += p->wLen;
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
	segment.init();
	if(!segment._loadSegDict("../dicts/segdict.gbk.v3.0"))
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
