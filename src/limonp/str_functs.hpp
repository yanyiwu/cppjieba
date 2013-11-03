/************************************
 * file enc : ascii
 * author   : wuyanyi09@gmail.com
 ************************************/
#ifndef LIMONP_STR_FUNCTS_H
#define LIMONP_STR_FUNCTS_H
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
#include <memory.h>
#include <functional> 
#include <locale>
#include <sstream>
#include <sys/types.h>
#include <map_functs.hpp>

#define print(x) cout<<(x)<<endl

namespace Limonp
{
    using namespace std;
    inline string string_format(const char* fmt, ...) 
    {
        int size = 256;
        std::string str;
        va_list ap;
        while (1) {
            str.resize(size);
            va_start(ap, fmt);
            int n = vsnprintf((char *)str.c_str(), size, fmt, ap);
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

    inline void string_format(string& res, const char* fmt, ...)
    {
        int size = 256;
        va_list ap;
        while (1) {
            res.resize(size);
            va_start(ap, fmt);
            int n = vsnprintf((char *)res.c_str(), size, fmt, ap);
            va_end(ap);
            if (n > -1 && n < size) {
                res.resize(n);
                return;
            }
            if (n > -1)
              size = n + 1;
            else
              size *= 2;
        }
    }

    //inline bool joinStr(const vector<string>& src, string& dest, const string& connectorStr)
    //{
    //    if(src.empty())
    //    {
    //        return false;
    //    }
    //    for(uint i = 0; i < src.size() - 1; i++)
    //    {
    //        dest += src[i];
    //        dest += connectorStr;
    //    }
    //    dest += src[src.size() - 1];
    //    return true;
    //}

    //inline string joinStr(const vector<string>& source, const string& connector)
    //{
    //    string res;
    //    joinStr(source, res, connector);
    //    return res;
    //}

    template<class T>
        void join(T begin, T end, string& res, const string& connector)
        {
            if(begin == end)
            {
                return;
            }
            stringstream ss;
            ss<<*begin;
            begin++;
            while(begin != end)
            {
                ss << connector << *begin;
                begin ++;
            }
            res = ss.str();
        }

    template<class T>
        string join(T begin, T end, const string& connector)
        {
            string res;
            join(begin ,end, res, connector);
            return res;
        }

    template<class T1, class T2>
        ostream& operator << (ostream& os, const pair<T1, T2>& pr)
        {
            os << pr.first << ":" << pr.second ;
            return os;
        }

    template<class T>
        ostream& operator << (ostream& os, const vector<T>& vec)
        {
            if(vec.empty())
            {
                return os << "[]";
            }
            os<<"[\""<<vec[0];
            for(uint i = 1; i < vec.size(); i++)
            {
                os<<"\", \""<<vec[i];
            }
            os<<"\"]";
            return os;
        }

    template<class T>
        string& operator << (string& str, const T& obj)
        {
            stringstream ss;
            ss << obj; // call ostream& operator << (ostream& os,
            return str = ss.str();
        }

    template<class T1, class T2>
        ostream& operator << (ostream& os, const map<T1, T2>& mp)
        {
            if(mp.empty())
            {
                os<<"{}";
                return os;
            }
            os<<'{';
            typename map<T1, T2>::const_iterator it = mp.begin();
            os<<*it;
            it++;
            while(it != mp.end())
            {
                os<<", "<<*it;
                it++;
            }
            os<<'}';
            return os;
        }
    

    //template<class T1, class T2>
    //    string& operator << (string& str, const map<T1, T2>& mp)
    //    {
    //        if(mp.empty())
    //        {
    //            str = "{}";
    //            return str;
    //        }
    //        stringstream ss;
    //        ss<<'{';
    //        typename map<T1, T2>::const_iterator it = mp.begin();
    //        ss<<*it;
    //        it++;
    //        while(it != mp.end())
    //        {
    //            ss<<", "<<*it;
    //                it++;
    //        }
    //        ss<<'}';
    //        str = ss.str();
    //        return str;
    //    }

    template<class T1, class T2>
        ostream& operator << (ostream& os, const HashMap<T1, T2>& mp)
        {
            if(mp.empty())
            {
                return os << "{}";
            }
            os<<'{';
            typename map<T1, T2>::const_iterator it = mp.begin();
            os<<*it;
            it++;
            while(it != mp.end())
            {
                os<<", "<<*it++;
            }
            return os<<'}';
        }

    //template<class T>
    //    string& operator << (string& str, const set<T>& st)
    //    {
    //        stringstream ss;
    //        ss << st;
    //        return str = ss.str();
    //    }

    template<class T>
        ostream& operator << (ostream& os, const set<T>& st)
        {
            if(st.empty())
            {
                os << "{}";
                return os;
            }
            os<<'{';
            typename set<T>::const_iterator it = st.begin();
            os<<*it;
            it++;
            while(it != st.end())
            {
                os<<", "<<*it;
                it++;
            }
            os<<'}';
            return os;
        }

    inline bool splitStr(const string& src, vector<string>& res, const string& pattern)
    {
        if(src.empty())
        {
            return false;
        }
        res.clear();

        size_t start = 0;
        size_t end = 0;
        while(start < src.size())
        {
            end = src.find_first_of(pattern, start);
            if(string::npos == end)
            {
                res.push_back(src.substr(start));
                return true;
            }
            res.push_back(src.substr(start, end - start));
            if(end == src.size() - 1)
            {
                res.push_back("");
                break;
            }
            start = end + 1;
        }
        return true;
    }

    inline string& upper(string& str)
    {
        transform(str.begin(), str.end(), str.begin(), (int (*)(int))toupper);
        return str;
    }

    inline string& lower(string& str)
    {
        transform(str.begin(), str.end(), str.begin(), (int (*)(int))tolower);
        return str;
    }

    inline std::string &ltrim(std::string &s) 
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    inline std::string &rtrim(std::string &s) 
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    inline std::string &trim(std::string &s) 
    {
        return ltrim(rtrim(s));
    }


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

    inline bool strStartsWith(const string& str, const string& prefix)
    {
        //return str.substr(0, prefix.size()) == prefix;
        if(prefix.length() > str.length())
        {
            return false;
        }
        return 0 == str.compare(0, prefix.length(), prefix);
    }

    inline bool strEndsWith(const string& str, const string& suffix)
    {
        if(suffix.length() > str.length())
        {
            return false;
        }
        return 0 == str.compare(str.length() -  suffix.length(), suffix.length(), suffix);
    }

    inline bool isInStr(const string& str, char ch)
    {
        return str.find(ch) != string::npos;
    }

    //inline void extractWords(const string& sentence, vector<string>& words)
    //{
    //    bool flag = false;
    //    uint lhs = 0, len = 0;
    //    for(uint i = 0; i < sentence.size(); i++)
    //    {
    //        char x = sentence[i];
    //        if((0x0030 <= x && x<= 0x0039) || (0x0041 <= x && x <= 0x005a ) || (0x0061 <= x && x <= 0x007a))
    //        {
    //            if(flag)
    //            {
    //                len ++;
    //            }
    //            else
    //            {
    //                lhs = i;
    //                len = 1;
    //            }
    //            flag = true;
    //        }
    //        else
    //        {
    //            if(flag)
    //            {
    //                words.push_back(string(sentence, lhs, len));
    //            }
    //            flag = false;
    //        }
    //    }
    //    if(flag)
    //    {
    //        words.push_back(string(sentence, lhs, len));
    //    }
    //}


}
#endif
