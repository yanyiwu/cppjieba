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
#include "cppcommon/str_functs.h"
#include "cppcommon/vec_functs.h"
#include "cppcommon/file_functs.h"
#include "cppcommon/logger.h"
#include "TransCode.h"
#include "globals.h"
#include "structs.h"


namespace CppJieba
{
    using namespace CPPCOMMON;
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

			int64_t _freqSum;
			double _minLogFreq;
			bool _initFlag;

        public:
            typedef vector<TrieNodeInfo>::iterator iterator;

        public:
            iterator begin();
            iterator end();

        public:
            Trie();
            ~Trie();
            bool init();
			bool loadDict(const char * const filePath);
            bool dispose();

		private:
			void _setInitFlag(bool on);
			bool _getInitFlag();

		public:
			const TrieNodeInfo* find(const string& str);
			const TrieNodeInfo* find(const VUINT16& uintVec);
			const TrieNodeInfo* find(VUINT16_CONST_ITER begin, VUINT16_CONST_ITER end);
			const TrieNodeInfo* findPrefix(const string& str);

		public:
			double getWeight(const string& str);
			double getWeight(const VUINT16& uintVec);
			double getWeight(VUINT16_CONST_ITER begin, VUINT16_CONST_ITER end);
			double getMinLogFreq();
			
			int64_t getTotalCount();

			bool insert(const TrieNodeInfo& nodeInfo);

        private:
			bool _trieInsert(const char * const filePath);
			bool _countWeight();
            bool _deleteNode(TrieNode* node);

    };
}

#endif
