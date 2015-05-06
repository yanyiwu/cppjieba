#include "src/KeywordExtractor.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;



TEST(KeywordExtractorTest, Test1) {
  KeywordExtractor extractor("../dict/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8", "../dict/idf.utf8", "../dict/stop_words.utf8");

  {
    string s("我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。");
    string res;
    vector<pair<string, double> > wordweights;
    size_t topN = 5;
    extractor.extract(s, wordweights, topN);
    res << wordweights;
    ASSERT_EQ(res, "[\"CEO:11.7392\", \"升职:10.8562\", \"加薪:10.6426\", \"手扶拖拉机:10.0089\", \"巅峰:9.49396\"]");
  }

  {
    string s("一部iPhone6");
    string res;
    vector<pair<string, double> > wordweights;
    size_t topN = 5;
    extractor.extract(s, wordweights, topN);
    res << wordweights;
    ASSERT_EQ(res, "[\"iPhone6:11.7392\", \"一部:6.47592\"]");
  }
}

TEST(KeywordExtractorTest, Test2) {
  KeywordExtractor extractor("../dict/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8", "../dict/idf.utf8", "../dict/stop_words.utf8", "../test/testdata/userdict.utf8");

  {
    string s("蓝翔优秀毕业生");
    string res;
    vector<pair<string, double> > wordweights;
    size_t topN = 5;
    extractor.extract(s, wordweights, topN);
    res << wordweights;
    ASSERT_EQ(res, "[\"蓝翔:11.7392\", \"毕业生:8.13549\", \"优秀:6.78347\"]");
  }

  {
    string s("一部iPhone6");
    string res;
    vector<pair<string, double> > wordweights;
    size_t topN = 5;
    extractor.extract(s, wordweights, topN);
    res << wordweights;
    ASSERT_EQ(res, "[\"iPhone6:11.7392\", \"一部:6.47592\"]");
  }
}
