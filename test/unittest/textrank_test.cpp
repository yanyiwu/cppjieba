#include "cppjieba/TextRankExtractor.hpp"
#include "gtest/gtest.h"

using namespace cppjieba;

TEST(TextRankExtractorTest, Test1) {
  TextRankExtractor Extractor(
    "../test/testdata/extra_dict/jieba.dict.small.utf8",
    "../dict/hmm_model.utf8", 
    "../dict/stop_words.utf8");
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
      //ASSERT_EQ(res, "[\"世界:1\", \"你好:0.514286\"]");
      ASSERT_EQ(res, "[\"\xE4\xB8\x96\xE7\x95\x8C:1\", \"\xE4\xBD\xA0\xE5\xA5\xBD:0.519787\"]");
    }

    {
      vector<TextRankExtractor::Word> words;
      Extractor.Extract(s, words, topN);
      res << words;
      //ASSERT_EQ(res, "[\"世界|[\"6\", \"12\"]|1\", \"你好|[\"0\"]|0.514286\"]");
      ASSERT_EQ(res, "[\"\xE4\xB8\x96\xE7\x95\x8C|[\"6\", \"12\"]|1\", \"\xE4\xBD\xA0\xE5\xA5\xBD|[\"0\"]|0.519787\"]");
    }
  }

  { 
    string s("\xe6\x88\x91\xe6\x98\xaf\xe6\x8b\x96\xe6\x8b\x89\xe6\x9c\xba\xe5\xad\xa6\xe9\x99\xa2\xe6\x89\x8b\xe6\x89\xb6\xe6\x8b\x96\xe6\x8b\x89\xe6\x9c\xba\xe4\xb8\x93\xe4\xb8\x9a\xe7\x9a\x84\xe3\x80\x82\xe4\xb8\x8d\xe7\x94\xa8\xe5\xa4\x9a\xe4\xb9\x85\xef\xbc\x8c\xe6\x88\x91\xe5\xb0\xb1\xe4\xbc\x9a\xe5\x8d\x87\xe8\x81\x8c\xe5\x8a\xa0\xe8\x96\xaa\xef\xbc\x8c\xe5\xbd\x93\xe4\xb8\x8a CEO\xef\xbc\x8c\xe8\xb5\xb0\xe4\xb8\x8a\xe4\xba\xba\xe7\x94\x9f\xe5\xb7\x85\xe5\xb3\xb0");
    string res;
    vector<TextRankExtractor::Word> wordweights;
    size_t topN = 5;
    Extractor.Extract(s, wordweights, topN);
    res << wordweights;
    ASSERT_EQ(res, "[\"\xE5\xBD\x93\xE4\xB8\x8A|[\"87\"]|1\", \"\xE4\xB8\x8D\xE7\x94\xA8|[\"48\"]|0.989848\", \"\xE5\xA4\x9A\xE4\xB9\x85|[\"54\"]|0.985126\", \"\xE5\x8A\xA0\xE8\x96\xAA|[\"78\"]|0.983046\", \"\xE5\x8D\x87\xE8\x81\x8C|[\"72\"]|0.980278\"]");
    // ASSERT_EQ(res, "[\"\xE4\xB8\x93\xE4\xB8\x9A|[\"36\"]|1\", \"CEO|[\"94\"]|0.953149\", \"\xE6\x89\x8B\xE6\x89\xB6\xE6\x8B\x96\xE6\x8B\x89\xE6\x9C\xBA|[\"21\"]|0.794203\", \"\xE5\xBD\x93\xE4\xB8\x8A|[\"87\"]|0.78716\", \"\xE8\xB5\xB0\xE4\xB8\x8A|[\"100\"]|0.767636\"]");
  }

  {
    string s("一部iPhone6");
    string res;
    vector<TextRankExtractor::Word> wordweights;
    size_t topN = 5;
    Extractor.Extract(s, wordweights, topN);
    res << wordweights;
    ASSERT_EQ(res, "[\"\xE4\xB8\x80\xE9\x83\xA8|[\"0\"]|1\", \"iPhone6|[\"6\"]|0.996126\"]");
    //ASSERT_EQ(res, "[\"iPhone6|[\"6\"]|1\", \"\xE4\xB8\x80\xE9\x83\xA8|[\"0\"]|0.996126\"]");
  }
}

TEST(TextRankExtractorTest, Test2) {
  TextRankExtractor Extractor(
    "../test/testdata/extra_dict/jieba.dict.small.utf8",
    "../dict/hmm_model.utf8",
    "../dict/stop_words.utf8",
    "../test/testdata/userdict.utf8");

  {
    string s("\xe8\x93\x9d\xe7\xbf\x94\xe4\xbc\x98\xe7\xa7\x80\xe6\xaf\x95\xe4\xb8\x9a\xe7\x94\x9f");
    string res;
    vector<TextRankExtractor::Word> wordweights;
    size_t topN = 5;
    Extractor.Extract(s, wordweights, topN);
    res << wordweights;
    ASSERT_EQ(res, "[\"蓝翔|[\"0\"]|1\", \"毕业生|[\"12\"]|0.996685\", \"优秀|[\"6\"]|0.992994\"]");
    //ASSERT_EQ(res, "[\"\xE4\xBC\x98\xE7\xA7\x80|[\"6\"]|1\", \"\xE6\xAF\x95\xE4\xB8\x9A\xE7\x94\x9F|[\"12\"]|0.996685\", \"\xE8\x93\x9D\xE7\xBF\x94|[\"0\"]|0.992994\"]");
  }

  {
    string s("一部iPhone6");
    string res;
    vector<TextRankExtractor::Word> wordweights;
    size_t topN = 5;
    Extractor.Extract(s, wordweights, topN);
    res << wordweights;
    //ASSERT_EQ(res, "[\"iPhone6|[\"6\"]|1\", \"\xE4\xB8\x80\xE9\x83\xA8|[\"0\"]|0.996126\"]");
    ASSERT_EQ(res, "[\"\xE4\xB8\x80\xE9\x83\xA8|[\"0\"]|1\", \"iPhone6|[\"6\"]|0.996126\"]");
  }
}
