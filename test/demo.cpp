#include "cppjieba/Jieba.hpp"
#include "cppjieba/KeywordExtractor.hpp"

using namespace std;

const char* const DICT_PATH = "../dict/jieba.dict.utf8";
const char* const HMM_PATH = "../dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../dict/user.dict.utf8";
const char* const IDF_PATH = "../dict/idf.utf8";
const char* const STOP_WORD_PATH = "../dict/stop_words.utf8";

int main(int argc, char** argv) {
  cppjieba::Jieba jieba(DICT_PATH,
        HMM_PATH,
        USER_DICT_PATH);
  vector<string> words;
  string result;
  string s = "我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。";

  cout << "[demo] Cut With HMM" << endl;
  jieba.Cut(s, words, true);
  cout << limonp::Join(words.begin(), words.end(), "/") << endl;

  cout << "[demo] Cut Without HMM " << endl;
  jieba.Cut(s, words, false);
  cout << limonp::Join(words.begin(), words.end(), "/") << endl;

  cout << "[demo] CutAll" << endl;
  jieba.CutAll(s, words);
  cout << limonp::Join(words.begin(), words.end(), "/") << endl;

  cout << "[demo] CutForSearch" << endl;
  jieba.CutForSearch(s, words);
  cout << limonp::Join(words.begin(), words.end(), "/") << endl;

  cout << "[demo] Insert User Word" << endl;
  jieba.Cut("男默女泪", words);
  cout << limonp::Join(words.begin(), words.end(), "/") << endl;
  jieba.InsertUserWord("男默女泪");
  jieba.Cut("男默女泪", words);
  cout << limonp::Join(words.begin(), words.end(), "/") << endl;

  cout << "[demo] Locate Words" << endl;
  vector<cppjieba::Jieba::LocWord> loc_words;
  jieba.Cut("南京市长江大桥", words, true);
  cppjieba::Jieba::Locate(words, loc_words);
  for (size_t i = 0; i < loc_words.size(); i++) {
    cout << loc_words[i].word 
      << ", " << loc_words[i].begin
      << ", " << loc_words[i].end
      << endl;
  }

  cout << "[demo] TAGGING" << endl;
  vector<pair<string, string> > tagres;
  jieba.Tag(s, tagres);
  cout << s << endl;
  cout << tagres << endl;;

  cppjieba::KeywordExtractor extractor(jieba,
        IDF_PATH,
        STOP_WORD_PATH);
  cout << "[demo] KEYWORD" << endl;
  const size_t topk = 5;
  vector<pair<string, double> > keywordres;
  extractor.Extract(s, keywordres, topk);
  cout << s << endl;
  cout << keywordres << endl;
  return EXIT_SUCCESS;
}
