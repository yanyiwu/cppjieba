#include "str_functs.h"

namespace CPPCOMMON
{
	string joinStr(const vector<string>& src, const string& connectorStr)
	{
		string res;
		string tmpStr;
		size_t len = src.size();
		for(size_t i = 0; i < len - 1; i++)
		{
			res += stripStr(src[i]);
			res += connectorStr;
		}
		if(0 < len)
		{
			res +=  stripStr(src[len-1]);
		}
		return res;
	}
	vector<string> splitStr(const string& source, const string& pattern)
	{
		vector<string> res;
		splitStr(source, res, pattern);
		return res;
	}
	void splitStr(const string& source, vector<string>& out_vec, const string& pattern)
	{
		if(0 == pattern.size())
		{
			return;
		}
		string s = source + pattern;
		string::size_type pos;
		int length = s.size();

		for(int i = 0; i < length; i++)
		{
			pos = s.find(pattern, i);
			if(pos < length)
			{
				string tmp = stripStr(s.substr(i, pos - i));
				if("" != tmp)
				{
					out_vec.push_back(tmp);
				}
				i = pos + pattern.size() - 1;
			}
		}
	}

	string stripStr(const string& str, const string& patternStr)
	{
		if(str.empty())
		{
			return str;
		}
		string::size_type posL = str.find_first_not_of(patternStr);
		if(string::npos == posL)
		{
			return str;
		}
		string::size_type posR = str.find_last_not_of(patternStr);
		return str.substr(posL, posR - posL + 1);

	}
	
	bool splitStrMultiPatterns(
				const string& strSrc, 
				vector<string>& outVec, 
				const vector<string>& patterns
				)
	{
		char transChar = '#';
		int transLenThreshold = 10;
		string transStr;
		transStr += transChar;
		while(strSrc.find(transStr) != string::npos)
		{
			transStr += transChar;
			if(transStr.size() > transLenThreshold)
			  return false;
		}
		string strSrcMutable = strSrc;
		for(int i = 0; i < patterns.size(); i++)
		{
			strSrcMutable = replaceStr(strSrcMutable, patterns[i], transStr);
		}
		splitStr(strSrcMutable, outVec, transStr);
		return true;
	}

	string upperStr(const string& strIn)
	{
		string str = strIn;
		transform(str.begin(), str.end(), str.begin(), (int (*)(int))toupper);
		return str;
	}

	string lowerStr(const string& strIn)
	{
		string str = strIn;
		transform(str.begin(), str.end(), str.begin(), (int (*)(int))tolower);
		return str;
	}

	string replaceStr(const string& strSrc, const string& oldStr, const string& newStr, int count)
	{
		string strRet = strSrc;
		size_t pos = 0;
		int l_count = 0;
		if(-1 == count)
		  count = strRet.size();
		while((pos = strRet.find(oldStr, pos)) != string::npos)
		{
			strRet.replace(pos, oldStr.size(), newStr);
			if(++l_count >= count)
			  break;
			pos += newStr.size();
		}
		return strRet;
	}

	unsigned int countStrDistance(const string& A, const string& B)
	{
		unsigned int lenA = A.size();
		unsigned int lenB = B.size();
		unsigned int len = (lenA < lenB ? lenA : lenB);
		unsigned int res = lenA + lenB - 2 * len;
		for(size_t i = 0; i < len; i++)
		{
			if(A[i] != B[i])
			  res++;
		}
		return res;
	}

	unsigned int countStrSimilarity(const string& A, const string& B)
	{
		unsigned int lenA = A.size();
		unsigned int lenB = B.size();
		unsigned int len = (lenA < lenB ? lenA : lenB);
		unsigned int res = 0;
		for(size_t i = 0; i < len; i++)
		{
			if(A[i] == B[i])
			  res++;
		}
		return res;
	}

    //unicode utf8 transform
    size_t unicodeToUtf8(uint16_t *in, size_t len, char * out)
    {
        size_t res = 0;
        for (int i = 0; i < len; i++)
        {
            uint16_t unicode = in[i];
            if (unicode >= 0x0000 && unicode <= 0x007f)
            {
                *out = (uint8_t)unicode;
                out += 1;
                res += 1;
            }
            else if (unicode >= 0x0080 && unicode <= 0x07ff)
            {
                *out = 0xc0 | (unicode >> 6);
                out += 1;
                *out = 0x80 | (unicode & (0xff >> 2));
                out += 1;
                res += 2;
            }
            else if (unicode >= 0x0800 && unicode <= 0xffff)
            {
                *out = 0xe0 | (unicode >> 12);
                out += 1;
                *out = 0x80 | ((unicode >> 6) & 0x3f);
                out += 1;
                *out = 0x80 | (unicode & 0x3f);
                out += 1;
                res += 3;
            }

        }
        *out = '\0';
        return res;
    }

