#include "src/Application.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

TEST(ApplicationTest, Test1) {
  CppJieba::Application app("../dict/jieba.dict.utf8",
                            "../dict/hmm_model.utf8",
                            "../dict/user.dict.utf8",
                            "../dict/idf.utf8",
                            "../dict/stop_words.utf8");
  vector<string> words;
  string result;

  app.cut("他来到了网易杭研大厦", words);
  result << words;
  ASSERT_EQ("[\"他\", \"来到\", \"了\", \"网易\", \"杭研\", \"大厦\"]", result);
  
  app.cut("我来自北京邮电大学。", words, METHOD_MP);
  result << words;
  ASSERT_EQ("[\"我\", \"来自\", \"北京邮电大学\", \"。\"]", result);

  app.cut("南京市长江大桥", words, 3);
  ASSERT_EQ("[\"南京市\", \"长江\", \"大桥\"]", result << words);

  app.cut("我来自北京邮电大学。。。学号123456", words, METHOD_HMM);
  result << words;
  ASSERT_EQ("[\"我来\", \"自北京\", \"邮电大学\", \"。\", \"。\", \"。\", \"学号\", \"123456\"]", result);

  app.cut("我来自北京邮电大学。。。学号123456，用AK47", words, METHOD_MIX);
  result << words;
  ASSERT_EQ("[\"我\", \"来自\", \"北京邮电大学\", \"。\", \"。\", \"。\", \"学号\", \"123456\", \"，\", \"用\", \"AK47\"]", result);

  app.cut("他来到了网易杭研大厦", words, METHOD_MIX);
  result << words;
  ASSERT_EQ("[\"他\", \"来到\", \"了\", \"网易\", \"杭研\", \"大厦\"]", result);

  app.cut("我来自北京邮电大学", words, METHOD_FULL);
  result << words;
  ASSERT_EQ(result, "[\"我\", \"来自\", \"北京\", \"北京邮电\", \"北京邮电大学\", \"邮电\", \"邮电大学\", \"电大\", \"大学\"]");

  app.cut("他来到了网易杭研大厦", words, METHOD_QUERY);
  result << words;
  ASSERT_EQ("[\"他\", \"来到\", \"了\", \"网易\", \"杭研\", \"大厦\"]", result);

  app.cut("南京市长江大桥", words, METHOD_LEVEL);
  result << words;
  ASSERT_EQ("[\"南京市\", \"长江大桥\", \"南京\", \"长江\", \"大桥\"]", result);

  vector<pair<string, size_t> > word_levels;
  app.cut("南京市长江大桥", word_levels);
  result << word_levels;
  ASSERT_EQ("[\"南京市:0\", \"长江大桥:0\", \"南京:1\", \"长江:1\", \"大桥:1\"]", result);

  vector<pair<string, string> > tagres;
  app.tag("iPhone6手机的最大特点是很容易弯曲。", tagres);
  result << tagres;
  ASSERT_EQ("[\"iPhone6:eng\", \"手机:n\", \"的:uj\", \"最大:a\", \"特点:n\", \"是:v\", \"很:zg\", \"容易:a\", \"弯曲:v\", \"。:x\"]", result);

  vector<pair<string, double> > keywordres;
  app.extract("我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。", keywordres, 5);
  result << keywordres;
  ASSERT_EQ(result, "[\"CEO:11.7392\", \"升职:10.8562\", \"加薪:10.6426\", \"手扶拖拉机:10.0089\", \"巅峰:9.49396\"]");
}

TEST(ApplicationTest, InsertUserWord) {
  CppJieba::Application app("../dict/jieba.dict.utf8",
                            "../dict/hmm_model.utf8",
                            "../dict/user.dict.utf8",
                            "../dict/idf.utf8",
                            "../dict/stop_words.utf8");
  vector<string> words;
  string result;

  app.cut("男默女泪", words);
  result << words;
  ASSERT_EQ("[\"男默\", \"女泪\"]", result);

  ASSERT_TRUE(app.InsertUserWord("男默女泪"));

  app.cut("男默女泪", words);
  result << words;
  ASSERT_EQ("[\"男默女泪\"]", result);

  for (size_t i = 0; i < 100; i++) {
    string newWord;
    newWord << rand();
    ASSERT_TRUE(app.InsertUserWord(newWord));
    app.cut(newWord, words);
    result << words;
    ASSERT_EQ(result, string_format("[\"%s\"]", newWord.c_str()));
  }
}
