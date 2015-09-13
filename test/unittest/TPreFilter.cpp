#include "gtest/gtest.h"
#include "src/PreFilter.hpp"

using namespace CppJieba;

TEST(PreFilterTest, Test1) {
  PreFilter filter;
  filter.Reset("你好，美丽的，世界");
  const char* expected[] = {"你好", "，", "美丽的", "，", "世界"};
  ASSERT_TRUE(filter.HasNext());
  vector<string> words;
  while (filter.HasNext()) {
    PreFilter::Range range;
    range = filter.Next();
    words.push_back(TransCode::encode(range.begin, range.end));
  }
  ASSERT_EQ(vector<string>(expected, expected + sizeof(expected)/sizeof(*expected)), words);
}
