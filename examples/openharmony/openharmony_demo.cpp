#include <iostream>
#include <string>
#include <vector>

#include "cppjieba/Jieba.hpp"

#ifndef CPPJIEBA_OPENHARMONY_DICT_DIR
#define CPPJIEBA_OPENHARMONY_DICT_DIR ""
#endif

namespace {
std::string JoinPath(const std::string& dir, const std::string& file) {
  if (dir.empty()) {
    return file;
  }
  const char last = dir[dir.size() - 1];
  if (last == '/' || last == '\\') {
    return dir + file;
  }
  return dir + "/" + file;
}
}  // namespace

int main() {
  const std::string dict_dir = CPPJIEBA_OPENHARMONY_DICT_DIR;
  cppjieba::Jieba jieba(
      JoinPath(dict_dir, "jieba.dict.utf8"),
      JoinPath(dict_dir, "hmm_model.utf8"),
      JoinPath(dict_dir, "user.dict.utf8"),
      JoinPath(dict_dir, "idf.utf8"),
      JoinPath(dict_dir, "stop_words.utf8"));

  std::vector<std::string> words;
  const std::string sentence = "OpenHarmony 原生应用可以直接集成 cppjieba";
  jieba.Cut(sentence, words, true);

  for (size_t i = 0; i < words.size(); ++i) {
    if (i != 0) {
      std::cout << "/";
    }
    std::cout << words[i];
  }
  std::cout << std::endl;
  return 0;
}
