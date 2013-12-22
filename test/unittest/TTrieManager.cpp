#include "src/TrieManager.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

struct md5_ptr
{
    string md5;
    Trie* ptr;
};
typedef struct md5_ptr MD5_PTR;

static const char* const DICT_FILE[] = {
    "../test/testdata/jieba.dict.0.utf8", 
    "../test/testdata/jieba.dict.0.utf8", 
    "../test/testdata/jieba.dict.0.utf8", 
    "../test/testdata/jieba.dict.0.1.utf8", 
    "../test/testdata/jieba.dict.0.1.utf8", 
    "../test/testdata/jieba.dict.0.1.utf8", 
    "../test/testdata/jieba.dict.1.utf8",
    "../test/testdata/jieba.dict.1.utf8",
    "../test/testdata/jieba.dict.1.utf8",
    "../test/testdata/jieba.dict.2.utf8",
    "../test/testdata/jieba.dict.2.utf8",
    "../test/testdata/jieba.dict.2.utf8",
    "../test/testdata/jieba.dict.2.utf8"};

TEST(TrieManagerTest, Test1)
{
    vector<MD5_PTR> tries(sizeof(DICT_FILE)/sizeof(DICT_FILE[0]));
    for (uint i = 0; i < tries.size(); i++)
    {
        tries[i].ptr = TrieManager::getInstance().getTrie(DICT_FILE[i]);
        ASSERT_TRUE(md5File(DICT_FILE[i], tries[i].md5));
    }

    for (uint i = 0; i < tries.size(); i++)
    {
        for (uint j = i + 1; j < tries.size(); j++)
        {
            if (tries[i].md5 == tries[j].md5)
            {
                ASSERT_EQ(tries[i].ptr, tries[j].ptr);
            }
            else
            {
                ASSERT_NE(tries[i].ptr, tries[j].ptr);
            }
        }
    }
}

