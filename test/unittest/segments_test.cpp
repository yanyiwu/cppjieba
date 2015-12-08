#include "src/SegmentBase.hpp"
#include "src/MixSegment.hpp"
#include "src/MPSegment.hpp"
#include "src/HMMSegment.hpp"
#include "src/FullSegment.hpp"
#include "src/QuerySegment.hpp"
#include "src/LevelSegment.hpp"
#include "gtest/gtest.h"

using namespace cppjieba;

TEST(MixSegmentTest, Test1) {
  MixSegment segment("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8");;
  string sentence;
  vector<string> words;
  string actual;
  string expected;

  {
    sentence = "我来自北京邮电大学。。。学号123456，用AK47";
    expected = "我/来自/北京邮电大学/。/。/。/学号/123456/，/用/AK47";
    segment.Cut(sentence, words);
    actual = join(words.begin(), words.end(), "/");
    ASSERT_EQ(actual, expected);
  }

  {
    sentence = "B超 T恤";
    expected = "B超/ /T恤";
    segment.Cut(sentence, words);
    actual = join(words.begin(), words.end(), "/");
    ASSERT_EQ(actual, expected);
  }

  {
    sentence = "他来到了网易杭研大厦";
    expected = "他/来到/了/网易/杭/研/大厦";
    segment.Cut(sentence, words, false);
    actual = join(words.begin(), words.end(), "/");
    ASSERT_EQ(actual, expected);
  }

  {
    sentence = "他来到了网易杭研大厦";
    expected = "他/来到/了/网易/杭研/大厦";
    segment.Cut(sentence, words);
    actual = join(words.begin(), words.end(), "/");
    ASSERT_EQ(actual, expected);
  }
}

TEST(MixSegmentTest, NoUserDict) {
  MixSegment segment("../test/testdata/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8");
  const char* str = "令狐冲是云计算方面的专家";
  vector<string> words;
  segment.Cut(str, words);
  string res;
  ASSERT_EQ("[\"令狐冲\", \"是\", \"云\", \"计算\", \"方面\", \"的\", \"专家\"]", res << words);

}
TEST(MixSegmentTest, UserDict) {
  MixSegment segment("../test/testdata/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8", "../dict/user.dict.utf8");
  {
    const char* str = "令狐冲是云计算方面的专家";
    vector<string> words;
    segment.Cut(str, words);
    string res;
    ASSERT_EQ("[\"令狐冲\", \"是\", \"云计算\", \"方面\", \"的\", \"专家\"]", res << words);
  }
  {
    const char* str = "小明先就职于IBM,后在日本京都大学深造";
    vector<string> words;
    segment.Cut(str, words);
    string res;
    res << words;
    ASSERT_EQ("[\"小明\", \"先\", \"就职\", \"于\", \"IBM\", \",\", \"后\", \"在\", \"日本\", \"京都大学\", \"深造\"]", res);
  }
  {
    const char* str = "IBM,3.14";
    vector<string> words;
    segment.Cut(str, words);
    string res;
    res << words;
    ASSERT_EQ("[\"IBM\", \",\", \"3.14\"]", res);
  }
}
TEST(MixSegmentTest, TestUserDict) {
  MixSegment segment("../test/testdata/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8", 
        "../test/testdata/userdict.utf8");
  vector<string> words;
  string res;

  segment.Cut("令狐冲是云计算方面的专家", words);
  ASSERT_EQ("[\"令狐冲\", \"是\", \"云计算\", \"方面\", \"的\", \"专家\"]", res << words);

  segment.Cut("小明先就职于IBM,后在日本京都大学深造", words);
  res << words;
  ASSERT_EQ("[\"小明\", \"先\", \"就职\", \"于\", \"I\", \"B\", \"M\", \",\", \"后\", \"在\", \"日本\", \"京都大学\", \"深造\"]", res);

  segment.Cut("IBM,3.14", words);
  res << words;
  ASSERT_EQ("[\"I\", \"B\", \"M\", \",\", \"3.14\"]", res);

  segment.Cut("忽如一夜春风来，千树万树梨花开", words);
  res = limonp::join(words.begin(), words.end(), "/");
  ASSERT_EQ("忽如一夜春风来/，/千树/万树/梨花/开", res);
}

TEST(MixSegmentTest, TestMultiUserDict) {
  MixSegment segment("../test/testdata/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8", 
        "../test/testdata/userdict.utf8;../test/testdata/userdict.2.utf8");
  vector<string> words;
  string res;

  segment.Cut("忽如一夜春风来，千树万树梨花开", words);
  res = limonp::join(words.begin(), words.end(), "/");
  ASSERT_EQ("忽如一夜春风来/，/千树万树梨花开", res);
}

