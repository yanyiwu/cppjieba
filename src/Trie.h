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


namespace CppJieba
{
    using namespace CPPCOMMON;
    using namespace std;
    //using __gnu_cxx::hash_map;
	typedef map<ChUnicode, struct TrieNode*> TrieNodeMap;

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
			string _encoding;
			vector<string> _encVec;
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
			bool setEncoding(const string& enc);
            bool dispose();

		private:
			void _setInitFlag(bool on);
			bool _getInitFlag();

		public:
			//const TrieNodeInfo* find(const string& uniStr);
			//const TrieNodeInfo* find(const ChUnicode* const chUniStr, size_t len);
			const TrieNodeInfo* find(const string& str);
			const TrieNodeInfo* findPrefix(const string& str);

		public:
			//double getWeight(const ChUnicode* uniStr, size_t len);
			double getWeight(const string& uniStr);
			double getMinWeight();
			
			int64_t getTotalCount();

			bool insert(const TrieNodeInfo& nodeInfo);

			string decode(const string& str);
			string encode(const string& str);

        private:
			bool _buildTree(const string& filePath);
			bool _countWeight();
            bool _deleteNode(TrieNode* node);
			const TrieNodeInfo* _findUniStr(const string& uniStr);

		public:
			static const string& UTF8;
			static const string& GBK;
    };
}

#endif
