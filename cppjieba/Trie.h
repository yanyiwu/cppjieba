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
#include <str_functs.hpp>
#include <logger.hpp>
#include "TransCode.hpp"
#include "globals.h"
#include "structs.h"


namespace CppJieba
{
    using namespace Limonp;
    struct TrieNode
    {
        TrieNodeMap hmap;
        bool isLeaf;
        uint nodeInfoVecPos;
        TrieNode()
        {
            isLeaf = false;
            nodeInfoVecPos = 0;
        }
    };

    class Trie
    {

        private:
            TrieNode* _root;
            vector<TrieNodeInfo> _nodeInfoVec;

            bool _initFlag;
            int64_t _freqSum;
            double _minLogFreq;

        public:
            Trie();
            ~Trie();
            bool init();
            bool loadDict(const char * const filePath);
            bool dispose();

        private:
            void _setInitFlag(bool on){_initFlag = on;};
            bool _getInitFlag()const{return _initFlag;};

        public:
            const TrieNodeInfo* find(const string& str)const;
            const TrieNodeInfo* find(const Unicode& uintVec)const;
            const TrieNodeInfo* find(Unicode::const_iterator begin, Unicode::const_iterator end)const;
			bool find(const Unicode& unico, vector<pair<uint, const TrieNodeInfo*> >& res)const;

            const TrieNodeInfo* findPrefix(const string& str)const;

        public:
            //double getWeight(const string& str);
            //double getWeight(const Unicode& uintVec);
            //double getWeight(Unicode::const_iterator begin, Unicode::const_iterator end);
            double getMinLogFreq()const{return _minLogFreq;};
            
            //int64_t getTotalCount(){return _freqSum;};

            bool insert(const TrieNodeInfo& nodeInfo);

        private:
            bool _trieInsert(const char * const filePath);
            bool _countWeight();
            bool _deleteNode(TrieNode* node);

    };
}

#endif
