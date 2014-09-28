#include "src/PosTagger.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

const char * const QUERY_TEST1 = "我是蓝翔技工拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上总经理，出任CEO，迎娶白富美，走上人生巅峰。";
const char * const ANS_TEST1 = "[\"我:r\", \"是:v\", \"蓝翔:x\", \"技工:n\", \"拖拉机:n\", \"学院:n\", \"手扶拖拉机:n\", \"专业:n\", \"的:uj\", \"。:x\", \"不用:v\", \"多久:m\", \"，:x\", \"我:r\", \"就:d\", \"会:v\", \"升职:v\", \"加薪:nr\", \"，:x\", \"当上:t\", \"总经理:n\", \"，:x\", \"出任:v\", \"CEO:x\", \"，:x\", \"迎娶:v\", \"白富:x\", \"美:ns\", \"，:x\", \"走上:v\", \"人生:n\", \"巅峰:n\", \"。:x\"]";

TEST(PosTaggerTest, Test1)
{
    PosTagger tagger("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8");
    vector<pair<string, string> > res;
    tagger.tag(QUERY_TEST1, res);
    string s;
    s << res;
    ASSERT_TRUE(s == ANS_TEST1);
}
