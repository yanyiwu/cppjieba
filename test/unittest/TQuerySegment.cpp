#include "src/QuerySegment.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

TEST(QuerySegment, Test1)
{
    QuerySegment segment("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8", 3);
    const char* str = "小明硕士毕业于中国科学院计算所，后在日本京都大学深造";
    const char* res[] = {"小明", "硕士", "毕业", "于", "中国", "中国科学院", "科学", "科学院", "学院", "计算所", "，", "后", "在", "日本", "日本京都大学", "京都", "京都大学", "大学", "深造"};
    vector<string> words;

    ASSERT_EQ(segment.cut(str, words), true);

    EXPECT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
}

