#ifndef CPPCOMMON_HTTP_FUNCTS_H
#define CPPCOMMON_HTTP_FUNCTS_H

#include <iostream>
#include <string>
#include "typedefs.h"
#include "map_functs.h"
#include "str_functs.h"
#include "logger.h"

namespace CPPCOMMON
{
    using namespace std;

    class HttpReqInfo
    {
        public:
            static const char* const KEY_METHOD;
            static const char* const KEY_PATH;
            static const char* const KEY_PROTOCOL;
        public:
            bool load(const string& headerStr);
        private:
            //bool _parse(const string& headerStr, size_t lpos, size_t rpos, const char * const key);
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
            bool _find(const HashMap<string, string>& mp, const string& key, string& res) const;
        public:
            string toString() const;// function for debug because of heavy time consuming
    };

    bool parseUrl(const string& url, HashMap<string, string>& mp);
}

#endif
