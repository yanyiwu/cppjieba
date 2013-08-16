/************************************
 * file enc : ASCII
 * author   : wuyanyi09@gmail.com
************************************/
#include "KeyWordExt.h"


namespace CppJieba
{

	KeyWordExt::KeyWordExt()
	{
	}
	
	KeyWordExt::~KeyWordExt()
	{
	}

	bool KeyWordExt::init()
	{
		return _segment.init();
	}

	bool KeyWordExt::loadSegDict(const string& filePath)
	{
		return _segment.loadSegDict(filePath);
	}

	bool KeyWordExt::loadPriorSubWords(const string& filePath)
	{
		LogInfo(string_format("loadPriorSubWords(%s) start", filePath.c_str()));
		if(!checkFileExist(filePath.c_str()))
		{
			LogError(string_format("cann't find file[%s].",filePath.c_str()));
			return false;
		}
		if(!_priorSubWords.empty())
		{
			LogError("_priorSubWords has been initted before");
			return false;
		}
		ifstream infile(filePath.c_str());
		string subword;
		while(getline(infile, subword))
		{
			_priorSubWords.push_back(subword);
		}
		LogInfo(string_format("loadPriorSubWords(%s) end", filePath.c_str()));
		infile.close();
		return true;
	}

	bool KeyWordExt::loadStopWords(const string& filePath)
	{

		LogInfo(string_format("loadStopWords(%s) start", filePath.c_str()));
		if(!_stopWords.empty())
		{
			LogError("_stopWords has been loaded before! ");
			return false;
		}
		if(!checkFileExist(filePath.c_str()))
		{
			LogError(string_format("cann't find file[%s].",filePath.c_str()));
			return false;
		}

		ifstream ifile(filePath.c_str());
		string line;
		while(getline(ifile, line))
		{
			_stopWords.insert(line);
		}
		LogInfo(string_format("load stopwords[%d] finished.", _stopWords.size()));
		
		return true;
	}
	
	bool KeyWordExt::dispose()
	{
		_segment.dispose();
		return true;
	}

	bool KeyWordExt::_wordInfoCompare(const WordInfo& a, const WordInfo& b)
	{
		return a.weight > b.weight;
	}

	bool KeyWordExt::_sortWLIDF(vector<WordInfo>& wordInfos)
	{
		//size_t wLenSum = 0;
		for(uint i = 0; i < wordInfos.size(); i++)
		{
			wordInfos[i].wLen = TransCode::getWordLength(wordInfos[i].word);
			if(0 == wordInfos[i].wLen)
			{
				LogFatal("wLen is 0");
				return false;
			}
			//wLenSum += wordInfos[i].wLen;
		}

		/*
		if(0 == wLenSum)
		{
			LogFatal("wLenSum == 0.");
			return false;
		}*/

		for(uint i = 0; i < wordInfos.size(); i++)
		{
			WordInfo& wInfo = wordInfos[i];
			double logWordFreq = _segment.getWordWeight(wInfo.word);
			wInfo.idf = -logWordFreq;
			size_t wLen = TransCode::getWordLength(wInfo.word);
			if(0 == wLen)
			{
				LogFatal("getUtf8WordLen(%s) return 0");
			}
			wInfo.weight = log(double(wLen + 1)) * wInfo.idf;
		}
		sort(wordInfos.begin(), wordInfos.end(), _wordInfoCompare);
		return true;
	}

	bool KeyWordExt::_extractTopN(const vector<string>& words, vector<string>& keywords, uint topN)
	{
		keywords.clear();
		vector<WordInfo> wordInfos;
		for(uint i = 0; i < words.size(); i++)
		{
			WordInfo wInfo;
			wInfo.word = words[i];
			wordInfos.push_back(wInfo);
		}
		
		_sortWLIDF(wordInfos);
#ifdef DEBUG
		LogDebug(string_format("calc weight & sorted:%s",joinWordInfos(wordInfos).c_str()));
#endif
		
		_prioritizeSubWords(wordInfos);
#ifdef DEBUG
		LogDebug(string_format("_prioritizeSubWords res:%s", joinWordInfos(wordInfos).c_str()));
#endif
		//extract TopN
		for(uint i = 0; i < topN && i < wordInfos.size(); i++)
		{
			keywords.push_back(wordInfos[i].word);
		}
		return true;
	}


	bool KeyWordExt::extract(const vector<string>& _words, vector<string>& keywords, uint topN)
	{
		if(_words.empty())
		{
			return false;
		}

		vector<string> words(_words);

#ifdef DEBUG
		LogDebug(string_format("words:[%s]", joinStr(words, ",").c_str()));
#endif

		bool retFlag = _filter(words);
		if(!retFlag)
		{
			LogError("_filter failed.");
			return false;
		}

#ifdef DEBUG
		LogDebug(string_format("_filter res:[%s]", joinStr(words, ",").c_str()));
#endif

		retFlag = _extractTopN(words, keywords, topN);
		if(!retFlag)
		{
			LogError("_extractTopN failed.");
			return false;
		}
		//LogDebug("_extractTopN finished.");

#ifdef DEBUG
		LogDebug(string_format("ext res:[%s]", joinStr(keywords, ",").c_str()));
#endif
		
		return true;
	}

