#include "src/KeywordExtractor.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

TEST(KeywordExtractorTest, Test1)
{
    KeywordExtractor extractor("../dicts/jieba.dict.utf8");
    const char* str = "我来自北京邮电大学。。。  学号 123456";
    const char* res[] = {"我", "来自", "北京邮电大学", "。","。","。","  ","学","号", " 123456"};
    vector<string> words;
    ASSERT_TRUE(extractor);
    ASSERT_TRUE(extractor.extract(str, words, 2));
    //print(words);
    //exit(0);
    //print(words);
    ASSERT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
}

