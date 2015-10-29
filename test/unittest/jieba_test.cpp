#include "src/Jieba.hpp"
#include "gtest/gtest.h"

using namespace cppjieba;

TEST(JiebaTest, Test1) {
  cppjieba::Jieba jieba("../dict/jieba.dict.utf8",
                            "../dict/hmm_model.utf8",
                            "../dict/user.dict.utf8");
  vector<string> words;
  string result;

  jieba.Cut("他来到了网易杭研大厦", words);
  result << words;
  ASSERT_EQ("[\"他\", \"来到\", \"了\", \"网易\", \"杭研\", \"大厦\"]", result);
  
  jieba.Cut("我来自北京邮电大学。", words, false);
  result << words;
  ASSERT_EQ("[\"我\", \"来自\", \"北京邮电大学\", \"。\"]", result);

  jieba.CutSmall("南京市长江大桥", words, 3);
  ASSERT_EQ("[\"南京市\", \"长江\", \"大桥\"]", result << words);

  jieba.CutHMM("我来自北京邮电大学。。。学号123456", words);
  result << words;
  ASSERT_EQ("[\"我来\", \"自北京\", \"邮电大学\", \"。\", \"。\", \"。\", \"学号\", \"123456\"]", result);

  jieba.Cut("我来自北京邮电大学。。。学号123456，用AK47", words);
  result << words;
  ASSERT_EQ("[\"我\", \"来自\", \"北京邮电大学\", \"。\", \"。\", \"。\", \"学号\", \"123456\", \"，\", \"用\", \"AK47\"]", result);

  jieba.CutAll("我来自北京邮电大学", words);
  result << words;
  ASSERT_EQ(result, "[\"我\", \"来自\", \"北京\", \"北京邮电\", \"北京邮电大学\", \"邮电\", \"邮电大学\", \"电大\", \"大学\"]");

  jieba.CutForSearch("他来到了网易杭研大厦", words);
  result << words;
  ASSERT_EQ("[\"他\", \"来到\", \"了\", \"网易\", \"杭研\", \"大厦\"]", result);

  jieba.CutLevel("南京市长江大桥", words);
  result << words;
  ASSERT_EQ("[\"南京市\", \"长江大桥\", \"南京\", \"长江\", \"大桥\"]", result);

  vector<pair<string, size_t> > word_levels;
  jieba.CutLevel("南京市长江大桥", word_levels);
  result << word_levels;
  ASSERT_EQ("[\"南京市:0\", \"长江大桥:0\", \"南京:1\", \"长江:1\", \"大桥:1\"]", result);

  //vector<pair<string, string> > tagres;
  //jieba.Tag("iPhone6手机的最大特点是很容易弯曲。", tagres);
  //result << tagres;
  //ASSERT_EQ("[\"iPhone6:eng\", \"手机:n\", \"的:uj\", \"最大:a\", \"特点:n\", \"是:v\", \"很:zg\", \"容易:a\", \"弯曲:v\", \"。:x\"]", result);

  //vector<pair<string, double> > keywordres;
  //jieba.Extract("我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。", keywordres, 5);
  //result << keywordres;
  //ASSERT_EQ(result, "[\"CEO:11.7392\", \"升职:10.8562\", \"加薪:10.6426\", \"手扶拖拉机:10.0089\", \"巅峰:9.49396\"]");
}

TEST(JiebaTest, InsertUserWord) {
  cppjieba::Jieba jieba("../dict/jieba.dict.utf8",
                            "../dict/hmm_model.utf8",
                            "../dict/user.dict.utf8");
  vector<string> words;
  string result;

  jieba.Cut("男默女泪", words);
  result << words;
  ASSERT_EQ("[\"男默\", \"女泪\"]", result);

  ASSERT_TRUE(jieba.InsertUserWord("男默女泪"));

  jieba.Cut("男默女泪", words);
  result << words;
  ASSERT_EQ("[\"男默女泪\"]", result);

  for (size_t i = 0; i < 100; i++) {
    string newWord;
    newWord << rand();
    ASSERT_TRUE(jieba.InsertUserWord(newWord));
    jieba.Cut(newWord, words);
    result << words;
    ASSERT_EQ(result, string_format("[\"%s\"]", newWord.c_str()));
  }
}
