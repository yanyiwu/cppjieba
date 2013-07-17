#include "KeyWordExt.h"


namespace CppJieba
{

	KeyWordExt::KeyWordExt()
	{
	}
	
	KeyWordExt::~KeyWordExt()
	{
	}

	bool KeyWordExt::init(const char * const filePath)
	{
		LogInfo(string_format("init(%s) start", filePath));
		if(!checkFileExist(filePath))
		{
			LogError(string_format("cann't find file[%s].",filePath));
			return false;
		}
		bool retFlag = _segment.init(filePath);
		LogInfo(string_format("init(%s) end", filePath));
		return retFlag;

	}

	bool KeyWordExt::loadPriorWordPrefixes(const char * const filePath)
	{
		LogInfo(string_format("loadPriorWordPrefixes(%s) start", filePath));
		if(!checkFileExist(filePath))
		{
			LogError(string_format("cann't find file[%s].",filePath));
			return false;
		}
		bool retFlag = _priorPrefixTrie.init(filePath);
		if(!retFlag)
		{
			LogError("_priorPrefixTrie.init return false.");
			return false;
		}
		LogInfo(string_format("loadPriorWordPrefixes(%s) end", filePath));
		return true;
	}

	bool KeyWordExt::loadStopWords(const char * const filePath)
	{

		LogInfo(string_format("loadStopWords(%s) start", filePath));
		if(!_stopWords.empty())
		{
			LogError("_stopWords has been loaded before! ");
			return false;
		}
		if(!checkFileExist(filePath))
		{
			LogError(string_format("cann't find file[%s].",filePath));
			return false;
		}

		ifstream ifile(filePath);
		string line;
		while(getline(ifile, line))
		{
			_stopWords.insert(line);
		}
		LogInfo(string_format("load stopwords[%d] finished.", _stopWords.size()));
		
		return true;
	}
	
	bool KeyWordExt::destroy()
	{
		_segment.destroy();
		_priorPrefixTrie.destroy();
		return true;
	}

	bool KeyWordExt::_wordInfoCompare(const WordInfo& a, const WordInfo& b)
	{
		return a.weight < b.weight;
	}

	bool KeyWordExt::_extractTopN(const vector<string>& words, vector<string>& keywords, uint topN)
	{
		keywords.clear();
		vector<WordInfo> wordInfos;
		for(uint i = 0; i < words.size(); i++)
		{
			double w = _segment.getUtf8WordWeight(words[i]);
			WordInfo wInfo;
			wInfo.word = words[i];
			wInfo.weight = w;
			wInfo.idf = w;
			wordInfos.push_back(wInfo);
		}
		sort(wordInfos.begin(), wordInfos.end(), _wordInfoCompare);
		LogDebug(string_format("calc weight & sorted:\n%s",joinWordInfos(wordInfos).c_str()));
		
		_priorWordPrefixes(wordInfos);
		LogDebug(string_format("_priorWordPrefixes res:\n%s", joinWordInfos(wordInfos).c_str()));
		//extract TopN
		for(uint i = 0; i < topN && i < wordInfos.size(); i++)
		{
			keywords.push_back(wordInfos[i].word);
		}
		return true;
	}

	bool KeyWordExt::extract(const string& utf8Str, vector<string>& keywords, uint topN)
	{
		LogInfo(string_format("title:[%s]",utf8Str.c_str()));

		bool retFlag;
		vector<string> words;
		retFlag = _segment.cutDAG(utf8Str, words);
		if(!retFlag)
		{
			LogError(string_format("cutDAG(%s) failed.", utf8Str.c_str()));
			return false;
		}
		LogDebug(string_format("cutDAG result:[%s]", joinStr(words, ",").c_str()));

		retFlag = _filter(words);
		if(!retFlag)
		{
			LogError("_filter failed.");
			return false;
		}
		LogDebug(string_format("_filter res:[%s]", joinStr(words, ",").c_str()));

		retFlag = _extractTopN(words, keywords, topN);
		if(!retFlag)
		{
			LogError("_extractTopN failed.");
			return false;
		}
		LogDebug("_extractTopN finished.");

		LogInfo(string_format("ext res:[%s]", joinStr(keywords, ",").c_str()));
		return true;
	}

