#include "src/SegmentBase.hpp"
#include "src/MixSegment.hpp"
#include "src/MPSegment.hpp"
#include "src/HMMSegment.hpp"
#include "src/FullSegment.hpp"
#include "src/QuerySegment.hpp"
#include "src/LevelSegment.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;


TEST(MixSegmentTest, Test1) {
  MixSegment segment("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8");;
  const char* str = "我来自北京邮电大学。。。学号123456，用AK47";
  const char* res[] = {"我", "来自", "北京邮电大学", "。","。","。", "学号", "123456","，","用","AK47"};
  const char* str2 = "B超 T恤";
  const char* res2[] = {"B超"," ", "T恤"};
  vector<string> words;
  ASSERT_TRUE(segment.cut(str, words));
  ASSERT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
  ASSERT_TRUE(segment.cut(str2, words));
  ASSERT_EQ(words, vector<string>(res2, res2 + sizeof(res2)/sizeof(res2[0])));
}

TEST(MixSegmentTest, NoUserDict) {
  MixSegment segment("../test/testdata/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8");
  const char* str = "令狐冲是云计算方面的专家";
  vector<string> words;
  ASSERT_TRUE(segment.cut(str, words));
  string res;
  ASSERT_EQ("[\"令狐冲\", \"是\", \"云\", \"计算\", \"方面\", \"的\", \"专家\"]", res << words);

}
TEST(MixSegmentTest, UserDict) {
  MixSegment segment("../test/testdata/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8", "../dict/user.dict.utf8");
  {
    const char* str = "令狐冲是云计算方面的专家";
    vector<string> words;
    ASSERT_TRUE(segment.cut(str, words));
    string res;
    ASSERT_EQ("[\"令狐冲\", \"是\", \"云计算\", \"方面\", \"的\", \"专家\"]", res << words);
  }
  {
    const char* str = "小明先就职于IBM,后在日本京都大学深造";
    vector<string> words;
    ASSERT_TRUE(segment.cut(str, words));
    string res;
    res << words;
    ASSERT_EQ("[\"小明\", \"先\", \"就职\", \"于\", \"IBM\", \",\", \"后\", \"在\", \"日本\", \"京都大学\", \"深造\"]", res);
  }
  {
    const char* str = "IBM,3.14";
    vector<string> words;
    ASSERT_TRUE(segment.cut(str, words));
    string res;
    res << words;
    ASSERT_EQ("[\"IBM\", \",\", \"3.14\"]", res);
  }
}
TEST(MixSegmentTest, UserDict2) {
  MixSegment segment("../test/testdata/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8", "../test/testdata/userdict.utf8");
  {
    const char* str = "令狐冲是云计算方面的专家";
    vector<string> words;
    ASSERT_TRUE(segment.cut(str, words));
    string res;
    ASSERT_EQ("[\"令狐冲\", \"是\", \"云计算\", \"方面\", \"的\", \"专家\"]", res << words);
  }
  {
    const char* str = "小明先就职于IBM,后在日本京都大学深造";
    vector<string> words;
    ASSERT_TRUE(segment.cut(str, words));
    string res;
    res << words;
    ASSERT_EQ("[\"小明\", \"先\", \"就职\", \"于\", \"I\", \"B\", \"M\", \",\", \"后\", \"在\", \"日本\", \"京都大学\", \"深造\"]", res);
  }
  {
    const char* str = "IBM,3.14";
    vector<string> words;
    ASSERT_TRUE(segment.cut(str, words));
    string res;
    res << words;
    ASSERT_EQ("[\"I\", \"B\", \"M\", \",\", \"3.14\"]", res);
  }
}

TEST(MPSegmentTest, Test1) {
  MPSegment segment("../dict/jieba.dict.utf8");;
  string s;
  vector<string> words;
  ASSERT_TRUE(segment.cut("我来自北京邮电大学。", words));
  ASSERT_EQ("[\"我\", \"来自\", \"北京邮电大学\", \"。\"]", s << words);

  ASSERT_TRUE(segment.cut("B超 T恤", words));
  ASSERT_EQ(s << words, "[\"B超\", \" \", \"T恤\"]");

  ASSERT_TRUE(segment.cut("南京市长江大桥", words));
  ASSERT_EQ("[\"南京市\", \"长江大桥\"]", s << words);

  // MaxWordLen
  ASSERT_TRUE(segment.cut("南京市长江大桥", words, 3));
  ASSERT_EQ("[\"南京市\", \"长江\", \"大桥\"]", s << words);

  ASSERT_TRUE(segment.cut("南京市长江大桥", words, 0));
  ASSERT_EQ("[\"南\", \"京\", \"市\", \"长\", \"江\", \"大\", \"桥\"]", s << words);
}

