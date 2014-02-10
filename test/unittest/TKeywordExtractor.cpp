#include "src/KeywordExtractor.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;



TEST(KeywordExtractorTest, Test1)
{
    KeywordExtractor extractor("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8", "../dict/idf.utf8");
    string s("我是蓝翔技工拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上总经理，出任CEO，迎娶白富美，走上人生巅峰。");
    string res;
    vector<pair<string, double> > wordweights;
    size_t topN = 5;
    extractor.extract(s, wordweights, topN);
    res << wordweights;
    ASSERT_EQ(res, "[\"CEO:11.7392\", \"蓝翔:11.7392\", \"白富美:11.7392\", \"升职:10.8562\", \"加薪:10.6426\"]");
}
