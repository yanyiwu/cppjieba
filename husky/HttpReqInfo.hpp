#ifndef HUSKY_HTTP_REQINFO_H
#define HUSKY_HTTP_REQINFO_H

#include <iostream>
#include <string>
#include "globals.h"
#include <str_functs.hpp>
#include <logger.hpp>
#include <map_functs.hpp>

namespace Husky
{
    using namespace Limonp;

    static const char* const KEY_METHOD = "METHOD";
    static const char* const KEY_PATH = "PATH";
    static const char* const KEY_PROTOCOL = "PROTOCOL";

    class HttpReqInfo
    {
        public:
            bool load(const string& headerStr)
            {
                size_t lpos = 0, rpos = 0;
                vector<string> buf;
                rpos = headerStr.find("\n", lpos);
                if(string::npos == rpos)
                {
                    LogFatal("headerStr illegal.");
                    return false;
                }
                string firstline(headerStr, lpos, rpos - lpos);
                trim(firstline);
                if(!splitStr(firstline, buf, " ") || 3 != buf.size())
                {
                    LogFatal("parse header first line failed.");
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
                    LogFatal("headerStr illegal");
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
                        LogFatal("headerStr illegal.");
                        return false;
                    }
                    _headerMap[upperStr(k)] = v;
                    lpos = rpos + 1;
                }
                //message header end

                //body begin
                
                return true;
            }
        public:
            string& operator[] (const string& key)
            {
                return _headerMap[key];
            }
            bool find(const string& key, string& res)const
            {
                return _find(_headerMap, key, res);
            }
            bool GET(const string& argKey, string& res)const
            {
                return _find(_methodGetMap, argKey, res);
            }
            bool POST(const string& argKey, string& res)const
            {
                return _find(_methodPostMap, argKey, res);
            }
        private:
            HashMap<string, string> _headerMap;
            HashMap<string, string> _methodGetMap;
            HashMap<string, string> _methodPostMap;
        private:
            bool _find(const HashMap<string, string>& mp, const string& key, string& res)const
            {
                HashMap<string, string>::const_iterator it = mp.find(key);
                if(it == mp.end())
                {
                    return false;
                }
                res = it->second;
                return true;
            }
        public:
            //string toString() const;// function for debug because of heavy time consuming
            string toString() const
            {
                string res("{");
                res += HashMapToString(_headerMap);
                res += ",";
                res += HashMapToString(_methodGetMap);
                res += ",";
                res += HashMapToString(_methodPostMap);
                res += "}";
                return res;
            }
        private:
            bool _parseUrl(const string& url, HashMap<string, string>& mp)
            {
                if(url.empty())
                {
                    return false;
                }

                uint pos = url.find('?');
                if(string::npos == pos)
                {
                    return false;
                }
                uint kleft = 0, kright = 0;
                uint vleft = 0, vright = 0;
                for(uint i = pos + 1; i < url.size();)
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

}

#endif
