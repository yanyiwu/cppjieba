#include "../src/Application.hpp"

using namespace CppJieba;

int main(int argc, char** argv) {
  CppJieba::Application app("../dict/jieba.dict.utf8",
                            "../dict/hmm_model.utf8",
                            "../dict/user.dict.utf8",
                            "../dict/idf.utf8",
                            "../dict/stop_words.utf8");
  vector<string> words;
  string result;
  string s = "我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。";

  cout << "[demo] DEFAULT" << endl;
  app.cut(s, words);
  cout << join(words.begin(), words.end(), "/") << endl;

  cout << "[demo] METHOD_MP" << endl;
  app.cut(s, words, METHOD_MP);
  cout << join(words.begin(), words.end(), "/") << endl;

  cout << "[demo] METHOD_HMM" << endl;
  app.cut(s, words, METHOD_HMM);
  cout << join(words.begin(), words.end(), "/") << endl;

  cout << "[demo] METHOD_MIX" << endl;
  app.cut(s, words, METHOD_MIX);
  cout << join(words.begin(), words.end(), "/") << endl;

  cout << "[demo] METHOD_FULL" << endl;
  app.cut(s, words, METHOD_FULL);
  cout << join(words.begin(), words.end(), "/") << endl;

  cout << "[demo] METHOD_QUERY" << endl;
  app.cut(s, words, METHOD_QUERY);
  cout << join(words.begin(), words.end(), "/") << endl;

  cout << "[demo] TAGGING" << endl;
  vector<pair<string, string> > tagres;
  app.tag(s, tagres);
  cout << s << endl;
  cout << tagres << endl;;

  cout << "[demo] KEYWORD" << endl;
  vector<pair<string, double> > keywordres;
  app.extract(s, keywordres, 5);
  cout << s << endl;
  cout << keywordres << endl;

  cout << "[demo] Insert User Word" << endl;
  app.cut("男默女泪", words);
  cout << join(words.begin(), words.end(), "/") << endl;
  app.InsertUserWord("男默女泪");
  app.cut("男默女泪", words);
  cout << join(words.begin(), words.end(), "/") << endl;

  return EXIT_SUCCESS;
}
