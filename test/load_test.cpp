#include <iostream>
#include <ctime>
#include <fstream>
#include "cppjieba/MPSegment.hpp"
#include "cppjieba/HMMSegment.hpp"
#include "cppjieba/MixSegment.hpp"
#include "cppjieba/KeywordExtractor.hpp"
#include "limonp/Colors.hpp"

using namespace cppjieba;

void Cut(size_t times = 50) {
  MixSegment seg("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8");
  vector<string> res;
  string doc;
  ifstream ifs("../test/testdata/weicheng.utf8");
  assert(ifs);
  doc << ifs;
  long beginTime = clock();
  for (size_t i = 0; i < times; i ++) {
    printf("process [%3.0lf %%]\r", 100.0*(i+1)/times);
    fflush(stdout);
    res.clear();
    seg.Cut(doc, res);
  }
  printf("\n");
  long endTime = clock();
  ColorPrintln(GREEN, "Cut: [%.3lf seconds]time consumed.", double(endTime - beginTime)/CLOCKS_PER_SEC);
}

void Extract(size_t times = 400) {
  KeywordExtractor Extractor("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8", "../dict/idf.utf8", "../dict/stop_words.utf8");
  vector<string> words;
  string doc;
  ifstream ifs("../test/testdata/review.100");
  assert(ifs);
  doc << ifs;
  long beginTime = clock();
  for (size_t i = 0; i < times; i ++) {
    printf("process [%3.0lf %%]\r", 100.0*(i+1)/times);
    fflush(stdout);
    words.clear();
    Extractor.Extract(doc, words, 5);
  }
  printf("\n");
  long endTime = clock();
  ColorPrintln(GREEN, "Extract: [%.3lf seconds]time consumed.", double(endTime - beginTime)/CLOCKS_PER_SEC);
}

int main(int argc, char ** argv) {
  Cut();
  Extract();
  return EXIT_SUCCESS;
}
