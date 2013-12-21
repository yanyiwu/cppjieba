#include "src/MixSegment.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

TEST(MixSegmentTest, Test1)
{
    MixSegment segment("../dicts/jieba.dict.utf8", "../dicts/hmm_model.utf8");;
    const char* str = "我来自北京邮电大学。。。  学号 123456";
    const char* res[] = {"我", "来自", "北京邮电大学", "。","。","。","  ","学号", " 123456"};
    vector<string> words;
    ASSERT_TRUE(segment);
    ASSERT_TRUE(segment.cut(str, words));
    EXPECT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
}

