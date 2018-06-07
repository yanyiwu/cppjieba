#ifndef CPPJIEAB_JIEBA_H
#define CPPJIEAB_JIEBA_H

#include "QuerySegment.hpp"
#include "KeywordExtractor.hpp"

namespace cppjieba {

class Jieba {
 public:
  Jieba(const std::string& dict_path,
        const std::string& model_path,
        const std::string& user_dict_path,
        const std::string& idfPath,
        const std::string& stopWordPath)
    : dict_trie_(dict_path, user_dict_path),
      model_(model_path),
      mp_seg_(&dict_trie_),
      hmm_seg_(&model_),
      mix_seg_(&dict_trie_, &model_),
      full_seg_(&dict_trie_),
      query_seg_(&dict_trie_, &model_),
      extractor(&dict_trie_, &model_, idfPath, stopWordPath) {
  }
  ~Jieba() {
  }

  struct LocWord {
    std::string word;
    std::size_t begin;
    std::size_t end;
  }; // struct LocWord

  void Cut(const std::string& sentence, std::vector<string>& words, bool hmm = true) const {
    mix_seg_.Cut(sentence, words, hmm);
  }
  void Cut(const std::string& sentence, std::vector<Word>& words, bool hmm = true) const {
    mix_seg_.Cut(sentence, words, hmm);
  }
  void CutAll(const std::string& sentence, std::vector<string>& words) const {
    full_seg_.Cut(sentence, words);
  }
  void CutAll(const std::string& sentence, std::vector<Word>& words) const {
    full_seg_.Cut(sentence, words);
  }
  void CutForSearch(const std::string& sentence, std::vector<string>& words, bool hmm = true) const {
    query_seg_.Cut(sentence, words, hmm);
  }
  void CutForSearch(const std::string& sentence, std::vector<Word>& words, bool hmm = true) const {
    query_seg_.Cut(sentence, words, hmm);
  }
  void CutHMM(const std::string& sentence, std::vector<std::string>& words) const {
    hmm_seg_.Cut(sentence, words);
  }
  void CutHMM(const std::string& sentence, std::vector<Word>& words) const {
    hmm_seg_.Cut(sentence, words);
  }
  void CutSmall(const std::string& sentence, std::vector<std::string>& words, size_t max_word_len) const {
    mp_seg_.Cut(sentence, words, max_word_len);
  }
  void CutSmall(const std::string& sentence, std::vector<Word>& words, size_t max_word_len) const {
    mp_seg_.Cut(sentence, words, max_word_len);
  }

  void Tag(const std::string& sentence, std::vector<pair<std::string, std::string> >& words) const {
    mix_seg_.Tag(sentence, words);
  }
  std::string LookupTag(const std::string &str) const {
    return mix_seg_.LookupTag(str);
  }
  bool InsertUserWord(const std::string& word, const std::string& tag = UNKNOWN_TAG) {
    return dict_trie_.InsertUserWord(word, tag);
  }

  void ResetSeparators(const std::string& s) {
    //TODO
    mp_seg_.ResetSeparators(s);
    hmm_seg_.ResetSeparators(s);
    mix_seg_.ResetSeparators(s);
    full_seg_.ResetSeparators(s);
    query_seg_.ResetSeparators(s);
  }

  const DictTrie* GetDictTrie() const {
    return &dict_trie_;
  }
  const HMMModel* GetHMMModel() const {
    return &model_;
  }

 private:
  DictTrie dict_trie_;
  HMMModel model_;

  // They share the same dict trie and model
  MPSegment mp_seg_;
  HMMSegment hmm_seg_;
  MixSegment mix_seg_;
  FullSegment full_seg_;
  QuerySegment query_seg_;

 public:
  KeywordExtractor extractor;
}; // class Jieba

} // namespace cppjieba

#endif // CPPJIEAB_JIEBA_H
