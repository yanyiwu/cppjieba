#ifndef CPPJIEBA_UNICODE_H
#define CPPJIEBA_UNICODE_H

#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "limonp/LocalVector.hpp"

namespace cppjieba {

typedef uint32_t Rune;

struct RuneStr {
  Rune rune;
  const char* str;
  uint32_t len;
  RuneStr(): rune(0), str(NULL), len(0) {
  }
  RuneStr(Rune r, const char* s, uint32_t l)
    : rune(r), str(s), len(l) {
  }
}; // struct RuneStr


typedef limonp::LocalVector<Rune> Unicode;
typedef limonp::LocalVector<struct RuneStr> RuneStrArray;

// [left, right]
struct WordRange {
  RuneStrArray::const_iterator left;
  RuneStrArray::const_iterator right;
  WordRange(RuneStrArray::const_iterator l, RuneStrArray::const_iterator r)
   : left(l), right(r) {
  }
  size_t Length() const {
    return right - left + 1;
  }
  bool IsAllAscii() const {
    for (RuneStrArray::const_iterator iter = left; iter <= right; ++iter) {
      if (iter->rune >= 0x80) {
        return false;
      }
    }
    return true;
  }
}; // struct WordRange

struct RuneStrLite {
  uint32_t rune;
  uint32_t len;
  RuneStrLite(): rune(0), len(0) {
  }
  RuneStrLite(uint32_t r, uint32_t l): rune(r), len(l) {
  }
}; // struct RuneStrLite

inline RuneStrLite DecodeRuneInString(const char* str, size_t len) {
  RuneStrLite rp(0, 0);
  if (str == NULL || len == 0) {
    return rp;
  }
  if (!(str[0] & 0x80)) { // 0xxxxxxx
    // 7bit, total 7bit
    rp.rune = (uint8_t)(str[0]) & 0x7f;
    rp.len = 1;
  } else if ((uint8_t)str[0] <= 0xdf &&  1 < len) { 
    // 110xxxxxx
    // 5bit, total 5bit
    rp.rune = (uint8_t)(str[0]) & 0x1f;

    // 6bit, total 11bit
    rp.rune <<= 6;
    rp.rune |= (uint8_t)(str[1]) & 0x3f;
    rp.len = 2;
  } else if((uint8_t)str[0] <= 0xef && 2 < len) { // 1110xxxxxx
    // 4bit, total 4bit
    rp.rune = (uint8_t)(str[0]) & 0x0f;

    // 6bit, total 10bit
    rp.rune <<= 6;
    rp.rune |= (uint8_t)(str[1]) & 0x3f;

    // 6bit, total 16bit
    rp.rune <<= 6;
    rp.rune |= (uint8_t)(str[2]) & 0x3f;

    rp.len = 3;
  } else if((uint8_t)str[0] <= 0xf7 && 3 < len) { // 11110xxxx
    // 3bit, total 3bit
    rp.rune = (uint8_t)(str[0]) & 0x07;

    // 6bit, total 9bit
    rp.rune <<= 6;
    rp.rune |= (uint8_t)(str[1]) & 0x3f;

    // 6bit, total 15bit
    rp.rune <<= 6;
    rp.rune |= (uint8_t)(str[2]) & 0x3f;

    // 6bit, total 21bit
    rp.rune <<= 6;
    rp.rune |= (uint8_t)(str[3]) & 0x3f;

    rp.len = 4;
  } else {
    rp.rune = 0;
    rp.len = 0;
  }
  return rp;
}

inline bool DecodeRunesInString(const char* s, size_t len, RuneStrArray& runes) {
  runes.clear();
  runes.reserve(len / 2);
  for (size_t i = 0; i < len;) {
    RuneStrLite rp = DecodeRuneInString(s + i, len - i);
    if (rp.len == 0) {
      return false;
    }
    RuneStr x(rp.rune, s + i, rp.len);
    runes.push_back(x);
    i += rp.len;
  }
  return true;
}

inline bool DecodeRunesInString(const std::string& s, RuneStrArray& runes) {
  return DecodeRunesInString(s.c_str(), s.size(), runes);
}

inline bool DecodeRunesInString(const char* s, size_t len, Unicode& unicode) {
  unicode.clear();
  RuneStrArray runes;
  if (!DecodeRunesInString(s, len, runes)) {
    return false;
  }
  unicode.reserve(runes.size());
  for (size_t i = 0; i < runes.size(); i++) {
    unicode.push_back(runes[i].rune);
  }
  return true;
}

inline bool IsSingleWord(const std::string& str) {
  RuneStrLite rp = DecodeRuneInString(str.c_str(), str.size());
  return rp.len == str.size();
}

inline bool DecodeRunesInString(const std::string& s, Unicode& unicode) {
  return DecodeRunesInString(s.c_str(), s.size(), unicode);
}

inline Unicode DecodeRunesInString(const std::string& s) {
  Unicode result;
  DecodeRunesInString(s, result);
  return result;
}


//[left, right]
inline std::string GetStringFromRunes(RuneStrArray::const_iterator left, RuneStrArray::const_iterator right) {
  assert(right->str >= left->str);
  return std::string(left->str, right->str - left->str + right->len);
}

inline void GetStringsFromWordRanges(const std::vector<WordRange>& wrs, std::vector<std::string>& words) {
  for (size_t i = 0; i < wrs.size(); i++) {
    words.push_back(GetStringFromRunes(wrs[i].left, wrs[i].right));
  }
}

inline std::vector<std::string> GetStringsFromWordRanges(const std::vector<WordRange>& wrs) {
  std::vector<std::string> result;
  GetStringsFromWordRanges(wrs, result);
  return result;
}

} // namespace cppjieba

#endif // CPPJIEBA_UNICODE_H
