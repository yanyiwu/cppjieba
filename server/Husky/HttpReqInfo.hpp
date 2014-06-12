#ifndef HUSKY_HTTP_REQINFO_H
#define HUSKY_HTTP_REQINFO_H

#include <iostream>
#include <string>
#include "Limonp/Logger.hpp"
#include "Limonp/StringUtil.hpp"
#include "Limonp/InitOnOff.hpp"

namespace Husky
{
    using namespace Limonp;
    using namespace std;

    static const char* const KEY_METHOD = "METHOD";
    static const char* const KEY_PATH = "PATH";
    static const char* const KEY_PROTOCOL = "PROTOCOL";

    typedef unsigned char BYTE;

    inline BYTE toHex(BYTE x)
    {
        return x > 9 ? x -10 + 'A': x + '0';
    }

    inline BYTE fromHex(BYTE x)
    {
        return isdigit(x) ? x-'0' : x-'A'+10;
    }

    inline void URLEncode(const string &sIn, string& sOut)
    {
        for( size_t ix = 0; ix < sIn.size(); ix++ )
        {      
            BYTE buf[4];
            memset( buf, 0, 4 );
            if( isalnum( (BYTE)sIn[ix] ) )
            {      
                buf[0] = sIn[ix];
            }
            else
            {
                buf[0] = '%';
                buf[1] = toHex( (BYTE)sIn[ix] >> 4 );
                buf[2] = toHex( (BYTE)sIn[ix] % 16);
            }
            sOut += (char *)buf;
        }
    };

    inline void URLDecode(const string &sIn, string& sOut)
    {
        for( size_t ix = 0; ix < sIn.size(); ix++ )
        {
            BYTE ch = 0;
            if(sIn[ix]=='%')
            {
                ch = (fromHex(sIn[ix+1])<<4);
                ch |= fromHex(sIn[ix+2]);
                ix += 2;
            }
            else if(sIn[ix] == '+')
            {
                ch = ' ';
            }
            else
            {
                ch = sIn[ix];
            }
            sOut += (char)ch;
        }
    }

    class HttpReqInfo: public InitOnOff
    {
        public:
            HttpReqInfo(const string& headerStr)
            {
                _setInitFlag(_init(headerStr));
            }
        private:
            bool _init(const string& headerStr)
            {
                size_t lpos = 0, rpos = 0;
                vector<string> buf;
                rpos = headerStr.find("\n", lpos);
                if(string::npos == rpos)
                {
                    LogError("headerStr[%s] illegal.", headerStr.c_str());
                    return false;
                }
                string firstline(headerStr, lpos, rpos - lpos);
                trim(firstline);
                if(!split(firstline, buf, " ") || 3 != buf.size())
                {
                    LogError("parse header firstline[%s] failed.", firstline.c_str());
                    return false;
                }
                _headerMap[KEY_METHOD] = trim(buf[0]); 
                _headerMap[KEY_PATH] = trim(buf[1]); 
                _headerMap[KEY_PROTOCOL] = trim(buf[2]); 
                //first request line end
                //parse path to _methodGetMap
                if("GET" == _headerMap[KEY_METHOD])
                {
                    _parseUrl(firstline, _methodGetMap);
                }

                lpos = rpos + 1;
                if(lpos >= headerStr.size())
                {
                    LogError("headerStr[%s] illegal.", headerStr.c_str());
                    return false;
                }
                //message header begin
                while(lpos < headerStr.size() && string::npos != (rpos = headerStr.find('\n', lpos)) && rpos > lpos)
                {
                    string s(headerStr, lpos, rpos - lpos);
                    size_t p = s.find(':');
                    if(string::npos == p)
                    {
                        break;//encounter empty line
                    }
                    string k(s, 0, p);
                    string v(s, p+1);
                    trim(k);
                    trim(v);
                    if(k.empty()||v.empty())
                    {
                        LogError("headerStr[%s] illegal.", headerStr.c_str());
                        return false;
                    }
                    upper(k);
                    _headerMap[k] = v;
                    lpos = rpos + 1;
                }
                //message header end

                //body begin
                _body.assign(headerStr.substr(rpos));
                trim(_body);
                return true;
            }
        
        public:
            //string& operator[] (const string& key)
            //{
            //    return _headerMap[key];
            //}
            const string& set(const string& key, const string& value)
            {
                return _headerMap[key] = value;
            }
            bool find(const string& key, string& res)const
            {
                return _find(_headerMap, key, res);
            }
            bool GET(const string& argKey, string& res)const
            {
                return _find(_methodGetMap, argKey, res);
            }
            const string& getMethod() const
            {
                return _headerMap.find(KEY_METHOD)->second;
            }
            const unordered_map<string, string> & getMethodGetMap() const
            {
                return _methodGetMap;
            }
            const unordered_map<string, string> & getHeaders() const
            {
                return _headerMap;
            }
            const string& getBody() const
            {
                return _body;
            }
        private:
            unordered_map<string, string> _headerMap;
            unordered_map<string, string> _methodGetMap;
            string _body;
            friend ostream& operator<<(ostream& os, const HttpReqInfo& obj);
        private:
            bool _find(const std::unordered_map<string, string>& mp, const string& key, string& res)const
            {
                std::unordered_map<string, string>::const_iterator it = mp.find(key);
                if(it == mp.end())
                {
                    return false;
                }
                res = it->second;
                return true;
            }
        private:
            bool _parseUrl(const string& url, std::unordered_map<string, string>& mp)
            {
                if(url.empty())
                {
                    return false;
                }

                size_t pos = url.find('?');
                if(string::npos == pos)
                {
                    return false;
                }
                size_t kleft = 0, kright = 0;
                size_t vleft = 0, vright = 0;
                for(size_t i = pos + 1; i < url.size();)
                {
                    kleft = i;
                    while(i < url.size() && url[i] != '=')
                    {
                        i++;
                    }
                    if(i >= url.size())
                    {
                        break;
                    }
                    kright = i;
                    i++;
                    vleft = i;
                    while(i < url.size() && url[i] != '&' && url[i] != ' ')
                    {
                        i++;
                    }
                    vright = i;
                    mp[url.substr(kleft, kright - kleft)] = url.substr(vleft, vright - vleft);
                    i++;
                }

                return true;
            }
    };

    inline std::ostream& operator << (std::ostream& os, const Husky::HttpReqInfo& obj)
    {
        return os << obj._headerMap << obj._methodGetMap/* << obj._methodPostMap*/ << obj._body;
    }

}

#endif
