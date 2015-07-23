#include "src/DictTrie.hpp"
#include "src/MPSegment.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

static const char* const DICT_FILE = "../test/testdata/extra_dict/jieba.dict.small.utf8";

TEST(TrieTest, Empty) {
  vector<Unicode> keys;
  vector<const DictUnit*> values;
  Trie trie(keys, values);
}

TEST(TrieTest, Construct) {
  vector<Unicode> keys;
  vector<const DictUnit*> values;
  keys.push_back(TransCode::decode("你"));
  values.push_back((const DictUnit*)(NULL));
  Trie trie(keys, values);
}

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
  const char * words[] = {"清", "清华", "清华大学"};
  for(size_t i = 0; i < sizeof(words)/sizeof(words[0]); i++) {
    ASSERT_TRUE(TransCode::decode(words[i], uni));
    res.push_back(make_pair(uni.size() - 1, trie.find(uni.begin(), uni.end())));
    //resMap[uni.size() - 1] = trie.find(uni.begin(), uni.end());
  }
  vector<pair<size_t, const DictUnit*> > vec;
  vector<struct Dag> dags;
  ASSERT_TRUE(TransCode::decode(word, uni));
  trie.find(uni.begin(), uni.end(), dags);
  ASSERT_EQ(dags.size(), uni.size());
  ASSERT_NE(dags.size(), 0u);
  s1 << res;
  s2 << dags[0].nexts;
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

TEST(DictTrieTest, Dag) {
  DictTrie trie(DICT_FILE, "../test/testdata/userdict.utf8");

  {
    string word = "清华大学";
    Unicode unicode;
    ASSERT_TRUE(TransCode::decode(word, unicode));
    vector<struct Dag> res;
    trie.find(unicode.begin(), unicode.end(), res);

    size_t nexts_sizes[] = {3, 2, 2, 1};
    ASSERT_EQ(res.size(), sizeof(nexts_sizes)/sizeof(nexts_sizes[0]));
    for (size_t i = 0; i < res.size(); i++) {
      ASSERT_EQ(res[i].nexts.size(), nexts_sizes[i]);
    }
  }

  {
    string word = "北京邮电大学";
    Unicode unicode;
    ASSERT_TRUE(TransCode::decode(word, unicode));
    vector<struct Dag> res;
    trie.find(unicode.begin(), unicode.end(), res);

    size_t nexts_sizes[] = {3, 1, 2, 2, 2, 1};
    ASSERT_EQ(res.size(), sizeof(nexts_sizes)/sizeof(nexts_sizes[0]));
    for (size_t i = 0; i < res.size(); i++) {
      ASSERT_EQ(res[i].nexts.size(), nexts_sizes[i]);
    }
  }

  {
    string word = "长江大桥";
    Unicode unicode;
    ASSERT_TRUE(TransCode::decode(word, unicode));
    vector<struct Dag> res;
    trie.find(unicode.begin(), unicode.end(), res);

    size_t nexts_sizes[] = {3, 1, 2, 1};
    ASSERT_EQ(res.size(), sizeof(nexts_sizes)/sizeof(nexts_sizes[0]));
    for (size_t i = 0; i < res.size(); i++) {
      ASSERT_EQ(res[i].nexts.size(), nexts_sizes[i]);
    }
  }

  //findByLimit [2, 3]
  {
    string word = "长江大桥";
    Unicode unicode;
    ASSERT_TRUE(TransCode::decode(word, unicode));
    vector<struct Dag> res;
    trie.findByLimit(unicode.begin(), unicode.end(), res, 2, 3);

    size_t nexts_sizes[] = {1, 0, 1, 0};
    ASSERT_EQ(res.size(), sizeof(nexts_sizes)/sizeof(nexts_sizes[0]));
    for (size_t i = 0; i < res.size(); i++) {
      ASSERT_EQ(res[i].nexts.size(), nexts_sizes[i]);
    }
  }

  //findByLimit [0, 4]
  {
    string word = "长江大桥";
    Unicode unicode;
    ASSERT_TRUE(TransCode::decode(word, unicode));
    vector<struct Dag> res;
    trie.findByLimit(unicode.begin(), unicode.end(), res, 0, 4);

    size_t nexts_sizes[] = {3, 1, 2, 1};
    ASSERT_EQ(res.size(), sizeof(nexts_sizes)/sizeof(nexts_sizes[0]));
    for (size_t i = 0; i < res.size(); i++) {
      ASSERT_EQ(res[i].nexts.size(), nexts_sizes[i]);
    }
  }
}
