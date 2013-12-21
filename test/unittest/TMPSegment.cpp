#include "src/MPSegment.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

TEST(MPSegmentTest, Test1)
{
    MPSegment segment("../dicts/jieba.dict.utf8");;
    const char* str = "我来自北京邮电大学。。。  学号 123456";
    const char* res[] = {"我", "来自", "北京邮电大学", "。","。","。","  ","学","号", " 123456"};
    vector<string> words;
    ASSERT_TRUE(segment);
    ASSERT_TRUE(segment.cut(str, words));
    //print(words);
    EXPECT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
}

