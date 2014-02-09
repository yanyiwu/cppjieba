#include "src/KeywordExtractor.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

const char* KEYWORD_EXT_TEST_SENTENCE = "我来自北京邮电大学。 学号123456";

TEST(KeywordExtractorTest, Test1)
{
    KeywordExtractor extractor("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8", "../dict/idf.utf8");
    const char* res[] = {"学号", "北京邮电大学"};
    vector<string> words;
    ASSERT_TRUE(extractor);
    ASSERT_TRUE(extractor.extract(KEYWORD_EXT_TEST_SENTENCE, words, 2));
    ASSERT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
}

TEST(KeywordExtractorTest, Test2)
{
    KeywordExtractor extractor("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8", "../dict/idf.utf8");
    const char* res[] = {"学号", "北京邮电大学", "123456", "来自"};
    vector<string> words;
    ASSERT_TRUE(extractor);
    ASSERT_TRUE(extractor.extract(KEYWORD_EXT_TEST_SENTENCE, words, 9));
    ASSERT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
}


TEST(KeywordExtractorTest, Test3)
{
    ifstream ifs("../test/testdata/weicheng.utf8");
    ASSERT_TRUE(!!ifs);
    string str((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
    KeywordExtractor extractor("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8", "../dict/idf.utf8");
    const char* res[] = {"柔嘉", "小姐", "孙小姐", "方鸿渐", "鸿渐"};
    const char* res2 = "[\"柔嘉:5611.34\", \"小姐:4268.75\", \"孙小姐:3789.41\", \"方鸿渐:3030.35\", \"鸿渐:2552.93\"]";
    vector<string> keywords;
    string resStr;
    vector<pair<string,double> >  keywords2;
    extractor.extract(str, keywords, 5);
    extractor.extract(str, keywords2, 5);
    ASSERT_EQ(keywords, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
    resStr << keywords2;
    ASSERT_EQ(res2, resStr);

}

TEST(KeywordExtractorTest, Test4)
{
    KeywordExtractor extractor("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8", "../dict/idf.utf8");
    string s("我是蓝翔技工拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上总经理，出任CEO，迎娶白富美，走上人生巅峰。");
    string res;
    vector<pair<string, double> > wordweights;
    size_t topN = 5;
    extractor.extract(s, wordweights, topN);
    res << wordweights;
    print(res);
    ASSERT_EQ(res, "[\"CEO:11.7392\", \"白富美:11.7392\", \"蓝翔:11.7392\", \"迎娶:10.0505\", \"加薪:10.6426\"]");
}
