/************************************
 * file enc : ASCII
 * author   : wuyanyi09@gmail.com
 ************************************/
#ifndef CPPJIEBA_TRIE_H
#define CPPJIEBA_TRIE_H

#include <iostream>
#include <fstream>
#include <map>
#include <cstring>
#include <stdint.h>
#include <cmath>
#include <limits>
#include "Limonp/str_functs.hpp"
#include "Limonp/logger.hpp"
#include "Limonp/InitOnOff.hpp"
#include "TransCode.hpp"



namespace CppJieba
{
    using namespace Limonp;
    const double MIN_DOUBLE = -3.14e+100;
    const double MAX_DOUBLE = 3.14e+100;
    const size_t DICT_COLUMN_NUM = 3;
    typedef unordered_map<uint16_t, struct TrieNode*> TrieNodeMap;
    struct TrieNodeInfo;
    struct TrieNode
    {
        TrieNodeMap hmap;
        bool isLeaf;
        const TrieNodeInfo * ptTrieNodeInfo;
        TrieNode(): isLeaf(false), ptTrieNodeInfo(NULL)
        {}
    };

    struct TrieNodeInfo
    {
        Unicode word;
        size_t freq;
        string tag;
        double logFreq; //logFreq = log(freq/sum(freq));
        TrieNodeInfo():freq(0),logFreq(0.0)
        {}
        TrieNodeInfo(const TrieNodeInfo& nodeInfo):word(nodeInfo.word), freq(nodeInfo.freq), tag(nodeInfo.tag), logFreq(nodeInfo.logFreq)
        {}
        TrieNodeInfo(const Unicode& _word):word(_word),freq(0),logFreq(MIN_DOUBLE)
        {}
    };

    inline ostream& operator << (ostream& os, const TrieNodeInfo & nodeInfo)
    {
        return os << nodeInfo.word << ":" << nodeInfo.freq << ":" << nodeInfo.tag << ":" << nodeInfo.logFreq ;
    }

    typedef map<size_t, const TrieNodeInfo*> DagType;

    class Trie: public InitOnOff
    {

        private:
            TrieNode* _root;
            vector<TrieNodeInfo> _nodeInfos;

            int64_t _freqSum;
            double _minLogFreq;

        public:
            Trie()
            {
                _root = new TrieNode;
                _freqSum = 0;
                _minLogFreq = MAX_DOUBLE;
                _setInitFlag(false);
            }
            Trie(const string& filePath)
            {
                Trie();
                _setInitFlag(init(filePath));
            }
            ~Trie()
            {
                _deleteNode(_root);
            }
        public:
            bool init(const string& filePath)
            {
                assert(!_getInitFlag());
                _loadDict(filePath, _nodeInfos);
                _createTrie(_nodeInfos, _root);
                _freqSum = _calculateFreqSum(_nodeInfos);
                assert(_freqSum);
                _minLogFreq = _calculateLogFreqAndGetMinValue(_nodeInfos, _freqSum);
                return _setInitFlag(true);
            }

        public:
            const TrieNodeInfo* find(Unicode::const_iterator begin, Unicode::const_iterator end)const
            {
                TrieNodeMap::const_iterator citer;
                TrieNode* p = _root;
                for(Unicode::const_iterator it = begin; it != end; it++)
                {
                    uint16_t chUni = *it;
                    citer = p->hmap.find(chUni);
                    if(p-> hmap.end() == citer)
                    {
                        return NULL;
                    }
                    p = citer->second;
                }
                if(p->isLeaf)
                {
                    return p->ptTrieNodeInfo;
                }
                return NULL;
            }

            bool find(Unicode::const_iterator begin, Unicode::const_iterator end, vector<pair<size_t, const TrieNodeInfo*> >& res) const
            {
                TrieNodeMap::const_iterator citer;
                TrieNode* p = _root;
                for (Unicode::const_iterator itr = begin; itr != end; itr++)
                {
                    citer = p->hmap.find(*itr);
                    if(p->hmap.end() == citer)
                    {
                        break;
                    }
                    p = citer->second;
                    if(p->isLeaf)
                    {
                        res.push_back(make_pair(itr-begin, p->ptTrieNodeInfo));
                    }
                }
                return !res.empty();
            }

