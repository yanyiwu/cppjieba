#include "src/FullSegment.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

TEST(FullSegment, Test1)
{
    FullSegment segment("../dict/jieba.dict.utf8");
    const char* str = "我来自北京邮电大学。。。  学号 123456";
    const char* res[] = {"我", "来自", "北京", "北京邮电", "北京邮电大学", "邮电", "邮电大学", "电大", "大学", "。", "。", "。", "  ", "学号", " 123456"};
    vector<string> words;

    ASSERT_EQ(segment.cut(str, words), true);

    EXPECT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
}

