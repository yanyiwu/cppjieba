#include "src/MixSegment.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;

TEST(MixSegmentTest, Test1)
{
    MixSegment segment("../dicts/jieba.dict.utf8", "../dicts/hmm_model.utf8");;
    const char* str = "我来自北京邮电大学。。。学号 123456";
    const char* res[] = {"我", "来自", "北京邮电大学", "。","。","。","。","学号", " 123456"};
    string s;
    vector<string> buf(res, res + sizeof(res)/sizeof(res[0]));
    //for(uint i = 0; i < sizeof(res)/sizeof(res[0]); i++)
    //{
    //    buf.push_back()
    //}
    //buf.push_back("");
    //buf.push_back("你好");
    //buf.push_back("...hh");
    //vector<string> res;
    //uint size = strlen(str);
    //uint offset = 0;
    //while(offset < size)
    //{
    //    uint len;
    //    const char* t =  str + offset;
    //    int ret = filterAscii(t, size - offset, len);
    //    s.assign(t, len);
    //    res.push_back(s);
    //    //cout<<s<<","<<ret<<","<<len<<endl;
    //    //cout<<str<<endl;
    //    offset += len;
    //}
    //EXPECT_EQ(res, buf);
}

//int main(int argc, char** argv)
//{
//    //ChineseFilter chFilter;
//    return 0;
//}

