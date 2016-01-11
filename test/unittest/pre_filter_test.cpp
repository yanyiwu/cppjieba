#include "gtest/gtest.h"
#include "cppjieba/PreFilter.hpp"

using namespace cppjieba;

TEST(PreFilterTest, Test1) {
  unordered_set<Rune> symbol;
  symbol.insert(65292u); // "，"
  symbol.insert(12290u); // "。"
  string expected;
  string res;

  {
    PreFilter filter(symbol, "你好，美丽的，世界");
    expected = "你好/，/美丽的/，/世界";
    ASSERT_TRUE(filter.HasNext());
    vector<string> words;
    while (filter.HasNext()) {
      PreFilter::Range range;
      range = filter.Next();
      words.push_back(TransCode::Encode(range.begin, range.end));
    }
    res = Join(words.begin(), words.end(), "/");
    ASSERT_EQ(res, expected);
  }

  {
    PreFilter filter(symbol, "我来自北京邮电大学。。。学号123456，用AK47");
    expected = "我来自北京邮电大学/。/。/。/学号123456/，/用AK47";
    ASSERT_TRUE(filter.HasNext());
    vector<string> words;
    while (filter.HasNext()) {
      PreFilter::Range range;
      range = filter.Next();
      words.push_back(TransCode::Encode(range.begin, range.end));
    }
    res = Join(words.begin(), words.end(), "/");
    for (size_t i = 0; i < words.size(); i++) {
    }
    ASSERT_EQ(res, expected);
  }
}
