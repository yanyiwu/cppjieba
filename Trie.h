#ifndef TRIE_H
#define TRIE_H
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
#include "cppcommon/logger.h"
#include "globals.h"


namespace CppJieba
{
    using namespace CPPCOMMON;
    using namespace std;
    //using __gnu_cxx::hash_map;
    typedef uint16_t ChUnicode;
	const size_t ChUniMaxLen = 1024;
    typedef map<ChUnicode, struct TrieNode*> TrieNodeHashMap;

	struct TrieNodeInfo
	{
		string word;
		unsigned int count;
		string tag;
		double weight;
		TrieNodeInfo():word(),count(0),tag(),weight(0.0)
		{
		}
	};

	struct TrieNode
    {
        TrieNodeHashMap hmap;
        bool isLeaf;
		unsigned int nodeInfoVecPos;

        TrieNode()
        :hmap(), isLeaf(false), nodeInfoVecPos(0)
        {
        }
    };

	/*
    struct TrieNodeIterator
    {
        TrieNode* ptNode;
        
        TrieNodeIterator():ptNode(NULL)
        {
        }

        TrieNodeIterator(TrieNode* ptr):ptNode(NULL)
        {
            ptNode = ptr;
        }

        const int operator++(int)
        {
            return 1;
        }

        TrieNodeIterator& operator++()
        {
            return *this;
        }

        TrieNode& operator*() const
        {
            return *ptNode;
        }

        TrieNode* operator->() const
        {
            return ptNode;
        }
        
        bool operator==(const TrieNodeIterator& x) const
        {
            return ptNode == x.ptNode;
        }
        
        bool operator!=(const TrieNodeIterator& x) const
        {
            return ptNode != x.ptNode;
        }
    };
	*/

    class Trie
    {
        private:
            TrieNode* _root;
			vector<TrieNodeInfo> _nodeInfoVec;

			int64_t _totalCount;
			double _minWeight;

        public:
            typedef vector<TrieNodeInfo>::iterator iterator;

        public:
            iterator begin();
            iterator end();

        public:
            Trie();
            ~Trie();
            bool init(const char* const filePath);
            bool destroy();
            void display();

		public:
			const TrieNodeInfo* find(const ChUnicode* const chUniStr, size_t len);
            //bool find(const ChUnicode* chUniStr, size_t len);
			//bool find(const vector<ChUnicode>& uniVec);
			int findMaxMatch(const ChUnicode* chUniStr, size_t len);

		public:
			double getWeight(const ChUnicode* uniStr, size_t len);
            //bool cut(const ChUnicode* chUniStr, size_t len, vector< vector<size_t> >& res);
			//bool cutUni(const vector<ChUnicode>& uniVec, )
			//bool cutUtf8(const string& str, vector< vector<size_t> >& res);
			//bool cutMa

        private:
			bool _buildTree(const char* const filePath);
			bool _countWeight();
            bool _destroyNode(TrieNode* node);
            void _display(TrieNode* node, int level);
			bool _insert(const TrieNodeInfo& nodeInfo);

		private:
			enum {bufSize = 1024};
    };
}

#endif