	bool KeyWordExt::_filter(vector<string>& utf8Strs)
	{
		bool retFlag;
		retFlag = _filterDuplicate(utf8Strs);
		if(!retFlag)
		{
			LogError("_filterDuplicate failed.");
			return false;
		}
		LogDebug(string_format("_filterDuplicate res:[%s]", joinStr(utf8Strs, ",").c_str()));

		retFlag = _filterSingleWord(utf8Strs);
		if(!retFlag)
		{
			LogError("_filterSingleWord failed.");
			return false;
		}
		LogDebug(string_format("_filterSingleWord res:[%s]", joinStr(utf8Strs, ",").c_str()));

		retFlag = _filterStopWords(utf8Strs);
		if(!retFlag)
		{
			LogError("_filterStopWords failed.");
			return false;
		}
		LogDebug(string_format("_filterStopWords res:[%s]", joinStr(utf8Strs, ",").c_str()));

		retFlag = _filterSubstr(utf8Strs);
		if(!retFlag)
		{
			LogError("_filterSubstr failed.");
			return false;
		}
		LogDebug(string_format("_filterSubstr res:[%s]", joinStr(utf8Strs, ",").c_str()));

		return true;
	}

	bool KeyWordExt::_filterStopWords(vector<string>& utf8Strs)
	{
		if(_stopWords.empty())
		{
			return true;
		}
		for(VSI it = utf8Strs.begin(); it != utf8Strs.end();)
		{
			if(_stopWords.find(*it) != _stopWords.end())
			{
				it = utf8Strs.erase(it);
			}
			else
			{
				it ++;
			}
		}
		return true;
	}


	bool KeyWordExt::_filterDuplicate(vector<string>& utf8Strs)
	{
		set<string> st;
		for(VSI it = utf8Strs.begin(); it != utf8Strs.end(); )
		{
			if(st.find(*it) != st.end())
			{
				it = utf8Strs.erase(it);
			}
			else
			{
				st.insert(*it);
				it++;
			}
		}
		return true;
	}

	bool KeyWordExt::_filterSingleWord(vector<string>& utf8Strs)
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

	bool KeyWordExt::_filterSubstr(vector<string>& utf8Strs)
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

	bool KeyWordExt::_priorWordPrefixes(vector<WordInfo>& wordInfos)
	{
		if(2 > wordInfos.size())
		{
			return true;
		}

		WordInfo prior;
		bool flag = false;
		for(vector<WordInfo>::iterator it = wordInfos.begin(); it != wordInfos.end(); )
		{
			if(NULL != _priorPrefixTrie.findPrefix(it->word))
			{
				prior = *it;
				it = wordInfos.erase(it);
				flag = true;
				break;
			}
			else
			{
				it ++;
			}
		}
		if(flag)
		{
			wordInfos.insert(wordInfos.begin(), prior);
		}
		return true;
	}
}


#ifdef KEYWORDEXT_UT

using namespace CppJieba;

int main()
{
	KeyWordExt ext;
	if(!ext.init("../dicts/segdict.utf8.v2.1"))
	{
		return 1;
	}
	ext.loadStopWords("stopwords.tmp");

	if(!ext.loadPriorWordPrefixes("prior.utf8"))
	{
		cerr<<"err"<<endl;
		return 1;
	}
	//segment.init("dicts/jieba.dict.utf8");
	
	vector<string> res;
	string title;
	/*title = "我来到北京清华大学";
	res.clear();
	ext.extract(title, res, 5);
	PRINT_VECTOR(res);
	
	
	title = "特价！camel骆驼 柔软舒适头层牛皮平底凉鞋女 休闲平跟妈妈鞋夏";
	res.clear();
	ext.extract(title, res, 5);
	PRINT_VECTOR(res);
	*/


	title = "2013新款19CM超大檐帽 遮阳草帽子 沙滩帽防晒大檐欧美新款夏天女装";
	res.clear();
	ext.extract(title, res, 5);
	PRINT_VECTOR(res);

	ext.destroy();
	return 0;
}

#endif
