#include "../src/KeywordExtractor.hpp"
using namespace CppJieba;

int main(int argc, char ** argv) {
  KeywordExtractor extractor("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8", "../dict/idf.utf8", "../dict/stop_words.utf8");
  //KeywordExtractor extractor("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8", "../dict/idf.utf8", "../dict/stop_words.utf8", "../dict/user.dict.utf8");
  string s("我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。");
  vector<pair<string, double> > wordweights;
  vector<string> words;
  size_t topN = 5;
  extractor.extract(s, wordweights, topN);
  cout<< s << '\n' << wordweights << endl;
  extractor.extract(s, words, topN);
  cout<< s << '\n' << words << endl;
  return EXIT_SUCCESS;
}
