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

	bool KeyWordExt::init(const char* const segDictFile, const char* const stopWordDictFile)
	{
		LogInfo("KeyWordExt init start ...");
		if(!_segment.init(segDictFile))
		{
			LogError("_segment.init failed.");
			return false;
		}
		if(!_loadStopWords(stopWordDictFile))
		{
			LogError("_loadStopWords failed.");
			return false;
		}
		LogInfo("KeyWordExt init OK.");
		return true;
	}

	bool KeyWordExt::_loadPriorSubWords(const char * const filePath)
	{
		LogInfo(string_format("_loadPriorSubWords(%s) start", filePath));
		if(!checkFileExist(filePath))
		{
			LogError(string_format("cann't find file[%s].",filePath));
			return false;
		}
		if(!_priorSubWords.empty())
		{
			LogError("_priorSubWords has been initted before");
			return false;
		}
		ifstream infile(filePath);
		string subword;
		while(getline(infile, subword))
		{
			_priorSubWords.push_back(subword);
		}
		LogInfo(string_format("_loadPriorSubWords(%s) end", filePath));
		infile.close();
		return true;
	}

	bool KeyWordExt::_loadStopWords(const char * const filePath)
	{

		LogInfo(string_format("_loadStopWords(%s) start", filePath));
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
	
	bool KeyWordExt::dispose()
	{
		_segment.dispose();
		return true;
	}

	bool KeyWordExt::_wordInfoCompare(const KeyWordInfo& a, const KeyWordInfo& b)
	{
		return a.weight > b.weight;
	}

	bool KeyWordExt::_sortWLIDF(vector<KeyWordInfo>& wordInfos)
	{
		for(uint i = 0; i < wordInfos.size(); i++)
		{
			KeyWordInfo& wInfo = wordInfos[i];
			wInfo.idf = - wInfo.logFreq;
			if(0 == wInfo.wLen)
			{
				LogFatal("wLen is 0!");
				return false;
			}
			wInfo.weight = log(double(wInfo.wLen + 1)) * wInfo.idf;
		}
		sort(wordInfos.begin(), wordInfos.end(), _wordInfoCompare);
		return true;
	}

	bool KeyWordExt::_extTopN(vector<KeyWordInfo>& wordInfos, uint topN)
	{
		int dis = wordInfos.size() - topN;
		if(dis <= 0)
		{
			return true;
		}
		
		if(uint(dis) <= topN)
		{
			for(int i = 0; i< dis; i++)
			{
				wordInfos.pop_back();
			}
		}
		else// in case that topN << size;
		{
			
			vector<KeyWordInfo> tmp(wordInfos.begin(), wordInfos.begin() + topN);
			wordInfos.swap(tmp);
		}
		return true;
	}


	bool KeyWordExt::extract(const vector<string>& words, vector<KeyWordInfo>& keyWordInfos, uint topN)
	{
		if(words.empty())
		{
			return false;
		}

#ifdef DEBU
		LogDebug(string_format("words:[%s]", joinStr(words, ",").c_str()));
#endif

		keyWordInfos.clear();
		for(uint i = 0; i < words.size(); i++)
		{
			keyWordInfos.push_back(words[i]);
		}

		return _extract(keyWordInfos, topN);
	}

	bool KeyWordExt::extract(const string& title, vector<KeyWordInfo>& keyWordInfos, uint topN)
	{
		if(title.empty())
		{
			return false;
		}
		
		vector<TrieNodeInfo> trieNodeInfos; 
		_segment.cutDAG(title, trieNodeInfos);

		keyWordInfos.clear();
		for(uint i = 0; i < trieNodeInfos.size(); i++)
		{
			keyWordInfos.push_back(trieNodeInfos[i]);
		}
		return _extract(keyWordInfos, topN);
	}

	bool KeyWordExt::_extract(vector<KeyWordInfo>& keyWordInfos, uint topN)
	{
		if(!_filter(keyWordInfos))
		{
			LogError("_filter failed.");
			return false;
		}

		if(!_sortWLIDF(keyWordInfos))
		{
			LogError("_sortWLIDF failed.");
			return false;
		}

		if(!_extTopN(keyWordInfos, topN))
		{
			LogError("_extTopN failed.");
			return false;
		}

		return true;
	}

	bool KeyWordExt::_filter(vector<KeyWordInfo>& wordInfos)
	{
		if(!_filterDuplicate(wordInfos))
		{
			LogError("_filterDuplicate failed.");
			return false;
		}

		if(!_filterSingleWord(wordInfos))
		{
			LogError("_filterSingleWord failed.");
			return false;
		}

		if(!_filterStopWords(wordInfos))
		{
			LogError("_filterStopWords failed.");
			return false;
		}

		if(!_filterSubstr(wordInfos))
		{
			LogError("_filterSubstr failed.");
			return false;
		}

		return true;
	}

	bool KeyWordExt::_filterStopWords(vector<KeyWordInfo>& wordInfos)
	{
		if(_stopWords.empty())
		{
			return true;
		}
		for(vector<KeyWordInfo>::iterator it = wordInfos.begin(); it != wordInfos.end();)
		{
			if(_stopWords.find(it->word) != _stopWords.end())
			{
				it = wordInfos.erase(it);
			}
			else
			{
				it ++;
			}
		}
		return true;
	}


	bool KeyWordExt::_filterDuplicate(vector<KeyWordInfo>& wordInfos)
	{
		set<string> st;
		for(vector<KeyWordInfo>::iterator it = wordInfos.begin(); it != wordInfos.end(); )
		{
			if(st.find(it->word) != st.end())
			{
				it = wordInfos.erase(it);
			}
			else
			{
				st.insert(it->word);
				it++;
			}
		}
		return true;
	}

	bool KeyWordExt::_filterSingleWord(vector<KeyWordInfo>& wordInfos)
	{
		for(vector<KeyWordInfo>::iterator it = wordInfos.begin(); it != wordInfos.end();)
		{

			// filter single word
			if(1 == it->wLen)
			{
				it = wordInfos.erase(it);
			}
			else
			{
				it++;
			}
		}
		return true;
	}

	bool KeyWordExt::_filterSubstr(vector<KeyWordInfo>& wordInfos)
	{
		vector<string> tmp ;
		for(uint i = 0; i < wordInfos.size(); i++)
		{
			tmp.push_back(wordInfos[i].word);
		}
		set<string> subs;
		for(vector<KeyWordInfo>::iterator it = wordInfos.begin(); it != wordInfos.end(); it ++)
		{
			for(uint j = 0; j < tmp.size(); j++)
			{
				if(it->word != tmp[j] && string::npos != tmp[j].find(it->word, 0))
				{
					subs.insert(it->word);
				}
			}
		}

		//erase subs from strs
		for(vector<KeyWordInfo>::iterator it = wordInfos.begin(); it != wordInfos.end(); )
		{
			if(subs.end() != subs.find(it->word))
			{
				it =  wordInfos.erase(it);
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

	bool KeyWordExt::_prioritizeSubWords(vector<KeyWordInfo>& wordInfos)
	{
		if(2 > wordInfos.size())
		{
			return true;
		}

		KeyWordInfo prior;
		bool flag = false;
		for(vector<KeyWordInfo>::iterator it = wordInfos.begin(); it != wordInfos.end(); )
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
	ext._loadStopWords("../dicts/stopwords.gbk.v1.0");

	if(!ext._loadPriorSubWords("../dicts/prior.gbk"))
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
