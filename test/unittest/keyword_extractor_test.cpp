#include "cppjieba/KeywordExtractor.hpp"
#include "gtest/gtest.h"

using namespace cppjieba;

TEST(KeywordExtractorTest, Test1) {
  KeywordExtractor Extractor("../test/testdata/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8", "../dict/idf.utf8", "../dict/stop_words.utf8");

  {
    string s("你好世界世界而且而且");
    string res;
    size_t topN = 5;

    {
      vector<string> words;
      Extractor.Extract(s, words, topN);
      res << words;
      ASSERT_EQ(res, "[\"世界\", \"你好\"]");
    }

    {
      vector<pair<string, double> > words;
      Extractor.Extract(s, words, topN);
      res << words;
      ASSERT_EQ(res, "[\"世界:8.73506\", \"你好:7.95788\"]");
    }

    {
      vector<KeywordExtractor::Word> words;
      Extractor.Extract(s, words, topN);
      res << words;
      ASSERT_EQ(res, "[\"世界|[\"6\", \"12\"]|8.73506\", \"你好|[\"0\"]|7.95788\"]");
    }
  }

  {
    string s("我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。");
    string res;
    vector<KeywordExtractor::Word> wordweights;
    size_t topN = 5;
    Extractor.Extract(s, wordweights, topN);
    res << wordweights;
    ASSERT_EQ(res, "[\"CEO|[\"93\"]|11.7392\", \"\xE5\x8D\x87\xE8\x81\x8C|[\"72\"]|10.8562\", \"\xE5\x8A\xA0\xE8\x96\xAA|[\"78\"]|10.6426\", \"\xE6\x89\x8B\xE6\x89\xB6\xE6\x8B\x96\xE6\x8B\x89\xE6\x9C\xBA|[\"21\"]|10.0089\", \"\xE5\xB7\x85\xE5\xB3\xB0|[\"111\"]|9.49396\"]");
  }

  {
    string s("一部iPhone6");
    string res;
    vector<KeywordExtractor::Word> wordweights;
    size_t topN = 5;
    Extractor.Extract(s, wordweights, topN);
    res << wordweights;
    ASSERT_EQ(res, "[\"iPhone6|[\"6\"]|11.7392\", \"\xE4\xB8\x80\xE9\x83\xA8|[\"0\"]|6.47592\"]");
  }
}

TEST(KeywordExtractorTest, Test2) {
  KeywordExtractor Extractor("../test/testdata/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8", "../dict/idf.utf8", "../dict/stop_words.utf8", "../test/testdata/userdict.utf8");

  {
    string s("蓝翔优秀毕业生");
    string res;
    vector<KeywordExtractor::Word> wordweights;
    size_t topN = 5;
    Extractor.Extract(s, wordweights, topN);
    res << wordweights;
    ASSERT_EQ(res, "[\"\xE8\x93\x9D\xE7\xBF\x94|[\"0\"]|11.7392\", \"\xE6\xAF\x95\xE4\xB8\x9A\xE7\x94\x9F|[\"12\"]|8.13549\", \"\xE4\xBC\x98\xE7\xA7\x80|[\"6\"]|6.78347\"]");
  }

  {
    string s("一部iPhone6");
    string res;
    vector<KeywordExtractor::Word> wordweights;
    size_t topN = 5;
    Extractor.Extract(s, wordweights, topN);
    res << wordweights;
    ASSERT_EQ(res, "[\"iPhone6|[\"6\"]|11.7392\", \"\xE4\xB8\x80\xE9\x83\xA8|[\"0\"]|6.47592\"]");
  }
}
