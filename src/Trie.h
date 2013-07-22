/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#ifndef CPPJIEBA_TRIE_H
#define CPPJIEBA_TRIE_H

#include <iostream>
#include <fstream>
//#include <ext/hash_map>
#include <map>
#include <cstring>
#include <stdint.h>
#include <cmath>
#include <limits>
#include "cppcommon/str_functs.h"
#include "cppcommon/vec_functs.h"
#include "cppcommon/file_functs.h"
#include "cppcommon/logger.h"
#include "globals.h"
#include "tools.h"


namespace CppJieba
{
    using namespace CPPCOMMON;
    using namespace std;
    //using __gnu_cxx::hash_map;
	typedef map<uint16_t, struct TrieNode*> TrieNodeMap;

	struct TrieNodeInfo
	{
		string word;
		size_t wLen;// the word's len , not string.size(), eg: "我是中国人" wLen = 5 .
		size_t count;
		string tag;
		double weight;
		TrieNodeInfo()
		{
			word = "";
			wLen = 0;
			count = 0;
			tag = "";
			weight = 0.0;
		}
	};

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

			int64_t _totalCount;
			double _minWeight;
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
			bool loadDict(const string& filePath);
            bool dispose();

		private:
			void _setInitFlag(bool on);
			bool _getInitFlag();

		public:
			const TrieNodeInfo* find(const string& str);
			const TrieNodeInfo* find(const vector<uint16_t>& unicode);
			const TrieNodeInfo* findPrefix(const string& str);

		public:
			double getWeight(const string& str);
			double getMinWeight();
			
			int64_t getTotalCount();

			bool insert(const TrieNodeInfo& nodeInfo);

        private:
			bool _buildTree(const string& filePath);
			bool _countWeight();
            bool _deleteNode(TrieNode* node);

    };
}

#endif
