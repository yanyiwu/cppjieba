#ifndef CPPJIEBA_MPSEGMENT_H
#define CPPJIEBA_MPSEGMENT_H

#include <algorithm>
#include <set>
#include <cassert>
#include "limonp/Logger.hpp"
#include "DictTrie.hpp"
#include "ISegment.hpp"
#include "SegmentBase.hpp"

namespace CppJieba {

class MPSegment: public SegmentBase {
 public:
  MPSegment(const string& dictPath, const string& userDictPath = "") {
    dictTrie_ = new DictTrie(dictPath, userDictPath);
    isNeedDestroy_ = true;
    LogInfo("MPSegment init(%s) ok", dictPath.c_str());
  }
  MPSegment(const DictTrie* dictTrie)
    : dictTrie_(dictTrie), isNeedDestroy_(false) {
    assert(dictTrie_);
  }
  virtual ~MPSegment() {
    if(isNeedDestroy_) {
      delete dictTrie_;
    }
  }

  bool isUserDictSingleChineseWord(const Rune & value) const {
    return dictTrie_->isUserDictSingleChineseWord(value);
  }

  using SegmentBase::cut;
  void cut(Unicode::const_iterator begin , Unicode::const_iterator end, vector<Unicode>& res) const {
    vector<Dag> dags;

    dictTrie_->find(begin, end, dags);

    calcDP_(dags);

    cut_(dags, res);
  }
  void cut(Unicode::const_iterator begin,
           Unicode::const_iterator end,
           vector<Unicode>& res,
           size_t max_word_len) const {
    vector<Dag> dags;
    dictTrie_->find(begin, 
          end, 
          dags,
          max_word_len);
    calcDP_(dags);
    cut_(dags, res);
  }
  const DictTrie* getDictTrie() const {
    return dictTrie_;
  }

 private:
  void calcDP_(vector<Dag>& dags) const {
    size_t nextPos;
    const DictUnit* p;
    double val;

    for(vector<Dag>::reverse_iterator rit = dags.rbegin(); rit != dags.rend(); rit++) {
      rit->pInfo = NULL;
      rit->weight = MIN_DOUBLE;
      assert(!rit->nexts.empty());
      for(LocalVector<pair<size_t, const DictUnit*> >::const_iterator it = rit->nexts.begin(); it != rit->nexts.end(); it++) {
        nextPos = it->first;
        p = it->second;
        val = 0.0;
        if(nextPos + 1 < dags.size()) {
          val += dags[nextPos + 1].weight;
        }

        if(p) {
          val += p->weight;
        } else {
          val += dictTrie_->getMinWeight();
        }
        if(val > rit->weight) {
          rit->pInfo = p;
          rit->weight = val;
        }
      }
    }
  }
  void cut_(const vector<Dag>& dags, 
        vector<Unicode>& res) const {
    size_t i = 0;
    while(i < dags.size()) {
      const DictUnit* p = dags[i].pInfo;
      if(p) {
        res.push_back(p->word);
        i += p->word.size();
      } else { //single chinese word
        res.push_back(Unicode(1, dags[i].rune));
        i++;
      }
    }
  }

 private:
  const DictTrie* dictTrie_;
  bool isNeedDestroy_;
}; // class MPSegment

} // namespace CppJieba

#endif
