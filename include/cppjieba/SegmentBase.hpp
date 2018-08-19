#ifndef CPPJIEBA_SEGMENTBASE_H
#define CPPJIEBA_SEGMENTBASE_H

#include "limonp/Logging.hpp"
#include "PreFilter.hpp"
#include <cassert>
// #include "Unicode.hpp"

namespace cppjieba {

const char* const SPECIAL_SEPARATORS = " \t\n\xEF\xBC\x8C\xE3\x80\x82";

using namespace limonp;

class SegmentBase {
 public:
  SegmentBase() {
    XCHECK(ResetSeparators(SPECIAL_SEPARATORS));
  }
  virtual ~SegmentBase() {
  }

  virtual void Cut(const string& sentence, vector<string>& words) const = 0;

  bool ResetSeparators(const string& s) {
    symbols_.clear();
    RuneStrArray runes;
    if (!DecodeRunesInString(s, runes)) {
      XLOG(ERROR) << "decode " << s << " failed";
      return false;
    }
    for (size_t i = 0; i < runes.size(); i++) {
      if (!symbols_.insert(runes[i].rune).second) {
        XLOG(ERROR) << s.substr(runes[i].offset, runes[i].len) << " already exists";
        return false;
      }
    }
    return true;
  }
 protected:
  unordered_set<Rune> symbols_;
 
  // sequential letters rule
RuneStrArray::const_iterator SequentialLetterRule(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end) const {
  Rune x = begin->rune;
  if (('a' <= x && x <= 'z') || ('A' <= x && x <= 'Z')) {
    begin ++;
  } else {
    return begin;
  }
  while (begin != end) {
    x = begin->rune;
    if (('a' <= x && x <= 'z') || ('A' <= x && x <= 'Z') || ('0' <= x && x <= '9')) {
      begin ++;
    } else {
      break;
    }
  }
  return begin;
}
//
RuneStrArray::const_iterator NumbersRule(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end) const {
  Rune x = begin->rune;
  if ('0' <= x && x <= '9') {
    begin ++;
  } else {
    return begin;
  }
  while (begin != end) {
    x = begin->rune;
    if ( ('0' <= x && x <= '9') || x == '.') {
      begin++;
    } else {
      break;
    }
  }
  return begin;
}
// private:
//   virtual void InternalCut(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end, vector<WordRange>& res,size_t max_word_len) const = 0;

}; // class SegmentBase

} // cppjieba

#endif
