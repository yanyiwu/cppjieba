#ifndef CPPJIEBA_PRE_FILTER_H
#define CPPJIEBA_PRE_FILTER_H

#include "TransCode.hpp"

namespace CppJieba {

const Rune SPECIAL_SYMBOL[] = {32u, 9u, 10u, 12290u, 65292u};

class PreFilter {
 public:
  struct Range {
    Unicode::const_iterator begin;
    Unicode::const_iterator end;
  }; // struct Range

  PreFilter() {
    LoadSpecialSymbols();
  }
  ~PreFilter() {
  }

  void Reset(const string& sentence) {
    TransCode::decode(sentence, sentence_);
    cursor_ = sentence_.begin();
  }
  
  bool HasNext() const {
    return cursor_ != sentence_.end();
  }
  Range Next() {
    Range range;
    range.begin = cursor_;
    while (cursor_ != sentence_.end()) {
      if (isIn(specialSymbols_, *cursor_)) {
        if (range.begin == cursor_) {
          cursor_ ++;
        }
        range.end = cursor_;
        return range;
      }
      cursor_ ++;
    }
    range.end = sentence_.end();
    return range;
  }
 private:
  Unicode sentence_;
  Unicode::const_iterator cursor_;

  void LoadSpecialSymbols() {
    size_t size = sizeof(SPECIAL_SYMBOL)/sizeof(*SPECIAL_SYMBOL);
    for(size_t i = 0; i < size; i ++) {
      specialSymbols_.insert(SPECIAL_SYMBOL[i]);
    }
    assert(specialSymbols_.size());
  }

  unordered_set<Rune> specialSymbols_;
}; // class PreFilter

} // namespace CppJieba

#endif // CPPJIEBA_PRE_FILTER_H
