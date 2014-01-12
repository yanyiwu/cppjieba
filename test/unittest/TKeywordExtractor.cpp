#include "src/KeywordExtractor.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

TEST(KeywordExtractorTest, Test1)
{
    KeywordExtractor extractor("../dict/jieba.dict.utf8", "../dict/idf.utf8");
    const char* str = "我来自北京邮电大学。。。  学号 123456";
    const char* res[] = {"北京邮电大学", "来自"};
    vector<string> words;
    ASSERT_TRUE(extractor);
    ASSERT_TRUE(extractor.extract(str, words, 2));
    ASSERT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
}

TEST(KeywordExtractorTest, Test2)
{
    KeywordExtractor extractor("../dict/jieba.dict.utf8", "../dict/idf.utf8");
    const char* str = "我来自北京邮电大学。。。  学号 123456";
    const char* res[] = {"北京邮电大学", "来自"};
    vector<string> words;
    ASSERT_TRUE(extractor);
    ASSERT_TRUE(extractor.extract(str, words, 9));
    ASSERT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
}


TEST(KeywordExtractorTest, Test3)
{
    ifstream ifs("../test/testdata/weicheng.utf8");
    ASSERT_TRUE(!!ifs);
    string str((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
    KeywordExtractor extractor("../dict/jieba.dict.utf8", "../dict/idf.utf8");
    const char* res[] = {"小姐", "孙小姐", "方鸿渐", "自己", "没有"};
    vector<string> keywords;
    extractor.extract(str, keywords, 5);
    ASSERT_EQ(keywords, vector<string>(res, res + sizeof(res)/sizeof(res[0])));

}

TEST(KeywordExtractorTest, Test4)
{
    ifstream ifs("../test/testdata/weicheng.utf8");
    ASSERT_TRUE(!!ifs);
    string str((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
    KeywordExtractor extractor("../dict/jieba.dict.utf8", "../dict/idf.utf8");
    //const char* res[] = {"小姐", "孙小姐", "方鸿渐", "自己", "没有"};
    vector<pair<string,double> >  keywords;
    extractor.extract(str, keywords, 5);
    //print(keywords);
    string res;
    res << keywords;
    ASSERT_EQ(res, "[\"小姐:4268.75\", \"孙小姐:3789.41\", \"方鸿渐:3030.35\", \"自己:2300.54\", \"没有:2104.27\"]");

}
