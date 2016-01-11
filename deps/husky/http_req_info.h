#ifndef HUSKY_HTTP_REQINFO_H
#define HUSKY_HTTP_REQINFO_H

#include <iostream>
#include <string>
#include "limonp/Logging.hpp"
#include "limonp/StringUtil.hpp"

namespace husky {
using namespace limonp;
using namespace std;

static const char* const KEY_METHOD = "METHOD";
static const char* const KEY_URI = "URI";
static const char* const KEY_PROTOCOL = "PROTOCOL";

typedef unsigned char BYTE;

inline BYTE ToHex(BYTE x) {
  return x > 9 ? x -10 + 'A': x + '0';
}

inline BYTE FromHex(BYTE x) {
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
      buf[1] = ToHex( (BYTE)sIn[ix] >> 4 );
      buf[2] = ToHex( (BYTE)sIn[ix] % 16);
    }
    sOut += (char *)buf;
  }
};

inline void URLDecode(const string &sIn, string& sOut) {
  for( size_t ix = 0; ix < sIn.size(); ix++ ) {
    BYTE ch = 0;
    if(sIn[ix]=='%') {
      ch = (FromHex(sIn[ix+1])<<4);
      ch |= FromHex(sIn[ix+2]);
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
    is_header_finished_ = false;
    is_body_finished_ = false;
    content_length_ = 0;
  }

  bool ParseHeader(const string& buffer) {
    return ParseHeader(buffer.c_str(), buffer.size());
  }
  bool ParseHeader(const char* buffer, size_t len) {
    string headerStr(buffer, len);
    size_t lpos = 0, rpos = 0;
    vector<string> buf;
    rpos = headerStr.find("\n", lpos);
    if(string::npos == rpos) {
      LOG(ERROR) << "headerStr[" << headerStr << "] illegal.";
      return false;
    }
    string firstline(headerStr, lpos, rpos - lpos);
    Trim(firstline);
    Split(firstline, buf, " ");
    if (3 != buf.size()) {
      LOG(ERROR) << "parse header firstline [" << firstline << "] failed.";
      return false;
    }
    header_map_[KEY_METHOD] = Trim(buf[0]);
    header_map_[KEY_URI] = Trim(buf[1]);
    header_map_[KEY_PROTOCOL] = Trim(buf[2]);
    ParseUri(header_map_[KEY_URI], path_,  method_get_map_);

    lpos = rpos + 1;
    if(lpos >= headerStr.size()) {
      LOG(ERROR) << "headerStr[" << headerStr << "] illegal.";
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
      Trim(k);
      Trim(v);
      if(k.empty()||v.empty()) {
        LOG(ERROR) << "headerStr[" << headerStr << "] illegal.";
        return false;
      }
      Upper(k);
      header_map_[k] = v;
      lpos = rpos + 1;
    }
    rpos ++;
    is_header_finished_ = true;
    string content_length;
    if(!Find("CONTENT-LENGTH", content_length) || 0 == (content_length_ = atoi(content_length.c_str()))) {
      is_body_finished_ = true;
      return true;
    }
    content_length_ = atoi(content_length.c_str());
    if(rpos < headerStr.size()) {
      AppendBody(headerStr.c_str() + rpos, headerStr.size() - rpos);
    }
    return true;
    //message header end
  }
  void AppendBody(const char* buffer, size_t len) {
    if(is_body_finished_) {
      return;
    }
    body_.append(buffer, len);
    if(body_.size() >= content_length_) {
      is_body_finished_ = true;
    } else {
      is_body_finished_ = false;
    }
  }
  bool IsHeaderFinished() const {
    return is_header_finished_;
  }
  bool IsBodyFinished() const {
    return is_body_finished_;
  }

  const string& Set(const string& key, const string& value) {
    return header_map_[key] = value;
  }
  bool Find(const string& key, string& res)const {
    return Find(header_map_, key, res);
  }
  bool GET(const string& argKey, string& res)const {
    string tmp;
    if (!Find(method_get_map_, argKey, tmp)) {
      return false;
    }
    URLDecode(tmp, res);
    return true;
  }
  bool GET(const string& argKey, int& res) const {
    string tmp;
    if (!GET(argKey, tmp)) {
      return false;
    }
    res = atoi(tmp.c_str());
    return true;
  }
  bool GET(const string& argKey, size_t& res) const {
    int tmp = 0;
    if (!GET(argKey, tmp) || tmp < 0) {
      return false;
    }
    res = tmp;
    return true;
  }

  bool IsGET() const {
    string str;
    if(!Find(header_map_, KEY_METHOD, str)) {
      return false;
    }
    return str == "GET";
  }
  bool IsPOST() const {
    string str;
    if(!Find(header_map_, KEY_METHOD, str)) {
      return false;
    }
    return str == "POST";
  }
  const unordered_map<string, string> & GetMethodGetMap() const {
    return method_get_map_;
  }
  const unordered_map<string, string> & GetHeaders() const {
    return header_map_;
  }
  const string& GetBody() const {
    return body_;
  }
  const string& GetPath() const {
    return path_;
  }

 private:
  bool is_header_finished_;
  bool is_body_finished_;
  size_t content_length_;
  unordered_map<string, string> header_map_;
  unordered_map<string, string> method_get_map_;
  string path_;
  string body_;
  friend ostream& operator<<(ostream& os, const HttpReqInfo& obj);

  bool Find(const std::unordered_map<string, string>& mp, const string& key, string& res)const {
    std::unordered_map<string, string>::const_iterator it = mp.find(key);
    if(it == mp.end()) {
      return false;
    }
    res = it->second;
    return true;
  }

  void ParseUri(const string& uri, string& path, std::unordered_map<string, string>& mp) {
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

inline std::ostream& operator << (std::ostream& os, const husky::HttpReqInfo& obj) {
  return os << obj.header_map_ << obj.method_get_map_/* << obj._methodPostMap*/ << obj.path_ << obj.body_ ;
}

}

#endif
