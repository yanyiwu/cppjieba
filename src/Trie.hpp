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
    struct TrieNode
    {
        TrieNodeMap hmap;
        bool isLeaf;
        size_t nodeInfoVecPos;
        TrieNode()
        {
            isLeaf = false;
            nodeInfoVecPos = 0;
        }
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
            vector<TrieNodeInfo> _nodeInfoVec;

            int64_t _freqSum;
            double _minLogFreq;

        public:
            Trie()
            {
                _root = NULL;
                _freqSum = 0;
                _minLogFreq = MAX_DOUBLE;
                _setInitFlag(false);
            }
            Trie(const string& filePath): Trie()
            {
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

                _root = new TrieNode;
                assert(_root);
                if(!_trieInsert(filePath))
                {
                    LogError("_trieInsert failed.");
                    return false;
                }
                if(!_countWeight())
                {
                    LogError("_countWeight failed.");
                    return false;
                }
                return _setInitFlag(true);
            }

        public:
            const TrieNodeInfo* find(Unicode::const_iterator begin, Unicode::const_iterator end)const
            {
                TrieNode* p = _root;
                for(Unicode::const_iterator it = begin; it != end; it++)
                {
                    uint16_t chUni = *it;
                    if(p->hmap.find(chUni) == p-> hmap.end())
                    {
                        return NULL;
                    }
                    else
                    {
                        p = p->hmap[chUni];
                    }
                }
                if(p->isLeaf)
                {
                    size_t pos = p->nodeInfoVecPos;
                    if(pos < _nodeInfoVec.size())
                    {
                        return &(_nodeInfoVec[pos]);
                    }
                    else
                    {
                        LogFatal("node's nodeInfoVecPos is out of _nodeInfoVec's range");
                        return NULL;
                    }
                }
                return NULL;
            }

            bool find(Unicode::const_iterator begin, Unicode::const_iterator end, vector<pair<size_t, const TrieNodeInfo*> >& res) const
            {
                TrieNode* p = _root;
                for (Unicode::const_iterator itr = begin; itr != end; itr++)
                {
                    if(p->hmap.find(*itr) == p-> hmap.end())
                    {
                        break;
                    }
                    p = p->hmap[*itr];
                    if(p->isLeaf)
                    {
                        size_t pos = p->nodeInfoVecPos;
                        if(pos < _nodeInfoVec.size())
                        {
                            res.push_back(make_pair(itr-begin, &_nodeInfoVec[pos]));
                        }
                        else
                        {
                            LogFatal("node's nodeInfoVecPos is out of _nodeInfoVec's range");
                            return false;
                        }
                    }
                }
                return !res.empty();
            }

            bool find(Unicode::const_iterator begin, Unicode::const_iterator end, size_t offset, DagType & res) const
            {
                TrieNode* p = _root;
                for (Unicode::const_iterator itr = begin; itr != end; itr++)
                {
                    if(p->hmap.find(*itr) == p-> hmap.end())
                    {
                        break;
                    }
                    p = p->hmap[*itr];
                    if(p->isLeaf)
                    {
                        size_t pos = p->nodeInfoVecPos;
                        if(pos < _nodeInfoVec.size())
                        {
                            res[itr-begin + offset] = &_nodeInfoVec[pos];
                        }
                        else
                        {
                            LogFatal("node's nodeInfoVecPos is out of _nodeInfoVec's range");
                            return false;
                        }
                    }
                }
                return !res.empty();
            }

        public:
            double getMinLogFreq() const {return _minLogFreq;};

        private:
            void _insert(const TrieNodeInfo& nodeInfo)
            {

                const Unicode& uintVec = nodeInfo.word;
                TrieNode* p = _root;
                for(size_t i = 0; i < uintVec.size(); i++)
                {
                    uint16_t cu = uintVec[i];
                    assert(p);
                    if(p->hmap.end() == p->hmap.find(cu))
                    {
                        TrieNode * next = new TrieNode;
                        assert(next);
                        p->hmap[cu] = next;
                        p = next;
                    }
                    else
                    {
                        p = p->hmap[cu];
                    }
                }
                assert(p);
                assert(!p->isLeaf);

                p->isLeaf = true;
                _nodeInfoVec.push_back(nodeInfo);
                p->nodeInfoVecPos = _nodeInfoVec.size() - 1;

            }

        private:
            bool _trieInsert(const string& filePath)
            {
                ifstream ifs(filePath.c_str());
                if(!ifs)
                {
                    LogError("open %s failed.", filePath.c_str());
                    return false;
                }
                string line;
                vector<string> vecBuf;

                TrieNodeInfo nodeInfo;
                for(size_t lineno = 0 ; getline(ifs, line); lineno++)
                {
                    split(line, vecBuf, " ");
                    assert(vecBuf.size() == DICT_COLUMN_NUM);
                    if(!TransCode::decode(vecBuf[0], nodeInfo.word))
                    {
                        LogError("line[%u:%s] illegal.", lineno, line.c_str());
                        return false;
                    }
                    nodeInfo.freq = atoi(vecBuf[1].c_str());
                    nodeInfo.tag = vecBuf[2];

                    _insert(nodeInfo);
                }
                return true;
            }
            bool _countWeight()
            {
                if(_nodeInfoVec.empty() || 0 != _freqSum)
                {
                    LogError("_nodeInfoVec is empty or _freqSum has been counted already.");
                    return false;
                }

                //freq total freq
                for(size_t i = 0; i < _nodeInfoVec.size(); i++)
                {
                    _freqSum += _nodeInfoVec[i].freq;
                }

                assert(_freqSum);

                //normalize
                for(size_t i = 0; i < _nodeInfoVec.size(); i++)
                {
                    TrieNodeInfo& nodeInfo = _nodeInfoVec[i];
                    assert(nodeInfo.freq);
                    nodeInfo.logFreq = log(double(nodeInfo.freq)/double(_freqSum));
                    if(_minLogFreq > nodeInfo.logFreq)
                    {
                        _minLogFreq = nodeInfo.logFreq;
                    }
                }

                return true;
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