TEST(MPSegmentTest, Test1) {
  MPSegment segment("../dict/jieba.dict.utf8");;
  string s;
  vector<string> words;
  segment.Cut("我来自北京邮电大学。", words);
  ASSERT_EQ("[\"我\", \"来自\", \"北京邮电大学\", \"。\"]", s << words);

  segment.Cut("B超 T恤", words);
  ASSERT_EQ(s << words, "[\"B超\", \" \", \"T恤\"]");

  segment.Cut("南京市长江大桥", words);
  ASSERT_EQ("[\"南京市\", \"长江大桥\"]", s << words);

  // MaxWordLen
  segment.Cut("南京市长江大桥", words, 3);
  ASSERT_EQ("[\"南京市\", \"长江\", \"大桥\"]", s << words);

  segment.Cut("南京市长江大桥", words, 0);
  ASSERT_EQ("[\"南\", \"京\", \"市\", \"长\", \"江\", \"大\", \"桥\"]", s << words);

  segment.Cut("湖南长沙市天心区", words);
  s = join(words.begin(), words.end(), "/");
  ASSERT_EQ("湖南长沙市/天心区", s);

  segment.Cut("湖南长沙市天心区", words, 3);
  s = join(words.begin(), words.end(), "/");
  ASSERT_EQ("湖南/长沙市/天心区", s);
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
//  while (getline(ifs, line)) {
//    res += line;
//    res += '\n';
//
//    segment.Cut(line, words);
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
    segment.Cut(str, words);
    ASSERT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
  }

  {
    const char* str = "IBM,1.2,123";
    const char* res[] = {"IBM", ",", "1.2", ",", "123"};
    vector<string> words;
    segment.Cut(str, words);
    ASSERT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
  }
}

TEST(FullSegment, Test1) {
  FullSegment segment("../test/testdata/extra_dict/jieba.dict.small.utf8");
  vector<string> words;
  string s;

  segment.Cut("我来自北京邮电大学", words);
  s << words;
  ASSERT_EQ(s, "[\"我\", \"来自\", \"北京\", \"北京邮电大学\", \"邮电\", \"电大\", \"大学\"]");

  
  segment.Cut("上市公司CEO", words);
  s << words;
  ASSERT_EQ(s, "[\"上市\", \"公司\", \"C\", \"E\", \"O\"]");
}

TEST(QuerySegment, Test1) {
  QuerySegment segment("../test/testdata/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8", "", 3);
  const char* str = "小明硕士毕业于中国科学院计算所，后在日本京都大学深造";
  vector<string> words;

  segment.Cut(str, words);

  string s1, s2;
  s1 << words;
  s2 = "[\"小明\", \"硕士\", \"毕业\", \"于\", \"中国\", \"中国科学院\", \"科学\", \"科学院\", \"学院\", \"计算所\", \"，\", \"后\", \"在\", \"日本\", \"京都\", \"京都大学\", \"大学\", \"深造\"]";
  ASSERT_EQ(s1, s2);

}

TEST(QuerySegment, Test2) {
  QuerySegment segment("../test/testdata/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8", "../test/testdata/userdict.utf8|../test/testdata/userdict.english", 3);

  {
    const char* str = "小明硕士毕业于中国科学院计算所，后在日本京都大学深造";
    vector<string> words;

    segment.Cut(str, words);

    string s1, s2;
    s1 << words;
    s2 = "[\"小明\", \"硕士\", \"毕业\", \"于\", \"中国\", \"中国科学院\", \"科学\", \"科学院\", \"学院\", \"计算所\", \"，\", \"后\", \"在\", \"日本\", \"京都\", \"京都大学\", \"大学\", \"深造\"]";
    ASSERT_EQ(s1, s2);
  }

  {
    const char* str = "小明硕士毕业于中国科学院计算所iPhone6";
    vector<string> words;

    segment.Cut(str, words);

    string s1, s2;
    s1 << words;
    s2 = "[\"小明\", \"硕士\", \"毕业\", \"于\", \"中国\", \"中国科学院\", \"科学\", \"科学院\", \"学院\", \"计算所\", \"iPhone6\"]";
    ASSERT_EQ(s1, s2);
  }

  {
    vector<string> words;
    segment.Cut("internal", words);
    string s = join(words.begin(), words.end(), "/");
    ASSERT_EQ("internal", s);
  }

  segment.SetMaxWordLen(5);

  {
    vector<string> words;
    segment.Cut("中国科学院", words);
    string s = join(words.begin(), words.end(), "/");
    ASSERT_EQ("中国科学院", s);
  }
}

TEST(LevelSegmentTest, Test0) {
  string s;
  LevelSegment segment("../test/testdata/extra_dict/jieba.dict.small.utf8");
  vector<pair<string, size_t> > words;
  segment.Cut("南京市长江大桥", words);
  ASSERT_EQ("[\"南京市:0\", \"长江大桥:0\", \"南京:1\", \"长江:1\", \"大桥:1\"]", s << words);

  vector<string> res;
  segment.Cut("南京市长江大桥", res);
  ASSERT_EQ("[\"南京市\", \"长江大桥\", \"南京\", \"长江\", \"大桥\"]", s << res);
}
