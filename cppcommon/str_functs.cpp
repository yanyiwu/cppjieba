/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
 ************************************/
#include "str_functs.h"

namespace CPPCOMMON
{
    //http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
    string string_format(const string fmt, ...) 
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

    bool splitStr(const string& source, vector<string>& res, const string& pattern)
    {
        if(source.empty())
        {
            return false;
        }
        res.clear();

        size_t start = source.find_first_not_of(pattern);
        size_t end;
        if(string::npos == start)
        {
            return false;
        }
        while(string::npos != start)
        {
            end = source.find_first_of(pattern, start);
            if(string::npos == end)
            {
                res.push_back(source.substr(start));
                return true;
            }
            res.push_back(source.substr(start, end - start));
            start = source.find_first_not_of(pattern,  end);
        }
        return true;
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


    //http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
    // trim from start
    std::string &ltrim(std::string &s) 
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    // trim from end
    std::string &rtrim(std::string &s) 
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    // trim from both ends
    std::string &trim(std::string &s) 
    {
        return ltrim(rtrim(s));
    }

    bool splitStrMultiPatterns(
                const string& strSrc, 
                vector<string>& outVec, 
                const vector<string>& patterns
                )
    {
        char transChar = '#';
        uint transLenThreshold = 10;
        string transStr;
        transStr += transChar;
        while(strSrc.find(transStr) != string::npos)
        {
            transStr += transChar;
            if(transStr.size() > transLenThreshold)
              return false;
        }
        string strSrcMutable = strSrc;
        for(uint i = 0; i < patterns.size(); i++)
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

    //unicode str to vec
    bool uniStrToVec(const string& str, Unicode& vec)
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
    string uniVecToStr(const Unicode& vec)
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
    vector<string> vec;
    splitStr("1 3 4", vec);
    for(uint i =0;i < vec.size(); i++)
    {
        cout<<vec[i]<<endl;
    }
    cout<<strStartsWith("--help","--")<<endl;
    cout<<strStartsWith("--help","-")<<endl;
    cout<<strStartsWith("--help","he")<<endl;
    cout<<strStartsWith("help","help")<<endl;
    cout<<strStartsWith("","help")<<endl;
    cout<<strStartsWith("hel","")<<endl;
    cout<<strEndsWith("hel","")<<endl;
    cout<<strEndsWith("hel","el")<<endl;
    return 0;
}
#endif
