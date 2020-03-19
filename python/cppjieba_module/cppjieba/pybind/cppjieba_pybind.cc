#include <string>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "cppjieba/Jieba.hpp"

using namespace pybind11::literals;
namespace py = pybind11;


PYBIND11_MODULE(cppjieba_pybind, m) {
  py::class_<cppjieba::Jieba>(m, "Jieba")
    .def(py::init([](const char* DICT_PATH, const char* HMM_PATH, const char* USER_DICT_PATH, const char* IDF_PATH, const char* STOP_WORD_PATH) { return new cppjieba::Jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH); }))
    .def(
      "cut",
      [](cppjieba::Jieba& m, const std::string& sentence, bool hmm) { 
        std::vector<std::string> words;
        m.Cut(sentence, words, hmm); 
        return words;
      })
    .def(
      "cut_all",
      [](cppjieba::Jieba& m, const std::string& sentence) {
        std::vector<std::string> words;
        m.CutAll(sentence, words);
        return words;
      })
    .def(
      "cut_for_search",
      [](cppjieba::Jieba& m, const std::string& sentence) {
        std::vector<std::string> words;
        m.CutForSearch(sentence, words);
        return words;
      })
    .def(
      "pseg",
      [](cppjieba::Jieba& m, const std::string& sentence) {
        std::vector<std::pair<std::string, std::string>> tagres;
        m.Tag(sentence, tagres);
        return tagres;
      })
    .def(
      "extract",
      [](cppjieba::Jieba& m, const std::string& sentence, size_t topk) {
        std::vector<cppjieba::KeywordExtractor::Word> keywordres;
        m.extractor.Extract(sentence, keywordres, topk);
        std::vector<std::pair<std::string, double> > keywords;
        for (size_t i = 0; i < keywordres.size(); i++)
          keywords.push_back(std::make_pair(keywordres[i].word, keywordres[i].weight)); 
        return keywords;
      })
    ;
}
