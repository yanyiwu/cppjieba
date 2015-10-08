#ifndef CPPJIEBA_DICT_TRIE_HPP
#define CPPJIEBA_DICT_TRIE_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <cstring>
#include <stdint.h>
#include <cmath>
#include <limits>
#include "limonp/StringUtil.hpp"
#include "limonp/Logger.hpp"
#include "TransCode.hpp"
#include "Trie.hpp"

namespace CppJieba {

using namespace limonp;

const double MIN_DOUBLE = -3.14e+100;
const double MAX_DOUBLE = 3.14e+100;
const size_t DICT_COLUMN_NUM = 3;
const char* const UNKNOWN_TAG = "";

class DictTrie {
 public:
  enum UserWordWeightOption {
    Min,
    Median,
    Max,
  }; // enum UserWordWeightOption

  DictTrie() {
    trie_ = NULL;
    min_weight_ = MAX_DOUBLE;
  }
  DictTrie(const string& dict_path, const string& user_dict_paths = "") {
    new (this) DictTrie();
    init(dict_path, user_dict_paths);
  }
  ~DictTrie() {
    delete trie_;
  }

  void init(const string& dict_path, const string& user_dict_paths = "") {
    if (trie_ != NULL) {
      LogFatal("trie already initted");
    }
    LoadDict(dict_path);
    CalculateWeight(static_node_infos_);
    SetStaticWordWeights();

    if (user_dict_paths.size()) {
      LoadUserDict(user_dict_paths);
    }
    Shrink(static_node_infos_);
    CreateTrie(static_node_infos_);
  }
  
  bool insertUserWord(const string& word, const string& tag = UNKNOWN_TAG) {
    DictUnit node_info;
    if (!MakeUserNodeInfo(node_info, word, tag)) {
      return false;
    }
    active_node_infos_.push_back(node_info);
    trie_->insertNode(node_info.word, &active_node_infos_.back());
    return true;
  }

  const DictUnit* find(Unicode::const_iterator begin, Unicode::const_iterator end) const {
    return trie_->find(begin, end);
  }

  void find(Unicode::const_iterator begin, 
        Unicode::const_iterator end, 
        vector<struct Dag>&res,
        size_t max_word_len = MAX_WORD_LENGTH) const {
    trie_->find(begin, end, res, max_word_len);
  }

  bool isUserDictSingleChineseWord(const Rune& word) const {
    return isIn(user_dict_single_chinese_word_, word);
  }

  double getMinWeight() const {
    return min_weight_;
  }

 private:
  void CreateTrie(const vector<DictUnit>& dictUnits) {
    assert(dictUnits.size());
    vector<Unicode> words;
    vector<const DictUnit*> valuePointers;
    for (size_t i = 0 ; i < dictUnits.size(); i ++) {
      words.push_back(dictUnits[i].word);
      valuePointers.push_back(&dictUnits[i]);
    }

    trie_ = new Trie(words, valuePointers);
  }
  void LoadUserDict(const string& filePaths) {
    vector<string> files = limonp::split(filePaths, ":");
    size_t lineno = 0;
    for (size_t i = 0; i < files.size(); i++) {
      ifstream ifs(files[i].c_str());
      if (!ifs.is_open()) {
        LogFatal("file %s open failed.", files[i].c_str());
      }
      string line;
      DictUnit node_info;
      vector<string> buf;
      for (; getline(ifs, line); lineno++) {
        buf.clear();
        split(line, buf, " ");
        if (buf.size() < 1) {
          LogFatal("split [%s] result illegal", line.c_str());
        }
        DictUnit node_info;
        MakeUserNodeInfo(node_info, buf[0], 
              (buf.size() == 2 ? buf[1] : UNKNOWN_TAG));
        static_node_infos_.push_back(node_info);
      }
    }
    LogInfo("load userdicts[%s] ok. lines[%u]", filePaths.c_str(), lineno);
  }
  bool MakeNodeInfo(DictUnit& node_info,
        const string& word, 
        double weight, 
        const string& tag) {
    if (!TransCode::decode(word, node_info.word)) {
      LogError("decode %s failed.", word.c_str());
      return false;
    }
    node_info.weight = weight;
    node_info.tag = tag;
    return true;
  }
  bool MakeUserNodeInfo(DictUnit& node_info, 
        const string& word, 
        const string& tag = UNKNOWN_TAG) {
    if (!TransCode::decode(word, node_info.word)) {
      LogError("decode %s failed.", word.c_str());
      return false;
    }
    if (node_info.word.size() == 1) {
      user_dict_single_chinese_word_.insert(node_info.word[0]);
    }
    node_info.weight = max_weight_;
    node_info.tag = tag;
    return true;
  }
  void LoadDict(const string& filePath) {
    ifstream ifs(filePath.c_str());
    if (!ifs.is_open()) {
      LogFatal("file %s open failed.", filePath.c_str());
    }
    string line;
    vector<string> buf;

    DictUnit node_info;
    for (size_t lineno = 0; getline(ifs, line); lineno++) {
      split(line, buf, " ");
      if (buf.size() != DICT_COLUMN_NUM) {
        LogFatal("split result illegal, line: %s, result size: %u", line.c_str(), buf.size());
      }
      MakeNodeInfo(node_info, 
            buf[0], 
            atof(buf[1].c_str()), 
            buf[2]);
      static_node_infos_.push_back(node_info);
    }
  }

  static bool WeightCompare(const DictUnit& lhs, const DictUnit& rhs) {
    return lhs.weight < rhs.weight;
  }

  void SetStaticWordWeights() {
    if (static_node_infos_.empty()) {
      LogFatal("something must be wrong");
    }
    vector<DictUnit> x = static_node_infos_;
    sort(x.begin(), x.end(), WeightCompare);
    min_weight_ = x[0].weight;
    max_weight_ = x[x.size() - 1].weight;
    median_weight_ = x[x.size() / 2].weight;
  }

  void CalculateWeight(vector<DictUnit>& node_infos) const {
    double sum = 0.0;
    for (size_t i = 0; i < node_infos.size(); i++) {
      sum += node_infos[i].weight;
    }
    assert(sum);
    for (size_t i = 0; i < node_infos.size(); i++) {
      DictUnit& node_info = node_infos[i];
      assert(node_info.weight);
      node_info.weight = log(double(node_info.weight)/double(sum));
    }
  }

  void Shrink(vector<DictUnit>& units) const {
    vector<DictUnit>(units.begin(), units.end()).swap(units);
  }

  vector<DictUnit> static_node_infos_;
  deque<DictUnit> active_node_infos_; // must not be vector
  Trie * trie_;

  double min_weight_;
  double max_weight_;
  double median_weight_;
  unordered_set<Rune> user_dict_single_chinese_word_;
};
}

#endif