	string unicodeToUtf8(const string& uniStr)
	{
		size_t len = uniStr.size();
		if(len%2)
		{
			return "";
		}

		uint16_t * uniArr = new uint16_t[len>>1];
		char * utfStr = new char[len<<1];
		for(int i = 0; i < len; i+=2)
		{
			uint16_t tmp1 = uniStr[i];
			tmp1 <<= 8;
			tmp1&= 0xff00;
			uint16_t tmp2 = uniStr[i+1];
			tmp2 &= 0x00ff;
			uniArr[i>>1] = tmp1 | tmp2;
		}
		
		string res;
		size_t utfLen = unicodeToUtf8(uniArr, len>>1, utfStr);
		if(0 == utfLen)
		{
			res = "";
		}
		else
		{
			res = utfStr;
		}
		delete [] uniArr;
		delete [] utfStr;
		return res;
	}

    /*from: http://www.cppblog.com/lf426/archive/2008/03/31/45796.html */
    int utf8ToUnicode(const char* inutf8, int len, uint16_t* unicode)
    {
        int length;
        const unsigned char* utf8 = (const unsigned char*) inutf8;
        const unsigned char* t = (const unsigned char*) inutf8;

        length = 0;
        while (utf8 - t < len)
        {
            if ( *(unsigned char *) utf8 <= 0x7f ) 
            {
                //expand with 0s.
                *unicode++ = *utf8++;
            }
            //2 byte.
            else if ( *(unsigned char *) utf8 <= 0xdf ) 
            {
                *unicode++ = ((*(unsigned char *) utf8 & 0x1f) << 6) + ((*(unsigned char *) (utf8 + 1)) & 0x3f);
                utf8 += 2;
            }
            //3 byte.Chinese may use 3 byte.
            else {
                *unicode++ = ((int) (*(unsigned char *) utf8 & 0x0f) << 12) +
                    ((*(unsigned char *) (utf8 + 1) & 0x3f) << 6) +
                    (*(unsigned char *) (utf8 + 2) & 0x3f);
                utf8 += 3;
            }
            length++;
        }

        *unicode = 0;
        return length;
    }

	string utf8ToUnicode(const string& utfStr)
	{
		uint16_t* pUni = new uint16_t[utfStr.size()];
		size_t uniLen = utf8ToUnicode(utfStr.c_str(), utfStr.size(), pUni);
		string res;
		if(uniLen ==0 )
		{
			res = "";
		}
		else
		{
			for(uint i = 0; i < uniLen; i++)
			{
				char c = 0;
				c = ((pUni[i]>>8) & 0x00ff);
				res += c;
				c = (pUni[i] & 0x00ff);
				res += c;
			}
		}
		delete [] pUni;
		return res;
	}

}

#ifdef TEST_STR_FUNCTS
#include <iostream>
using namespace CPPCOMMON;
using namespace std;
int main()
{
	//string s = " \t\n1 a h \n";
	//cout<<"["<<stripStr(s)<<"]"<<endl;
	//cout<<countStrDistance("Aheheh","heheh1212")<<endl;
	//cout<<joinStr(splitStr(s), ",")<<endl;
	//vector<string> vec;
	//splitStr("1 3 4", vec);
	//char * a[] = {"3","jaj","ads"};
	//vector<string> pats(a,a+3);
	//vec.clear();
	//splitStrMultiPattern("1 #3 jajs5 asdf3ads 4", vec, pats);
	//for(int i=0;i<vec.size();i++)
	//{
	//	cout<<vec[i]<<endl;
	//}
	//string s = "1111aaafasfa,asdj.sadhashfhaha";
	//upperStr(s);
	//cout<<s<<endl;
	//
	//s = "ab1ba2ab3";
	//cout<<replaceStr(s,"ab","###")<<endl;
    //ifstream ifile("testdata/dict.txt");
    //string line;
    //while(getline(ifile, line))
    //{
    //    uint16_t strbuf[1024];

    //    size_t unilen = utf8ToUnicode(line.c_str(), line.size(), strbuf);
    //    for(int i = 0; i < unilen; i++)
    //    {
    //        // printf("%x\n", strbuf[i]);
    //    }
    //    char utf8str[512]={0};
    //    unicodeToUtf8(strbuf, unilen, utf8str);
    //    //cout<<strlen(utf8str);
    //    cout<<utf8str<<endl;
    //}
	ifstream ifile("jieba.dict.utf8");
	string line;
	while(getline(ifile, line))
	{
		cout<<line<<endl;
		string uniStr = utf8ToUnicode(line);
		//cout<<uniStr<<endl;
		string utfStr = unicodeToUtf8(uniStr);
		cout<<utfStr<<endl;
	}
	getchar();
	return 0;
}
#endif