            bool find(Unicode::const_iterator begin, Unicode::const_iterator end, size_t offset, DagType & res) const
            {
                TrieNode* p = _root;
                TrieNodeMap::const_iterator citer;
                for (Unicode::const_iterator itr = begin; itr != end; itr++)
                {
                    citer = p->hmap.find(*itr);
                    if(p->hmap.end() == citer)
                    {
                        break;
                    }
                    p = citer->second;
                    if(p->isLeaf)
                    {
                        res[itr - begin + offset] = p->ptTrieNodeInfo;
                    }
                }
                return !res.empty();
            }

        public:
            double getMinLogFreq() const {return _minLogFreq;};

        private:
            void _insertNode(const TrieNodeInfo& nodeInfo, TrieNode* ptNode) const
            {
                const Unicode& unico = nodeInfo.word;
                for(size_t i = 0; i < unico.size(); i++)
                {
                    uint16_t cu = unico[i];
                    assert(ptNode);
                    if(!isIn(ptNode->hmap, cu))
                    {
                        TrieNode * next = new TrieNode;
                        assert(next);
                        ptNode->hmap[cu] = next;
                        ptNode = next;
                    }
                    else
                    {
                        ptNode = ptNode->hmap[cu];
                    }
                }

                ptNode->isLeaf = true;
                ptNode->ptTrieNodeInfo = &nodeInfo;
            }

        private:
            void _loadDict(const string& filePath, vector<TrieNodeInfo>& nodeInfos) const
            {
                ifstream ifs(filePath.c_str());
                if(!ifs)
                {
                    LogFatal("open %s failed.", filePath.c_str());
                    exit(1);
                }
                string line;
                vector<string> buf;

                nodeInfos.clear();
                TrieNodeInfo nodeInfo;
                for(size_t lineno = 0 ; getline(ifs, line); lineno++)
                {
                    split(line, buf, " ");
                    assert(buf.size() == DICT_COLUMN_NUM);
                    if(!TransCode::decode(buf[0], nodeInfo.word))
                    {
                        LogError("line[%u:%s] illegal.", lineno, line.c_str());
                        continue;
                    }
                    nodeInfo.freq = atoi(buf[1].c_str());
                    nodeInfo.tag = buf[2];
                    
                    nodeInfos.push_back(nodeInfo);
                }
            }
            bool _createTrie(const vector<TrieNodeInfo>& nodeInfos, TrieNode * ptNode)
            {
                for(size_t i = 0; i < _nodeInfos.size(); i++)
                {
                    _insertNode(_nodeInfos[i], ptNode);
                }
                return true;
            }
            size_t _calculateFreqSum(const vector<TrieNodeInfo>& nodeInfos) const
            {
                size_t freqSum = 0;
                for(size_t i = 0; i < nodeInfos.size(); i++)
                {
                    freqSum += nodeInfos[i].freq;
                }
                return freqSum;
            }
            double _calculateLogFreqAndGetMinValue(vector<TrieNodeInfo>& nodeInfos, size_t freqSum) const
            {
                assert(freqSum);
                double minLogFreq = MAX_DOUBLE;
                for(size_t i = 0; i < nodeInfos.size(); i++)
                {
                    TrieNodeInfo& nodeInfo = nodeInfos[i];
                    assert(nodeInfo.freq);
                    nodeInfo.logFreq = log(double(nodeInfo.freq)/double(freqSum));
                    if(minLogFreq > nodeInfo.logFreq)
                    {
                        minLogFreq = nodeInfo.logFreq;
                    }
                }
                return minLogFreq;
            }

            void _deleteNode(TrieNode* node)
            {
                if(!node)
                {
                    return;
                }
                for(TrieNodeMap::iterator it = node->hmap.begin(); it != node->hmap.end(); it++)
                {
                    TrieNode* next = it->second;
                    _deleteNode(next);
                }
                delete node;
            }

    };
}

#endif