//TEST(MPSegmentTest, Test2) {
//  MPSegment segment("../test/testdata/extra_dict/jieba.dict.small.utf8");
//  string line;
//  ifstream ifs("../test/testdata/review.100");
//  vector<string> words;
//
//  string eRes;
//  {
//    ifstream ifs("../test/testdata/review.100.res");
//    ASSERT_TRUE(!!ifs);
//    eRes << ifs;
//  }
//  string res;
//
//  while(getline(ifs, line)) {
//    res += line;
//    res += '\n';
//
//    segment.cut(line, words);
//    string s;
//    s << words;
//    res += s;
//    res += '\n';
//  }
//  ofstream ofs("../test/testdata/review.100.res");
//  ASSERT_TRUE(!!ofs);
//  ofs << res;
//
//}

TEST(HMMSegmentTest, Test1) {
  HMMSegment segment("../dict/hmm_model.utf8");;
  {
    const char* str = "我来自北京邮电大学。。。学号123456";
    const char* res[] = {"我来", "自北京", "邮电大学", "。", "。", "。", "学号", "123456"};
    vector<string> words;
    ASSERT_TRUE(segment.cut(str, words));
    ASSERT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
  }

  {
    const char* str = "IBM,1.2,123";
    const char* res[] = {"IBM", ",", "1.2", ",", "123"};
    vector<string> words;
    ASSERT_TRUE(segment.cut(str, words));
    ASSERT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
  }
}

TEST(FullSegment, Test1) {
  FullSegment segment("../test/testdata/extra_dict/jieba.dict.small.utf8");
  vector<string> words;
  string s;

  ASSERT_TRUE(segment.cut("我来自北京邮电大学", words));
  s << words;
  ASSERT_EQ(s, "[\"我\", \"来自\", \"北京\", \"北京邮电大学\", \"邮电\", \"电大\", \"大学\"]");

  
  ASSERT_TRUE(segment.cut("上市公司CEO", words));
  s << words;
  ASSERT_EQ(s, "[\"上市\", \"公司\", \"C\", \"E\", \"O\"]");
}

TEST(QuerySegment, Test1) {
  QuerySegment segment("../test/testdata/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8", "", 3);
  const char* str = "小明硕士毕业于中国科学院计算所，后在日本京都大学深造";
  vector<string> words;

  ASSERT_TRUE(segment.cut(str, words));

  string s1, s2;
  s1 << words;
  s2 = "[\"小明\", \"硕士\", \"毕业\", \"于\", \"中国\", \"中国科学院\", \"科学\", \"科学院\", \"学院\", \"计算所\", \"，\", \"后\", \"在\", \"日本\", \"京都\", \"京都大学\", \"大学\", \"深造\"]";
  ASSERT_EQ(s1, s2);

}

TEST(QuerySegment, Test2) {
  QuerySegment segment("../test/testdata/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8", "../test/testdata/userdict.utf8", 3);

  {
    const char* str = "小明硕士毕业于中国科学院计算所，后在日本京都大学深造";
    vector<string> words;

    ASSERT_TRUE(segment.cut(str, words));

    string s1, s2;
    s1 << words;
    s2 = "[\"小明\", \"硕士\", \"毕业\", \"于\", \"中国\", \"中国科学院\", \"科学\", \"科学院\", \"学院\", \"计算所\", \"，\", \"后\", \"在\", \"日本\", \"京都\", \"京都大学\", \"大学\", \"深造\"]";
    ASSERT_EQ(s1, s2);
  }

  {
    const char* str = "小明硕士毕业于中国科学院计算所iPhone6";
    vector<string> words;

    ASSERT_TRUE(segment.cut(str, words));

    string s1, s2;
    s1 << words;
    s2 = "[\"小明\", \"硕士\", \"毕业\", \"于\", \"中国\", \"中国科学院\", \"科学\", \"科学院\", \"学院\", \"计算所\", \"iPhone6\"]";
    ASSERT_EQ(s1, s2);
  }

}

TEST(LevelSegmentTest, Test0) {
  string s;
  LevelSegment segment("../test/testdata/extra_dict/jieba.dict.small.utf8");
  vector<pair<string, size_t> > words;
  segment.cut("南京市长江大桥", words);
  ASSERT_EQ("[\"南京市:0\", \"长江大桥:0\", \"南京:1\", \"长江:1\", \"大桥:1\"]", s << words);

  vector<string> res;
  segment.cut("南京市长江大桥", res);
  ASSERT_EQ("[\"南京市\", \"长江大桥\", \"南京\", \"长江\", \"大桥\"]", s << res);
}
