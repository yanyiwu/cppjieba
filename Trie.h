#ifndef TRIE_H
#define TRIE_H
#include <iostream>
#include <fstream>
#include "cppcommon/str_functs.h"
#include "cppcommon/vec_functs.h"
#include "globals.h"

namespace CppJieba
{
    using namespace CPPCOMMON;
    using namespace std;
    class Trie
    {
        public:
            Trie();
            ~Trie();
        public:
            bool init(const char* const filepath = DICT_FILE_PATH);
    };
}

#endif
