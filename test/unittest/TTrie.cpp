#include "src/DictTrie.hpp"
#include "src/MPSegment.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

static const char* const DICT_FILE = "../dict/extra_dict/jieba.dict.small.utf8";

TEST(DictTrieTest, NewAndDelete) {
  DictTrie * trie;
  trie = new DictTrie(DICT_FILE);
  delete trie;
  trie = new DictTrie();
  delete trie;
}

TEST(DictTrieTest, Test1) {

  string s1, s2;
  DictTrie trie;
  trie.init(DICT_FILE);
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
  LocalVector<pair<size_t, const DictUnit*> > res;
  //vector<pair<size_t, const DictUnit* > resMap;
  LocalVector<pair<size_t, const DictUnit*> > res2;
  const char * words[] = {"清", "清华", "清华大学"};
  for(size_t i = 0; i < sizeof(words)/sizeof(words[0]); i++) {
    ASSERT_TRUE(TransCode::decode(words[i], uni));
    res.push_back(make_pair(uni.size() - 1, trie.find(uni.begin(), uni.end())));
    //resMap[uni.size() - 1] = trie.find(uni.begin(), uni.end());
  }
  //DictUnit
  //res.push_back(make_pair(0, ))

  vector<pair<size_t, const DictUnit*> > vec;
  ASSERT_TRUE(TransCode::decode(word, uni));
  ASSERT_TRUE(trie.find(uni.begin(), uni.end(), res2, 0));
  s1 << res;
  s2 << res;
  ASSERT_EQ(s1, s2);
}

TEST(DictTrieTest, UserDict) {
  DictTrie trie(DICT_FILE, "../test/testdata/userdict.utf8");
  string word = "云计算";
  Unicode unicode;
  ASSERT_TRUE(TransCode::decode(word, unicode));
  const DictUnit * unit = trie.find(unicode.begin(), unicode.end());
  ASSERT_TRUE(unit);
  string res ;
  res << *unit;
  ASSERT_EQ("[\"20113\", \"35745\", \"31639\"]  -2.975", res);
}

TEST(DictTrieTest, automation) {
  DictTrie trie(DICT_FILE, "../test/testdata/userdict.utf8");
  //string word = "yasherhs";
  string word = "abcderf";
  Unicode unicode;
  ASSERT_TRUE(TransCode::decode(word, unicode));
  vector<struct SegmentChar> res;
  trie.find(unicode.begin(), unicode.end(), res);
}
