/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#include "str_functs.h"

namespace CPPCOMMON
{
	//http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
	std::string string_format(const std::string fmt, ...) 
	{
		int size = 100;
		std::string str;
		va_list ap;
		while (1) {
			str.resize(size);
			va_start(ap, fmt);
			int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
			va_end(ap);
			if (n > -1 && n < size) {
				str.resize(n);
				return str;
			}
			if (n > -1)
				size = n + 1;
			else
				size *= 2;
		}
		return str;
	}

	string joinStr(const vector<string>& src, const string& connectorStr)
	{
		string res = "";
		int len = src.size();
		if(0 == len)
		{
			return "";
		}
		for(int i = 0; i < len - 1; i++)
		{
			res += stripStr(src[i]);
			res += connectorStr;
		}
		res += stripStr(src[len-1]);
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

	string unicodeToUtf8(const vector<uint16_t>& unicode)
	{
		if(unicode.empty())
		{
			return "";
		}

		uint16_t * uniArr = new uint16_t[unicode.size() + 1];
		if(NULL == uniArr)
		{
			return "";
		}
		char * utfStr = new char[unicode.size() * 4 + 1];
		if(NULL == utfStr)
		{
			delete [] uniArr;
			return "";
		}
		for(uint i = 0; i < unicode.size(); i++)
		{
			uniArr[i] = unicode[i];
		}
		
		string res("");
		size_t utfLen = unicodeToUtf8(uniArr, unicode.size(), utfStr);
		if(0 != utfLen)
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


        //*unicode = 0; !! this may cause out range of array;
        return length;
    }

	bool utf8ToUnicode(const string& utfStr, vector<uint16_t>& unicode)
	{
		unicode.clear();
		if(utfStr.empty())
		{
			return false;
		}
		uint16_t* pUni = new uint16_t[utfStr.size() + 1];
		if(NULL == pUni)
		{
			return false;
		}
		size_t uniLen = utf8ToUnicode(utfStr.c_str(), utfStr.size(), pUni);
		for(uint i = 0; i < uniLen; i++)
		{
			unicode.push_back(pUni[i]);
		}
		delete [] pUni;
		return true;
	}

	//iconv
	int code_convert(const char *from_charset,const char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen)
	{
		iconv_t cd;

		char **pin = &inbuf;
		char **pout = &outbuf;

		cd = iconv_open(to_charset,from_charset);
		if (cd==NULL) 
		{
			return -1;
		}
		memset(outbuf,0,outlen);
		size_t ret = iconv(cd,pin,&inlen,pout,&outlen);
		if (ret == -1)
		{
			//cout<<__FILE__<<__LINE__<<endl;
			return -1;
		}
		iconv_close(cd);
		return 0;
	}

	//gbk -> utf8
	string gbkToUtf8(const string& gbk)
	{
		if(gbk.empty())
		{
			return "";
		}
		string res("");
		size_t maxLen = gbk.size()*4 + 1;
		char * pUtf = new char[maxLen];
		if(NULL == pUtf)
		{
			return "";
		}
		int ret = code_convert("gbk", "utf-8", (char *)gbk.c_str(), gbk.size(), pUtf, maxLen);
		if(ret == -1)
		{
			delete [] pUtf;
			return res;
		}
		res = pUtf;
		delete [] pUtf;
		return res;
	}

	//utf8 -> gbk
	string utf8ToGbk(const string& utf)
	{
		//cout<<__FILE__<<__LINE__<<gbk<<endl;
		string res;
		size_t maxLen = utf.size()*4 + 1;
		char * pGbk = new char[maxLen];
		if(NULL == pGbk)
		{
			return "";
		}
		int ret = code_convert("utf-8", "gbk", (char *)utf.c_str(), utf.size(), pGbk, maxLen);
		if(ret == -1)
		{
			delete [] pGbk;
			return "";
		}
		res = pGbk;
		delete [] pGbk;
		return res;
	}

	//unicode str to vec
	bool uniStrToVec(const string& str, vector<uint16_t>& vec)
	{
		vec.clear();
		if(str.empty() || str.size() % 2)
		{
			return false;
		}
		for(uint i = 0; i < str.size(); i+=2)
		{
			vec.push_back(twocharToUint16(str[i], str[i + 1]));
		}

		return true;
	}

	//unicode vec to str
	string uniVecToStr(const vector<uint16_t>& vec)
	{
		string res("");
		for(uint i = 0; i < vec.size(); i++)
		{
			pair<char,char> pa = uint16ToChar2(vec[i]);
			res += pa.first;
			res += pa.second;
		}
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
	//cout<<string_format("hehe%s11asd%dasf","[here]",2);
	ifstream ifile("testdata/dict.gbk");
	string line;
	vector<uint16_t> unicode;
	while(getline(ifile, line))
	{
		cout<<line<<endl;
		utf8ToUnicode(line, unicode);
		printUnicode(unicode);
		cout<<unicodeToUtf8(unicode)<<endl;;
	}
	//vector<string> tmp;
	//tmp.push_back("1");
	////tmp.push_back("2");
	////tmp.clear();
	//cout<<joinStr(tmp, ",")<<endl;
	//ifstream ifile("testdata/dict.gbk");
	//string line;
	//while(getline(ifile, line))
	//{
	//	cout<<line<<endl;
	//	string s = gbkToUtf8(line);
	//	s = utf8ToGbk(s);
	//	cout<<s<<endl;
	//}
	return 0;
}
#endif
