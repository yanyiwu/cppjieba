#include "src/SegmentBase.hpp"
#include "src/MixSegment.hpp"
#include "src/MPSegment.hpp"
#include "src/HMMSegment.hpp"
#include "src/Limonp/io_functs.hpp"
#include "src/FullSegment.hpp"
#include "src/QuerySegment.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;


//TEST(SegmentBaseTest, Test1)
//{
//    const char* str = "heheh你好...hh";
//    string s;
//    vector<string> buf;
//    buf.push_back("heheh");
//    buf.push_back("你好");
//    buf.push_back("...hh");
//    vector<string> res;
//    size_t size = strlen(str);
//    size_t offset = 0;
//    while(offset < size)
//    {
//        size_t len = 0;
//        const char* t =  str + offset;
//        SegmentBase::filterAscii(t, size - offset, len);
//        s.assign(t, len);
//        res.push_back(s);
//        //cout<<s<<","<<ret<<","<<len<<endl;
//        //cout<<str<<endl;
//        offset += len;
//    }
//    ASSERT_EQ(res, buf);
//}

//int main(int argc, char** argv)
//{
//    //ChineseFilter chFilter;
//    return 0;
//}


TEST(MixSegmentTest, Test1)
{
    MixSegment segment("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8");;
    const char* str = "我来自北京邮电大学。。。学号123456";
    const char* res[] = {"我", "来自", "北京邮电大学", "。","。","。", "学号", "123456"};
    const char* str2 = "B超 T恤";
    const char* res2[] = {"B超"," ", "T恤"};
    vector<string> words;
    ASSERT_TRUE(segment);

    ASSERT_TRUE(segment.cut(str, words));
    ASSERT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
    ASSERT_TRUE(segment.cut(str2, words));
    ASSERT_EQ(words, vector<string>(res2, res2 + sizeof(res2)/sizeof(res2[0])));
}

TEST(MixSegmentTest, UserDict)
{
    MixSegment segment("../dict/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8", "../test/testdata/userdict.utf8");
    //MixSegment segment("../dict/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8", "../dict/extra_dict/jieba.dict.small.utf8");
    ASSERT_TRUE(segment);
    const char* str = "令狐冲是云计算方面的专家";
    vector<string> words;
    ASSERT_TRUE(segment.cut(str, words));
    print(words);
    exit(0);
    
    //* 之前： 李小福 / 是 / 创新 / 办 / 主任 / 也 / 是 / 云 / 计算 / 方面 / 的 / 专家 /
    //    加载自定义词库后：　李小福 / 是 / 创新办 / 主任 / 也 / 是 / 云计算 / 方面 / 的 / 专家 /
}

TEST(MPSegmentTest, Test1)
{
    MPSegment segment("../dict/extra_dict/jieba.dict.small.utf8");;
    const char* str = "我来自北京邮电大学。";
    const char* res[] = {"我", "来自", "北京邮电大学", "。"};
    vector<string> words;
    ASSERT_TRUE(segment);
    ASSERT_TRUE(segment.cut(str, words));
    //print(words);
    ASSERT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
}

TEST(MPSegmentTest, Test2)
{
    MPSegment segment("../dict/extra_dict/jieba.dict.small.utf8");
    string line;
    ifstream ifs("../test/testdata/review.100");
    vector<string> words;

    string eRes;
    loadFile2Str("../test/testdata/review.100.res", eRes);
    string res;
    
    while(getline(ifs, line))
    {
        res += line;
        res += '\n';
        
        segment.cut(line, words);
        string s;
        s << words;
        res += s;
        res += '\n';
    }
    WriteStr2File("../test/testdata/review.100.res", res.c_str(), "w");
    //ASSERT_EQ(res, eRes);
    
}
TEST(HMMSegmentTest, Test1)
{
    HMMSegment segment("../dict/hmm_model.utf8");;
    const char* str = "我来自北京邮电大学。。。学号123456";
    const char* res[] = {"我来", "自北京", "邮电大学", "。", "。", "。", "学号", "123456"};
    vector<string> words;
    ASSERT_TRUE(segment);
    ASSERT_TRUE(segment.cut(str, words));
    ASSERT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
}

TEST(FullSegment, Test1)
{
    FullSegment segment("../dict/extra_dict/jieba.dict.small.utf8");
    const char* str = "我来自北京邮电大学";
    vector<string> words;

    ASSERT_EQ(segment.cut(str, words), true);

    string s;
    s << words;
    ASSERT_EQ(s, "[\"我\", \"来自\", \"北京\", \"北京邮电大学\", \"邮电\", \"电大\", \"大学\"]");
}

TEST(QuerySegment, Test1)
{
    QuerySegment segment("../dict/extra_dict/jieba.dict.small.utf8", "../dict/hmm_model.utf8", 3);
    const char* str = "小明硕士毕业于中国科学院计算所，后在日本京都大学深造";
    vector<string> words;

    ASSERT_TRUE(segment.cut(str, words));

    string s1, s2;
    s1 << words;
    s2 = "[\"小明\", \"硕士\", \"毕业\", \"于\", \"中国\", \"中国科学院\", \"科学\", \"科学院\", \"学院\", \"计算所\", \"，\", \"后\", \"在\", \"日本\", \"京都\", \"京都大学\", \"大学\", \"深造\"]";
    ASSERT_EQ(s1, s2);

}

