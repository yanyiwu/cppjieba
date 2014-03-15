#include "src/FullSegment.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

TEST(FullSegment, Test1)
{
    FullSegment segment("../dict/extra_dict/jieba.dict.small.utf8");
    const char* str = "我来自北京邮电大学。。。  学号 123456";
    vector<string> words;

    ASSERT_EQ(segment.cut(str, words), true);

    string s;
    s << words;
    ASSERT_EQ(s, "[\"我\", \"来自\", \"北京\", \"北京邮电大学\", \"邮电\", \"电大\", \"大学\", \"。\", \"。\", \"。\", \"  \", \"学\", \"号\", \" 123456\"]");
}

