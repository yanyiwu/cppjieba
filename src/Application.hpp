#ifndef CPPJIEBA_APPLICATION_H
#define CPPJIEBA_APPLICATION_H

#include "Jieba.hpp"
#include "PosTagger.hpp"
#include "KeywordExtractor.hpp"

namespace CppJieba {

enum CutMethod {
  METHOD_MP,
  METHOD_HMM,
  METHOD_MIX,
  METHOD_FULL,
  METHOD_QUERY,
  METHOD_LEVEL
};

class Application {
 public:
  Application(const string& dict_path, 
              const string& model_path, 
              const string& user_dict_path,
              const string& idf_path,
              const string& stopWords_path) 
    : jieba_(dict_path, model_path, user_dict_path),
      tagger_(jieba_.GetDictTrie(), jieba_.GetHMMModel()), 
      extractor_(jieba_.GetDictTrie(), 
                 jieba_.GetHMMModel(), 
                 idf_path,
                 stopWords_path) {
  }
  void cut(const string& sentence, vector<string>& words, 
        CutMethod method = METHOD_MIX) const {
    switch(method) {
      case METHOD_MP:
        jieba_.Cut(sentence, false, words);
        break;
      case METHOD_HMM:
        jieba_.CutHMM(sentence, words);
        break;
      case METHOD_MIX:
        jieba_.Cut(sentence, true, words);
        break;
      case METHOD_FULL:
        jieba_.CutAll(sentence, words);
        break;
      case METHOD_QUERY:
        jieba_.CutForSearch(sentence, words);
        break;
      case METHOD_LEVEL:
        jieba_.CutLevel(sentence, words);
        break;
      default:
        LogError("argument method is illegal.");
    }
  }
  void cut(const string& sentence, 
        vector<pair<string, size_t> >& words) const {
    jieba_.CutLevel(sentence, words);
  }
  void cut(const string& sentence,
        vector<string>& words, size_t max_word_len) const {
    jieba_.CutSmall(sentence, words, max_word_len);
  }
  bool insertUserWord(const string& word, const string& tag = UNKNOWN_TAG) {
    return jieba_.InsertUserWord(word, tag);
  }
  void tag(const string& str, vector<pair<string, string> >& res) const {
    tagger_.tag(str, res);
  }
  void extract(const string& str, vector<pair<string, double> >& keyword, 
        size_t topN) const {
    extractor_.extract(str, keyword, topN);
  }
  ~Application() {
  }

 private:
  Jieba jieba_;
  PosTagger tagger_;
  KeywordExtractor extractor_;
}; // class Application

} // namespace CppJieba

#endif // CPPJIEBA_APPLICATION_H
