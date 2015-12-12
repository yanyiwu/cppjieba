#include "../src/Jieba.hpp"

using namespace std;

int main(int argc, char** argv) {
  cppjieba::Jieba jieba("../dict/jieba.dict.utf8",
                            "../dict/hmm_model.utf8",
                            "../dict/user.dict.utf8");
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
  jieba.Locate(words, loc_words);
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

  //cout << "[demo] KEYWORD" << endl;
  //vector<pair<string, double> > keywordres;
  //jieba.Extract(s, keywordres, 5);
  //cout << s << endl;
  //cout << keywordres << endl;

  return EXIT_SUCCESS;
}
