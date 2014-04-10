#ifndef CPPJIEBA_DICT_TRIE_HPP
#define CPPJIEBA_DICT_TRIE_HPP

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
#include "DictTrie.hpp"



namespace CppJieba
{
    using namespace Limonp;
    const double MIN_DOUBLE = -3.14e+100;
    const double MAX_DOUBLE = 3.14e+100;
    const size_t DICT_COLUMN_NUM = 3;


    struct DictUnit
    {
        Unicode word;
        size_t freq;
        string tag;
        double logFreq; //logFreq = log(freq/sum(freq));
    };

    typedef map<size_t, const DictUnit*> DagType;

    class DictTrie: InitOnOff
    {

        private:
            DictTrieNode* _root;
            vector<DictUnit> _nodeInfos;

            int64_t _freqSum;
            double _minLogFreq;

        public:
            DictTrie()
            {
                _root = new DictTrieNode;
                _root.ptKeyMap = NULL;
                _root.offset = 0;
                _freqSum = 0;
                _minLogFreq = MAX_DOUBLE;
                _setInitFlag(false);
            }
            DictTrie(const string& filePath)
            {
                new (this) DictTrie();
                _setInitFlag(init(filePath));
            }
            ~DictTrie()
            {
                _deleteNode(_root);
            }
        private:
            
            
        public:
            bool init(const string& filePath)
            {
                assert(!_getInitFlag());
                _loadDict(filePath, _nodeInfos);
                _createDictTrie(_nodeInfos, _root);
                _freqSum = _calculateFreqSum(_nodeInfos);
                assert(_freqSum);
                _minLogFreq = _calculateLogFreqAndGetMinValue(_nodeInfos, _freqSum);
                return _setInitFlag(true);
            }


        public:
            double getMinLogFreq() const {return _minLogFreq;};

        private:
            void _loadDict(const string& filePath, vector<DictUnit>& nodeInfos) const
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
                DictUnit nodeInfo;
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
            size_t _calculateFreqSum(const vector<DictUnit>& nodeInfos) const
            {
                size_t freqSum = 0;
                for(size_t i = 0; i < nodeInfos.size(); i++)
                {
                    freqSum += nodeInfos[i].freq;
                }
                return freqSum;
            }
            double _calculateLogFreqAndGetMinValue(vector<DictUnit>& nodeInfos, size_t freqSum) const
            {
                assert(freqSum);
                double minLogFreq = MAX_DOUBLE;
                for(size_t i = 0; i < nodeInfos.size(); i++)
                {
                    DictUnit& nodeInfo = nodeInfos[i];
                    assert(nodeInfo.freq);
                    nodeInfo.logFreq = log(double(nodeInfo.freq)/double(freqSum));
                    if(minLogFreq > nodeInfo.logFreq)
                    {
                        minLogFreq = nodeInfo.logFreq;
                    }
                }
                return minLogFreq;
            }


    };
}

#endif
