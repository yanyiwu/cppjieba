#ifndef CPPCOMMON_STR_FUNCTS_H
#define CPPCOMMON_STR_FUNCTS_H
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <stdint.h>
namespace CPPCOMMON
{
	using namespace std;
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

    
    size_t unicodeToUtf8(uint16_t *in, size_t len, char * out);
    int utf8ToUnicode(const char* inutf8, int len, uint16_t* unicode);
}
#endif
