#include "src/Trie.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

static const char* const DICT_FILE = "../dict/extra_dict/jieba.dict.small.utf8";

TEST(TrieTest, Test1)
{
    string s1, s2;
    Trie trie;
    ASSERT_TRUE(trie.init(DICT_FILE));
    ASSERT_LT(trie.getMinLogFreq() + 15.6479, 0.001);
    string word("来到");
    Unicode uni;
    ASSERT_TRUE(TransCode::decode(word, uni));
    TrieNodeInfo nodeInfo;
    nodeInfo.word = uni;
    nodeInfo.freq = 8779;
    nodeInfo.tag = "v";
    nodeInfo.logFreq = -8.87033;
    s1 << nodeInfo;
    s2 << (*trie.find(uni.begin(), uni.end()));
    
    EXPECT_EQ("[\"26469\", \"21040\"]:8779:v:-8.87033", s2);
    word = "清华大学";
    vector<pair<size_t, const TrieNodeInfo*> > res;
    map<size_t, const TrieNodeInfo* > resMap;
    map<size_t, const TrieNodeInfo* > map;
    const char * words[] = {"清", "清华", "清华大学"};
    for(size_t i = 0; i < sizeof(words)/sizeof(words[0]); i++)
    {
        ASSERT_TRUE(TransCode::decode(words[i], uni));
        res.push_back(make_pair(uni.size() - 1, trie.find(uni.begin(), uni.end())));
        resMap[uni.size() - 1] = trie.find(uni.begin(), uni.end());
    }
    //TrieNodeInfo
    //res.push_back(make_pair(0, ))

    vector<pair<size_t, const TrieNodeInfo*> > vec;
    ASSERT_TRUE(TransCode::decode(word, uni));
    //print(uni);
    ASSERT_TRUE(trie.find(uni.begin(), uni.end(), vec));
    ASSERT_EQ(vec, res);
    ASSERT_TRUE(trie.find(uni.begin(), uni.end(), 0, map));
    ASSERT_EQ(map, resMap);
   // print(vec);
}

