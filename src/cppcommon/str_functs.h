/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#ifndef CPPCOMMON_STR_FUNCTS_H
#define CPPCOMMON_STR_FUNCTS_H
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <map>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <iconv.h>
#include <memory.h>
#include "typedefs.h"
namespace CPPCOMMON
{
	using namespace std;
	std::string string_format(const std::string fmt, ...) ;
	string joinStr(const vector<string>& source, const string& connector);
	vector<string> splitStr(const string& source, const string& pattern = " \t\n");
	void splitStr(const string& source, vector<string>& out_vec, const string& pattern = " \t\n");
	bool splitStrMultiPatterns(
				const string& strSrc, 
				vector<string>& outVec, 
				const vector<string>& patterns
				);
	string upperStr(const string& str);
	string lowerStr(const string& str);
	string replaceStr(const string& strSrc, const string& oldStr, const string& newStr, int count = -1);
	string stripStr(const string& str, const string& patternstr = " \n\t");
	unsigned int countStrDistance(const string& A, const string& B);
	unsigned int countStrSimilarity(const string& A, const string& B);

    //encode
    size_t unicodeToUtf8(uint16_t *in, size_t len, char * out);
	string unicodeToUtf8(const string& uniStr);
    int utf8ToUnicode(const char* inutf8, int len, uint16_t* unicode);
	string utf8ToUnicode(const string& utfStr);
	int code_convert(const char *from_charset,const char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen);
	string gbkToUtf8(const string& gbk);
	string utf8ToGbk(const string& utf);

	size_t getUtf8WordLen(const string& utf);
	
	inline uint16_t twocharToUint16(char high, char low)
	{
		return (((uint16_t(high) & 0x00ff ) << 8) | (uint16_t(low) & 0x00ff));
	}

	inline pair<char, char> uint16ToChar2(uint16_t in)
	{
		pair<char, char> res;
		res.first = (in>>8) & 0x00ff; //high
		res.second = (in) & 0x00ff; //low
		return res;
	}

	
}
#endif
