#include "src/QuerySegment.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

TEST(QuerySegment, Test1)
{
    QuerySegment segment("../dict/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8", 3);
    const char* str = "小明硕士毕业于中国科学院计算所，后在日本京都大学深造";
    vector<string> words;

    ASSERT_TRUE(segment.cut(str, words));

    string s1, s2;
    s1 << words;
    s2 = "[\"小明\", \"硕士\", \"毕业\", \"于\", \"中国\", \"中国科学院\", \"科学\", \"科学院\", \"学院\", \"计算所\", \"，\", \"后\", \"在\", \"日本\", \"京都\", \"京都大学\", \"大学\", \"深造\"]";
    ASSERT_EQ(s1, s2);

}