	bool KeyWordExt::extract(const string& title, vector<string>& keywords, uint topN)
	{
		if(title.empty())
		{
			return false;
		}

#ifdef DEBUG
		LogDebug(string_format("title:[%s]",title.c_str()));
#endif

		bool retFlag;
		vector<string> words;
		retFlag = _segment.cutDAG(title, words);
		if(!retFlag)
		{
			LogError(string_format("cutDAG(%s) failed.", title.c_str()));
			return false;
		}
#ifdef DEBUG
		LogDebug(string_format("cutDAG result:[%s]", joinStr(words, ",").c_str()));
#endif

		retFlag = _filter(words);
		if(!retFlag)
		{
			LogError("_filter failed.");
			return false;
		}

#ifdef DEBUG
		LogDebug(string_format("_filter res:[%s]", joinStr(words, ",").c_str()));
#endif

		retFlag = _extractTopN(words, keywords, topN);
		if(!retFlag)
		{
			LogError("_extractTopN failed.");
			return false;
		}
		//LogDebug("_extractTopN finished.");

#ifdef DEBUG
		LogDebug(string_format("ext res:[%s]", joinStr(keywords, ",").c_str()));
#endif
		return true;
	}

	bool KeyWordExt::_filter(vector<string>& strs)
	{
		bool retFlag;
		retFlag = _filterDuplicate(strs);
		if(!retFlag)
		{
			LogError("_filterDuplicate failed.");
			return false;
		}
		//LogDebug(string_format("_filterDuplicate res:[%s]", joinStr(strs, ",").c_str()));

		retFlag = _filterSingleWord(strs);
		if(!retFlag)
		{
			LogError("_filterSingleWord failed.");
			return false;
		}
		//LogDebug(string_format("_filterSingleWord res:[%s]", joinStr(strs, ",").c_str()));

		retFlag = _filterStopWords(strs);
		if(!retFlag)
		{
			LogError("_filterStopWords failed.");
			return false;
		}
		//LogDebug(string_format("_filterStopWords res:[%s]", joinStr(strs, ",").c_str()));

		retFlag = _filterSubstr(strs);
		if(!retFlag)
		{
			LogError("_filterSubstr failed.");
			return false;
		}
		//LogDebug(string_format("_filterSubstr res:[%s]", joinStr(strs, ",").c_str()));

		return true;
	}

	bool KeyWordExt::_filterStopWords(vector<string>& strs)
	{
		if(_stopWords.empty())
		{
			return true;
		}
		for(VSI it = strs.begin(); it != strs.end();)
		{
			if(_stopWords.find(*it) != _stopWords.end())
			{
				it = strs.erase(it);
			}
			else
			{
				it ++;
			}
		}
		return true;
	}


	bool KeyWordExt::_filterDuplicate(vector<string>& strs)
	{
		set<string> st;
		for(VSI it = strs.begin(); it != strs.end(); )
		{
			if(st.find(*it) != st.end())
			{
				it = strs.erase(it);
			}
			else
			{
				st.insert(*it);
				it++;
			}
		}
		return true;
	}

	bool KeyWordExt::_filterSingleWord(vector<string>& strs)
	{
		for(vector<string>::iterator it = strs.begin(); it != strs.end();)
		{

			// filter single word
			if(1 == TransCode::getWordLength(*it)) 
			{
				it = strs.erase(it);
			}
			else
			{
				it++;
			}
		}
		return true;
	}

	bool KeyWordExt::_filterSubstr(vector<string>& strs)
	{
		vector<string> tmp = strs;
		set<string> subs;
		for(VSI it = strs.begin(); it != strs.end(); it ++)
		{
			for(uint j = 0; j < tmp.size(); j++)
			{
				if(*it != tmp[j] && string::npos != tmp[j].find(*it, 0))
				{
					subs.insert(*it);
				}
			}
		}

		//erase subs from strs
		for(VSI it = strs.begin(); it != strs.end(); )
		{
			if(subs.end() != subs.find(*it))
			{
				it =  strs.erase(it);
			}
			else
			{
				it ++;
			}
		}
		return true;
	}

	bool KeyWordExt::_isContainSubWords(const string& word)
	{
		for(uint i = 0; i < _priorSubWords.size(); i++)
		{
			if(string::npos != word.find(_priorSubWords[i]))
			{
				return true;
			}
		}
		return false;
	}

	bool KeyWordExt::_prioritizeSubWords(vector<WordInfo>& wordInfos)
	{
		if(2 > wordInfos.size())
		{
			return true;
		}

		WordInfo prior;
		bool flag = false;
		for(vector<WordInfo>::iterator it = wordInfos.begin(); it != wordInfos.end(); )
		{
			if(_isContainSubWords(it->word))
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
	ext.init();
	if(!ext.loadSegDict("../dicts/segdict.gbk.v2.1"))
	{
		return 1;
	}
	ext.loadStopWords("../dicts/stopwords.gbk.v1.0");

	if(!ext.loadPriorSubWords("../dicts/prior.gbk"))
	{
		cerr<<"err"<<endl;
		return 1;
	}

	ifstream ifile("testtitle.gbk");
	vector<string> res;
	string line;
	while(getline(ifile, line))
	{
		cout<<line<<endl;
		res.clear();
		ext.extract(line, res, 20);
		PRINT_VECTOR(res);
	}

	ext.dispose();
	return 0;
}

#endif
