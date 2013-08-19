#ifndef CPPJIEBA_STRUCTS_H
#define CPPJIEBA_STRUCTS_H

#include <limits>
#include "globals.h"
#include "Trie.h"

namespace CppJieba
{

	struct TrieNodeInfo
	{
		string word;
		size_t wLen;// the word's len , not string.length(), 
		size_t freq;
		string tag;
		double logFreq; //logFreq = log(freq/sum(freq));
		TrieNodeInfo()
		{
			wLen = 0;
			freq = 0;
			logFreq = 0.0;
		}
		TrieNodeInfo(const string& _word)
		{
			word = _word;
			wLen = TransCode::getWordLength(_word);
			freq = 0;
			logFreq = -numeric_limits<double>::max();
		}
	};
	
	
	struct SegmentContext//: public TrieNodeInfo
	{
		vector<uint16_t> uintVec;
		vector< vector<pair<uint, const TrieNodeInfo*> > > dag;
		vector< pair<const TrieNodeInfo*, double> > dp;
		//vector<string> words;
	};
	
	/*
	struct SegmentWordInfo: public TrieNodeInfo
	{
		
	};
	*/

	struct KeyWordInfo: public TrieNodeInfo
	{
		double idf;
		double weight;// log(wLen+1)*logFreq;
		KeyWordInfo()
		{
			idf = 0.0;
			weight = 0.0;
		}
		KeyWordInfo(const string& _word):TrieNodeInfo(_word)
		{ 
			idf = 0.0;
			weight = 0.0;
		}
		KeyWordInfo(const TrieNodeInfo& trieNodeInfo)
		{
			word = trieNodeInfo.word;
			freq = trieNodeInfo.freq;
			wLen = trieNodeInfo.wLen;
			tag = trieNodeInfo.tag;
			logFreq = trieNodeInfo.logFreq;
			idf = 0.0;
			weight = 0.0;
		}
		string toString() const
		{
			return string_format("{word:%s,wLen:%d weight:%lf, idf:%lf}", word.c_str(), wLen, weight, idf);
		}
		KeyWordInfo& operator = (const TrieNodeInfo& trieNodeInfo)
		{
			word = trieNodeInfo.word;
			freq = trieNodeInfo.freq;
			wLen = trieNodeInfo.wLen;
			tag = trieNodeInfo.tag;
			logFreq = trieNodeInfo.logFreq;
			return *this;
		}
	};

	inline string joinWordInfos(const vector<KeyWordInfo>& vec)
	{
		vector<string> tmp;
		for(uint i = 0; i < vec.size(); i++)
		{
			tmp.push_back(vec[i].toString());
		}
		return joinStr(tmp, ",");
	}
}

#endif
