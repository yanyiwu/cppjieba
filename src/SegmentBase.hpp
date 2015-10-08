#ifndef CPPJIEBA_SEGMENTBASE_H
#define CPPJIEBA_SEGMENTBASE_H

#include "limonp/Logger.hpp"
#include "PreFilter.hpp"
#include <cassert>


namespace CppJieba {

//const char* const SPECIAL_CHARS = " \t\n，。";
const Rune SPECIAL_SYMBOL[] = {32u, 9u, 10u, 65292u, 12290u};

using namespace limonp;

class SegmentBase {
 public:
  SegmentBase() {
    LoadSpecialSymbols();
  }
  ~SegmentBase() {
  }
  /*
 public:
  void cut(Unicode::const_iterator begin, Unicode::const_iterator end, vector<Unicode>& res) const = 0;
  bool cut(const string& sentence, vector<string>& words) const {
    PreFilter pre_filter(symbols_, sentence);
    PreFilter::Range range;
    vector<Unicode> uwords;
    uwords.reserve(sentence.size());
    while (pre_filter.HasNext()) {
      range = pre_filter.Next();
      cut(range.begin, range.end, uwords);
    }
    TransCode::encode(uwords, words);
    return true;
  }
  */

 protected:
  void LoadSpecialSymbols() {
    size_t size = sizeof(SPECIAL_SYMBOL)/sizeof(*SPECIAL_SYMBOL);
    for (size_t i = 0; i < size; i ++) {
      symbols_.insert(SPECIAL_SYMBOL[i]);
    }
    assert(symbols_.size());
  }
  unordered_set<Rune> symbols_;
}; // class SegmentBase

} // CppJieba

#endif
