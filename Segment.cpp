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

	bool Segment::cutMM(const string& chStr, vector<string>& res)
	{
		res.clear();
		char logBuf[bufSize];
		char utfBuf[bufSize];
		ChUnicode uniStr[bufSize];
		memset(uniStr, 0, sizeof(uniStr));
		size_t len = utf8ToUnicode(chStr.c_str(), chStr.size(), uniStr);

		if(0 == len)
		{
			sprintf(logBuf, "utf8ToUnicode [%s] failed!", chStr.c_str());
			LogError(logBuf);
			return false;
		}

		if(sizeof(uniStr) - len <= 5)
		{
			sprintf(logBuf, "%s too long!", chStr.c_str());
			LogError(logBuf);
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
		char logBuf[bufSize];
		char utfBuf[bufSize];
		ChUnicode uniStr[bufSize];
		memset(uniStr, 0, sizeof(uniStr));
		size_t len = utf8ToUnicode(chStr.c_str(), chStr.size(), uniStr);

		if(0 == len)
		{
			sprintf(logBuf, "utf8ToUnicode [%s] failed!", chStr.c_str());
			LogError(logBuf);
			return false;
		}

		if(sizeof(uniStr) - len <= 5)
		{
			sprintf(logBuf, "%s too long!", chStr.c_str());
			LogError(logBuf);
			return false;
		}

		int i = len - 1;
		while(i >= 0)
		{
			bool flag = false;
			for(int j = 0; j <= i; j++)
			{
				size_t uniLen = i - j + 1;
				if(_trie.find(uniStr + j, uniLen))
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
}


#ifdef SEGMENT_UT
using namespace CppJieba;

int main()
{
	Segment segment;
	segment.init("dict.utf8");
	vector<string> res;
	string title = "我来到北京清华大学3D电视";
	bool flag = segment.cutMM(title, res);
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
