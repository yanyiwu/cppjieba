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
		char utfBuf[bufSize];
		ChUnicode uniStr[bufSize];
		memset(uniStr, 0, sizeof(uniStr));
		size_t len = _utf8ToUni(chStr, uniStr, bufSize);
		if(0 == len)
		{
			LogError("_utf8ToUni failed.");
			return false;
		}

		//calc DAG
		vector<vector<uint> > dag;
		for(uint i = 0; i < len; i++)
		{
			vector<uint> vec;
			vec.push_back(i);
			for(uint j = i + 2; j <= len; j++)
			{
				if(NULL != _trie.find(uniStr + i, j - i))
				{
					vec.push_back(j - 1);
				}
			}
			dag.push_back(vec);
		}
		PRINT_MATRIX(dag);
		getchar();

		

		//calc dp
		


		
		return true;
	}


	bool Segment::cutMM(const string& chStr, vector<string>& res)
	{
		res.clear();
		char utfBuf[bufSize];
		ChUnicode uniStr[bufSize];
		memset(uniStr, 0, sizeof(uniStr));
		size_t len = _utf8ToUni(chStr, uniStr, bufSize);
		if(0 == len)
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

	bool Segment::cutRMM(const string& chStr, vector<string>& res)
	{
		res.clear();
		char utfBuf[bufSize];
		ChUnicode uniStr[bufSize];
		memset(uniStr, 0, sizeof(uniStr));

		size_t len = _utf8ToUni(chStr, uniStr, bufSize);
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

	size_t Segment::_utf8ToUni(const string& chStr, ChUnicode* uniStr, size_t size)
	{
		char logBuf[bufSize];
		size_t len = utf8ToUnicode(chStr.c_str(), chStr.size(), uniStr);

		if(0 == len)
		{
			sprintf(logBuf, "utf8ToUnicode [%s] failed!", chStr.c_str());
			LogError(logBuf);
			return 0;
		}

		if(size - len <= 5)
		{
			sprintf(logBuf, "%s too long!", chStr.c_str());
			LogError(logBuf);
			return 0;
		}
		return len;
		
	}

	bool Segment::_calcDP(const ChUnicode* uniStr, size_t len, vector<pair<uint, double> >& res)
	{
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
	bool flag = segment.cutDAG(title, res);
	if(flag)
	{
		for(int i = 0; i < res.size(); i++)
		{
			cout<<res[i]<<endl;
		}
	}

	segment.destroy();
	return 0;
}

#endif
