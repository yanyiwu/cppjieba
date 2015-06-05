#include "src/Application.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

TEST(ApplicationTest, Test1) {
  Application app("../dict/");
  vector<string> words;
  string result;
  
  app.cut("我来自北京邮电大学。", words, METHOD_MP);
  result << words;
  ASSERT_EQ("[\"我\", \"来自\", \"北京邮电大学\", \"。\"]", result);

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

  vector<pair<string, string> > tagres;
  app.tag("iPhone6手机的最大特点是很容易弯曲。", tagres);
  result << tagres;
  ASSERT_EQ("[\"iPhone6:eng\", \"手机:n\", \"的:uj\", \"最大:a\", \"特点:n\", \"是:v\", \"很:zg\", \"容易:a\", \"弯曲:v\", \"。:x\"]", result);

  vector<pair<string, double> > keywordres;
  app.extract("我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。", keywordres, 5);
  result << keywordres;
  ASSERT_EQ(result, "[\"CEO:11.7392\", \"升职:10.8562\", \"加薪:10.6426\", \"手扶拖拉机:10.0089\", \"巅峰:9.49396\"]");
}
