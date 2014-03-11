#include "src/Trie.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

static const char* const DICT_FILE = "../dict/jieba.dict.utf8";

TEST(TrieTest, Test1)
{
    Trie trie;
    ASSERT_TRUE(trie.init());
    ASSERT_TRUE(trie.loadDict(DICT_FILE));
    ASSERT_LT(trie.getMinLogFreq() + 17.2184, 0.001);
    string word("来到");
    Unicode uni;
    ASSERT_TRUE(TransCode::decode(word, uni));
    TrieNodeInfo nodeInfo;
    nodeInfo.word = uni;
    nodeInfo.freq = 8779;
    nodeInfo.tag = "v";
    nodeInfo.logFreq = -8.83144;
    EXPECT_EQ(nodeInfo, *trie.find(uni.begin(), uni.end()));
    word = "清华大学";
    vector<pair<uint, const TrieNodeInfo*> > res;
    map<uint, const TrieNodeInfo* > resMap;
    map<uint, const TrieNodeInfo* > map;
    const char * words[] = {"清", "清华", "清华大学"};
    for(uint i = 0; i < sizeof(words)/sizeof(words[0]); i++)
    {
        ASSERT_TRUE(TransCode::decode(words[i], uni));
        res.push_back(make_pair(uni.size() - 1, trie.find(uni.begin(), uni.end())));
        resMap[uni.size() - 1] = trie.find(uni.begin(), uni.end());
    }
    //TrieNodeInfo
    //res.push_back(make_pair(0, ))

    vector<pair<uint, const TrieNodeInfo*> > vec;
    ASSERT_TRUE(TransCode::decode(word, uni));
    //print(uni);
    ASSERT_TRUE(trie.find(uni.begin(), uni.end(), vec));
    ASSERT_EQ(vec, res);
    ASSERT_TRUE(trie.find(uni.begin(), uni.end(), 0, map));
    ASSERT_EQ(map, resMap);
   // print(vec);
}

