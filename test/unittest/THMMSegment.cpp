#include "src/HMMSegment.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

TEST(HMMSegmentTest, Test1)
{
    HMMSegment segment("../dict/hmm_model.utf8");;
    const char* str = "我来自北京邮电大学。。。  学号 123456";
    const char* res[] = {"我来", "自北京", "邮电大学", "。", "。", "。", "  ", "学号", " 123456"};
    //string s;
    //vector<string> buf(res, res + sizeof(res)/sizeof(res[0]));
    vector<string> words;
    ASSERT_TRUE(segment);
    ASSERT_TRUE(segment.cut(str, words));
    //print(words);
    EXPECT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
}

