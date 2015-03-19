#ifndef HUSKY_HTTP_REQINFO_H
#define HUSKY_HTTP_REQINFO_H

#include <iostream>
#include <string>
#include "Limonp/Logger.hpp"
#include "Limonp/StringUtil.hpp"

namespace Husky {
using namespace Limonp;
using namespace std;

static const char* const KEY_METHOD = "METHOD";
static const char* const KEY_URI = "URI";
static const char* const KEY_PROTOCOL = "PROTOCOL";

typedef unsigned char BYTE;

inline BYTE toHex(BYTE x) {
  return x > 9 ? x -10 + 'A': x + '0';
}

inline BYTE fromHex(BYTE x) {
  return isdigit(x) ? x-'0' : x-'A'+10;
}

inline void URLEncode(const string &sIn, string& sOut) {
  for( size_t ix = 0; ix < sIn.size(); ix++ ) {
    BYTE buf[4];
    memset( buf, 0, 4 );
    if( isalnum( (BYTE)sIn[ix] ) ) {
      buf[0] = sIn[ix];
    } else {
      buf[0] = '%';
      buf[1] = toHex( (BYTE)sIn[ix] >> 4 );
      buf[2] = toHex( (BYTE)sIn[ix] % 16);
    }
    sOut += (char *)buf;
  }
};

inline void URLDecode(const string &sIn, string& sOut) {
  for( size_t ix = 0; ix < sIn.size(); ix++ ) {
    BYTE ch = 0;
    if(sIn[ix]=='%') {
      ch = (fromHex(sIn[ix+1])<<4);
      ch |= fromHex(sIn[ix+2]);
      ix += 2;
    } else if(sIn[ix] == '+') {
      ch = ' ';
    } else {
      ch = sIn[ix];
    }
    sOut += (char)ch;
  }
}

class HttpReqInfo {
 public:
  HttpReqInfo() {
    _isHeaderFinished = false;
    _isBodyFinished = false;
    _contentLength = 0;
  }
 public:
  bool parseHeader(const string& buffer) {
    return parseHeader(buffer.c_str(), buffer.size());
  }
  bool parseHeader(const char* buffer, size_t len) {
    string headerStr(buffer, len);
    size_t lpos = 0, rpos = 0;
    vector<string> buf;
    rpos = headerStr.find("\n", lpos);
    if(string::npos == rpos) {
      LogError("headerStr[%s] illegal.", headerStr.c_str());
      return false;
    }
    string firstline(headerStr, lpos, rpos - lpos);
    trim(firstline);
    if(!split(firstline, buf, " ") || 3 != buf.size()) {
      LogError("parse header firstline[%s] failed.", firstline.c_str());
      return false;
    }
    _headerMap[KEY_METHOD] = trim(buf[0]);
    _headerMap[KEY_URI] = trim(buf[1]);
    _headerMap[KEY_PROTOCOL] = trim(buf[2]);
    _parseUri(_headerMap[KEY_URI], _path,  _methodGetMap);

    lpos = rpos + 1;
    if(lpos >= headerStr.size()) {
      LogError("headerStr[%s] illegal.", headerStr.c_str());
      return false;
    }
    //message header begin
    while(lpos < headerStr.size() && string::npos != (rpos = headerStr.find('\n', lpos)) && rpos > lpos) {
      string s(headerStr, lpos, rpos - lpos);
      size_t p = s.find(':');
      if(string::npos == p) {
        break;//encounter empty line
      }
      string k(s, 0, p);
      string v(s, p+1);
      trim(k);
      trim(v);
      if(k.empty()||v.empty()) {
        LogError("headerStr[%s] illegal.", headerStr.c_str());
        return false;
      }
      upper(k);
      _headerMap[k] = v;
      lpos = rpos + 1;
    }
    rpos ++;
    _isHeaderFinished = true;
    string content_length;
    if(!find("CONTENT-LENGTH", content_length)) {
      _isBodyFinished = true;
      return true;
    }
    _contentLength = atoi(content_length.c_str());
    if(rpos < headerStr.size()) {
      appendBody(headerStr.c_str() + rpos, headerStr.size() - rpos);
    }
    return true;
    //message header end
  }
  void appendBody(const char* buffer, size_t len) {
    if(_isBodyFinished) {
      return;
    }
    _body.append(buffer, len);
    if(_body.size() >= _contentLength) {
      _isBodyFinished = true;
    } else {
      _isBodyFinished = false;
    }
  }
  bool isHeaderFinished() const {
    return _isHeaderFinished;
  }
  bool isBodyFinished() const {
    return _isBodyFinished;
  }
 public:
  const string& set(const string& key, const string& value) {
    return _headerMap[key] = value;
  }
  bool find(const string& key, string& res)const {
    return _find(_headerMap, key, res);
  }
  bool GET(const string& argKey, string& res)const {
    return _find(_methodGetMap, argKey, res);
  }
  //const string& getMethod() const
  //{
  //    return _headerMap.find(KEY_METHOD)->second;
  //}
  bool isGET() const {
    string str;
    if(!_find(_headerMap, KEY_METHOD, str)) {
      return false;
    }
    return str == "GET";
  }
  bool isPOST() const {
    string str;
    if(!_find(_headerMap, KEY_METHOD, str)) {
      return false;
    }
    return str == "POST";
  }
  const unordered_map<string, string> & getMethodGetMap() const {
    return _methodGetMap;
  }
  const unordered_map<string, string> & getHeaders() const {
    return _headerMap;
  }
  const string& getBody() const {
    return _body;
  }
  const string& getPath() const {
    return _path;
  }
 private:
  bool _isHeaderFinished;
  bool _isBodyFinished;
  size_t _contentLength;
  unordered_map<string, string> _headerMap;
  unordered_map<string, string> _methodGetMap;
  string _path;
  string _body;
  friend ostream& operator<<(ostream& os, const HttpReqInfo& obj);
 private:
  bool _find(const std::unordered_map<string, string>& mp, const string& key, string& res)const {
    std::unordered_map<string, string>::const_iterator it = mp.find(key);
    if(it == mp.end()) {
      return false;
    }
    res = it->second;
    return true;
  }
 private:
  void _parseUri(const string& uri, string& path, std::unordered_map<string, string>& mp) {
    if(uri.empty()) {
      return;
    }

    size_t pos = uri.find('?');
    path = uri.substr(0, pos);
    if(string::npos == pos) {
      return ;
    }
    size_t kleft = 0, kright = 0;
    size_t vleft = 0, vright = 0;
    for(size_t i = pos + 1; i < uri.size();) {
      kleft = i;
      while(i < uri.size() && uri[i] != '=') {
        i++;
      }
      if(i >= uri.size()) {
        break;
      }
      kright = i;
      i++;
      vleft = i;
      while(i < uri.size() && uri[i] != '&' && uri[i] != ' ') {
        i++;
      }
      vright = i;
      mp[uri.substr(kleft, kright - kleft)] = uri.substr(vleft, vright - vleft);
      i++;
    }

    return;
  }
};

inline std::ostream& operator << (std::ostream& os, const Husky::HttpReqInfo& obj) {
  return os << obj._headerMap << obj._methodGetMap/* << obj._methodPostMap*/ << obj._path << obj._body ;
}

}

#endif
