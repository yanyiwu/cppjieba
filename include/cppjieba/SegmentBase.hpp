#ifndef CPPJIEBA_SEGMENTBASE_H
#define CPPJIEBA_SEGMENTBASE_H

#include "limonp/Logging.hpp"
#include "PreFilter.hpp"
#include <cassert>


namespace cppjieba {

const char* const SPECIAL_RUNES = " \t\n，。";

using namespace limonp;

class SegmentBase {
 public:
  SegmentBase() {
    XCHECK(Insert(SPECIAL_RUNES));
  }
  ~SegmentBase() {
  }

 protected:
  bool Insert(const string& s) {
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
  //bool Remove(const string& s) {
  //  RuneStrArray runes;
  //  if (!DecodeRunesInString(s, runes)) {
  //    XLOG(ERROR) << "decode " << s << " failed";
  //    return false;
  //  }
  //  for (size_t i = 0; i < runes.size(); i++) {
  //    unordered_set<Rune>::iterator iter = symbols_.find(runes[i].rune);
  //    if (iter == symbols_.end()) {
  //      XLOG(ERROR) << s.substr(runes[i].offset, runes[i].len) << " not found";
  //      return false;
  //    }
  //    symbols_.erase(iter);
  //  }
  //  return true;
  //}

  unordered_set<Rune> symbols_;
}; // class SegmentBase

} // cppjieba

#endif
