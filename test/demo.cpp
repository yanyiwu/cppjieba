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

  cout << "\n\e[32m" << "[demo] METHOD_MP" << "\e[0m\n"; // colorful
  app.cut(s, words, METHOD_MP);
  cout << join(words.begin(), words.end(), "/") << endl;

  cout << "\n\e[32m" << "[demo] METHOD_HMM" << "\e[0m\n"; // colorful
  app.cut(s, words, METHOD_HMM);
  cout << join(words.begin(), words.end(), "/") << endl;

  cout << "\n\e[32m" << "[demo] METHOD_MIX" << "\e[0m\n"; // colorful
  app.cut(s, words, METHOD_MIX);
  cout << join(words.begin(), words.end(), "/") << endl;

  cout << "\n\e[32m" << "[demo] METHOD_FULL" << "\e[0m\n"; // colorful
    app.cut(s, words, METHOD_FULL);
    cout << join(words.begin(), words.end(), "/") << endl;

  cout << "\n\e[32m" << "[demo] METHOD_QUERY" << "\e[0m\n"; // colorful
    app.cut(s, words, METHOD_QUERY);
    cout << join(words.begin(), words.end(), "/") << endl;

  cout << "\n\e[32m" << "[demo] TAGGING" << "\e[0m\n"; // colorful
  vector<pair<string, string> > tagres;
  app.tag(s, tagres);
  cout << s << endl;
  cout << tagres << endl;;

  cout << "\n\e[32m" << "[demo] KEYWORD" << "\e[0m\n"; // colorful
  vector<pair<string, double> > keywordres;
  app.extract(s, keywordres, 5);
  cout << s << endl;
  cout << keywordres << endl;

  return EXIT_SUCCESS;
}
