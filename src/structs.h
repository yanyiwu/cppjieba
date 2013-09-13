#ifndef CPPJIEBA_STRUCTS_H
#define CPPJIEBA_STRUCTS_H

#include <limits>
#include "globals.h"
#include "Trie.h"
#include "TransCode.h"

namespace CppJieba
{

    struct TrieNodeInfo
    {
        //string word;
        //size_t wLen;// the word's len , not string.length(), 
        Unicode word;
        size_t freq;
        string tag;
        double logFreq; //logFreq = log(freq/sum(freq));
        TrieNodeInfo():freq(0),logFreq(0.0)
        {
        }
        TrieNodeInfo(const TrieNodeInfo& nodeInfo):word(nodeInfo.word), freq(nodeInfo.freq), tag(nodeInfo.tag), logFreq(nodeInfo.logFreq)
        {
        }
        TrieNodeInfo(const Unicode& _word):word(_word),freq(0),logFreq(MIN_DOUBLE)
        {
        }
		string toString()const
		{
            string tmp;
            TransCode::encode(word, tmp);
            return string_format("{word:%s,freq:%d, logFreq:%lf}", tmp.c_str(), freq, logFreq);
		}
    };

    typedef unordered_map<uint, const TrieNodeInfo*> DagType;
    struct SegmentChar 
    {
        uint16_t uniCh;
        DagType dag;
        const TrieNodeInfo * pInfo;
        double weight;
        
        SegmentChar(uint16_t uni):uniCh(uni), pInfo(NULL), weight(0.0)
        {
        }
        
        /*const TrieNodeInfo* pInfo;
        double weight;
        SegmentChar(uint16_t unich, const TrieNodeInfo* p, double w):uniCh(unich), pInfo(p), weight(w)
        {
        }*/
    };
    /*
    struct SegmentContext
    {
        vector<SegmentChar> context;
        bool getDA
    };*/
    typedef vector<SegmentChar> SegmentContext;
    
    //struct SegmentContext
    //{
    //    vector<SegmentChar> context;
    //    //vector<uint16_t> uintVec;
    //    //vector< vector<pair<uint, const TrieNodeInfo*> > > dag;
    //    //vector< pair<const TrieNodeInfo*, double> > dp;
    //};
    
    /*
    struct SegmentWordInfo: public TrieNodeInfo
    {
        
    };
    */

    struct KeyWordInfo: public TrieNodeInfo
    {
        double idf;
        double weight;// log(wLen+1)*logFreq;
        KeyWordInfo():idf(0.0),weight(0.0)
        {
        }
        KeyWordInfo(const Unicode& _word):TrieNodeInfo(_word),idf(0.0),weight(0.0)
        { 
        }
        KeyWordInfo(const TrieNodeInfo& trieNodeInfo):TrieNodeInfo(trieNodeInfo)
        {
        }
        inline string toString() const
        {
            string tmp;
            TransCode::encode(word, tmp);
            return string_format("{word:%s,weight:%lf, idf:%lf}", tmp.c_str(), weight, idf);
        }
        KeyWordInfo& operator = (const TrieNodeInfo& trieNodeInfo)
        {
            word = trieNodeInfo.word;
            freq = trieNodeInfo.freq;
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
