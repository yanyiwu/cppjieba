#ifndef CPPJIBEA_HMMSEGMENT_H
#define CPPJIBEA_HMMSEGMENT_H

#include <iostream>
#include <fstream>
#include <memory.h>
#include <cassert>
#include "HMMModel.hpp"
#include "SegmentBase.hpp"

namespace cppjieba {
class HMMSegment: public SegmentBase {
 public:
  HMMSegment(const string& filePath)
  : model_(new HMMModel(filePath)), isNeedDestroy_(true) {
  }
  HMMSegment(const HMMModel* model) 
  : model_(model), isNeedDestroy_(false) {
  }
  ~HMMSegment() {
    if (isNeedDestroy_) {
      delete model_;
    }
  }

  void Cut(const string& sentence, 
        vector<string>& words) const {
    PreFilter pre_filter(symbols_, sentence);
    PreFilter::Range range;
    vector<unicode::WordRange> wrs;
    wrs.reserve(sentence.size()/2);
    while (pre_filter.HasNext()) {
      range = pre_filter.Next();
      Cut(range.begin, range.end, wrs);
    }
    words.clear();
    words.reserve(wrs.size());
    unicode::GetStringsFromWordRanges(wrs, words);
  }
  void Cut(unicode::RuneStrArray::const_iterator begin, unicode::RuneStrArray::const_iterator end, vector<unicode::WordRange>& res) const {
    unicode::RuneStrArray::const_iterator left = begin;
    unicode::RuneStrArray::const_iterator right = begin;
    while (right != end) {
      if (right->rune < 0x80) {
        if (left != right) {
          InternalCut(left, right, res);
        }
        left = right;
        do {
          right = SequentialLetterRule(left, end);
          if (right != left) {
            break;
          }
          right = NumbersRule(left, end);
          if (right != left) {
            break;
          }
          right ++;
        } while (false);
        unicode::WordRange wr(left, right - 1);
        res.push_back(wr);
        left = right;
      } else {
        right++;
      }
    }
    if (left != right) {
      InternalCut(left, right, res);
    }
  }
 private:
  // sequential letters rule
  unicode::RuneStrArray::const_iterator SequentialLetterRule(unicode::RuneStrArray::const_iterator begin, unicode::RuneStrArray::const_iterator end) const {
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
  unicode::RuneStrArray::const_iterator NumbersRule(unicode::RuneStrArray::const_iterator begin, unicode::RuneStrArray::const_iterator end) const {
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
  void InternalCut(unicode::RuneStrArray::const_iterator begin, unicode::RuneStrArray::const_iterator end, vector<unicode::WordRange>& res) const {
    vector<size_t> status;
    Viterbi(begin, end, status);

    unicode::RuneStrArray::const_iterator left = begin;
    unicode::RuneStrArray::const_iterator right;
    for (size_t i = 0; i < status.size(); i++) {
      if (status[i] % 2) { //if (HMMModel::E == status[i] || HMMModel::S == status[i])
        right = begin + i + 1;
        unicode::WordRange wr(left, right - 1);
        res.push_back(wr);
        left = right;
      }
    }
  }

  void Viterbi(unicode::RuneStrArray::const_iterator begin, 
        unicode::RuneStrArray::const_iterator end, 
        vector<size_t>& status) const {
    size_t Y = HMMModel::STATUS_SUM;
    size_t X = end - begin;

    size_t XYSize = X * Y;
    size_t now, old, stat;
    double tmp, endE, endS;

    vector<int> path(XYSize);
    vector<double> weight(XYSize);

    //start
    for (size_t y = 0; y < Y; y++) {
      weight[0 + y * X] = model_->startProb[y] + model_->GetEmitProb(model_->emitProbVec[y], begin->rune, MIN_DOUBLE);
      path[0 + y * X] = -1;
    }

    double emitProb;

    for (size_t x = 1; x < X; x++) {
      for (size_t y = 0; y < Y; y++) {
        now = x + y*X;
        weight[now] = MIN_DOUBLE;
        path[now] = HMMModel::E; // warning
        emitProb = model_->GetEmitProb(model_->emitProbVec[y], (begin+x)->rune, MIN_DOUBLE);
        for (size_t preY = 0; preY < Y; preY++) {
          old = x - 1 + preY * X;
          tmp = weight[old] + model_->transProb[preY][y] + emitProb;
          if (tmp > weight[now]) {
            weight[now] = tmp;
            path[now] = preY;
          }
        }
      }
    }

    endE = weight[X-1+HMMModel::E*X];
    endS = weight[X-1+HMMModel::S*X];
    stat = 0;
    if (endE >= endS) {
      stat = HMMModel::E;
    } else {
      stat = HMMModel::S;
    }

    status.resize(X);
    for (int x = X -1 ; x >= 0; x--) {
      status[x] = stat;
      stat = path[x + stat*X];
    }
  }

  const HMMModel* model_;
  bool isNeedDestroy_;
}; // class HMMSegment

} // namespace cppjieba

#endif
