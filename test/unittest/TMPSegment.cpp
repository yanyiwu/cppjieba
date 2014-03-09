#include "src/MPSegment.hpp"
#include "src/Limonp/io_functs.hpp"
#include "gtest/gtest.h"

using namespace CppJieba;
using namespace Limonp;

TEST(MPSegmentTest, Test1)
{
    MPSegment segment("../dict/jieba.dict.utf8");;
    const char* str = "我来自北京邮电大学。。。  学号 123456";
    const char* res[] = {"我", "来自", "北京邮电大学", "。","。","。","  ","学","号", " 123456"};
    vector<string> words;
    ASSERT_TRUE(segment);
    ASSERT_TRUE(segment.cut(str, words));
    //print(words);
    EXPECT_EQ(words, vector<string>(res, res + sizeof(res)/sizeof(res[0])));
}

TEST(MPSegmentTest, Test2)
{
    MPSegment segment("../dict/jieba.dict.utf8");
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
        
        words.clear();
        segment.cut(line, words);
        string s;
        s << words;
        res += s;
        res += '\n';
    }
    WriteStr2File("../test/testdata/review.100.res", res.c_str(), "w");
    //ASSERT_EQ(res, eRes);
    
}
