#ifndef TRIE_H
#define TRIE_H
#include <iostream>
#include <fstream>
#include <ext/hash_map>
#include <cstring>
#include "cppcommon/str_functs.h"
#include "cppcommon/vec_functs.h"
#include "cppcommon/logger.h"
#include "globals.h"


namespace CppJieba
{
    using namespace CPPCOMMON;
    using namespace std;
    using __gnu_cxx::hash_map;
    typedef uint16_t ChUnicode;
    typedef hash_map<ChUnicode, struct TrieNode*> TrieNodeHashMap;

    struct TrieNode
    {
        TrieNodeHashMap hmap;
        bool isLeaf;
        unsigned int count;
        string tag;
        TrieNode()
        :hmap(), isLeaf(false), count(0), tag()
        {
        }
    };

    class Trie
    {
        private:
            TrieNode* _root;
        public:
            Trie();
            ~Trie();
            bool init(const char* const filepath = DICT_FILE_PATH);
            bool destroy();
            void display();
            bool find(const ChUnicode* chUniStr, size_t len);
            bool cut(const ChUnicode* chUniStr, size_t len, vector< vector<size_t> >& res);
        private:
            bool _destroyNode(TrieNode* node);
            void _display(TrieNode* node, int level);
            bool _insert(const ChUnicode* chUniBuf, size_t len, unsigned int cnt, const string& tag);
    };
}

#endif
