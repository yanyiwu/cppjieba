#include "src/TrieManager.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

const char* const DICT_FILE[] = {
    "../test/testdata/jieba.dict.0.utf8", 
    "../test/testdata/jieba.dict.0.1.utf8", 
    "../test/testdata/jieba.dict.1.utf8",
    "../test/testdata/jieba.dict.2.utf8"};

const char* const DICT_FILE_MD5[] = {
    "5aef74a56b363d994095c407c4809d84",
    "5aef74a56b363d994095c407c4809d84",
    "55f1116c05c8051ab53171f0b7455197",
    "b123553a2418c4bda51abc64d705d5d4"};

TEST(Md5Test, Test1)
{
    ASSERT_EQ(sizeof(DICT_FILE)/sizeof(DICT_FILE[0]), sizeof(DICT_FILE_MD5)/sizeof(DICT_FILE_MD5[0]));
    string tmp;
    for (int i = 0; i < sizeof(DICT_FILE)/sizeof(DICT_FILE[0]); i++)
    {
        md5File(DICT_FILE[i], tmp);
        ASSERT_EQ(tmp, string(DICT_FILE_MD5[i]));
    }
}

