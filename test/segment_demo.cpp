#include <iostream>
#include <fstream>

#define LOGGER_LEVEL LL_WARN

#include "../src/MPSegment.hpp"
#include "../src/HMMSegment.hpp"
#include "../src/MixSegment.hpp"

using namespace CppJieba;

const char * const TEST_FILE = "../test/testdata/testlines.utf8";
const char * const JIEBA_DICT_FILE = "../dict/jieba.dict.utf8";
const char * const HMM_DICT_FILE = "../dict/hmm_model.utf8";
const char * const USER_DICT_FILE = "../dict/user.dict.utf8";

void cut(const ISegment& seg, const char * const filePath) {
  ifstream ifile(filePath);
  vector<string> words;
  string line;
  string res;
  while(getline(ifile, line)) {
    if(!line.empty()) {
      words.clear();
      seg.cut(line, words);
      join(words.begin(), words.end(), res, "/");
      cout<< res <<endl;
    }
  }
}


int main(int argc, char ** argv) {
  {
    printf("\e[32m%s\e[0m\n", "[demo] MPSegment"); // colorful
    MPSegment seg(JIEBA_DICT_FILE);
    cut(seg, TEST_FILE);
  }
  {
    printf("\e[32m%s\e[0m\n", "[demo] HMMSegment"); // colorful
    HMMSegment seg(HMM_DICT_FILE);
    cut(seg, TEST_FILE);
  }
  {
    printf("\e[32m%s\e[0m\n", "[demo] MixSegment"); // colorful
    MixSegment seg(JIEBA_DICT_FILE, HMM_DICT_FILE);
    cut(seg, TEST_FILE);
  }
  {
    printf("\e[32m%s\e[0m\n", "[demo] MixSegment with UserDict"); // colorful
    MixSegment seg(JIEBA_DICT_FILE, HMM_DICT_FILE, USER_DICT_FILE);
    cut(seg, TEST_FILE);
  }
  return EXIT_SUCCESS;
}
