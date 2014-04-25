#include "src/DictTrie.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

static const char* const DICT_FILE = "../dict/extra_dict/jieba.dict.small.utf8";

TEST(DictTrieTest, NewAndDelete)
{
    DictTrie * trie;
    trie = new DictTrie(DICT_FILE);
    delete trie;
    trie = new DictTrie();
    delete trie;
}

TEST(DictTrieTest, Test1)
{

    string s1, s2;
    DictTrie trie;
    ASSERT_TRUE(trie.init(DICT_FILE));
    ASSERT_LT(trie.getMinWeight() + 15.6479, 0.001);
    string word("来到");
    Unicode uni;
    ASSERT_TRUE(TransCode::decode(word, uni));
    DictUnit nodeInfo;
    nodeInfo.word = uni;
    nodeInfo.tag = "v";
    nodeInfo.weight = -8.87033;
    s1 << nodeInfo;
    s2 << (*trie.find(uni.begin(), uni.end()));
    
    EXPECT_EQ("[\"26469\", \"21040\"] v -8.870", s2);
    word = "清华大学";
    vector<pair<size_t, const DictUnit*> > res;
    map<size_t, const DictUnit* > resMap;
    map<size_t, const DictUnit* > mp;
    const char * words[] = {"清", "清华", "清华大学"};
    for(size_t i = 0; i < sizeof(words)/sizeof(words[0]); i++)
    {
        ASSERT_TRUE(TransCode::decode(words[i], uni));
        res.push_back(make_pair(uni.size() - 1, trie.find(uni.begin(), uni.end())));
        resMap[uni.size() - 1] = trie.find(uni.begin(), uni.end());
    }
    //DictUnit
    //res.push_back(make_pair(0, ))

    vector<pair<size_t, const DictUnit*> > vec;
    ASSERT_TRUE(TransCode::decode(word, uni));
    ASSERT_TRUE(trie.find(uni.begin(), uni.end(), mp, 0));
    ASSERT_EQ(mp, resMap);
}

TEST(DictTrieTest, UserDict)
{
    DictTrie trie(DICT_FILE, "../test/testdata/userdict.utf8");
    ASSERT_TRUE(trie);
    string word = "云计算";
    Unicode unicode;
    ASSERT_TRUE(TransCode::decode(word, unicode));
    const DictUnit * unit = trie.find(unicode.begin(), unicode.end());
    ASSERT_TRUE(unit);
    string res ;
    res << *unit;
    ASSERT_EQ("[\"20113\", \"35745\", \"31639\"] x -2.975", res);
}
