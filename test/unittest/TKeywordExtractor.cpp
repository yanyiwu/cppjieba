#include "src/KeywordExtractor.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

TEST(KeywordExtractorTest, Test1)
{
    KeywordExtractor extractor("../dicts/jieba.dict.utf8");
    const char* str = "我来自北京邮电大学。。。  学号 123456";
    const char* res[] = {"北京邮电大学", "来自"};
    vector<string> words;
    ASSERT_TRUE(extractor);
    ASSERT_TRUE(extractor.extract(str, words, 2));
    ASSERT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
}

TEST(KeywordExtractorTest, Test2)
{
    KeywordExtractor extractor("../dicts/jieba.dict.utf8");
    const char* str = "我来自北京邮电大学。。。  学号 123456";
    const char* res[] = {"北京邮电大学", "来自", "学", "号", "我"};
    vector<string> words;
    ASSERT_TRUE(extractor);
    ASSERT_TRUE(extractor.extract(str, words, 9));
    ASSERT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
}


TEST(KeywordExtractorTest, Test3)
{
    ifstream ifs("../test/testdata/weicheng.utf8");
    ASSERT_TRUE(ifs);
    string str((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
    KeywordExtractor extractor("../dicts/jieba.dict.utf8");
    vector<string> keywords;
    string res;
    extractor.extract(str, keywords, 5);
    res << keywords;
    ASSERT_EQ("[\"第三性\", \"多愁多病\", \"记挂着\", \"揭去\", \"贫血症\"]", res);
}
