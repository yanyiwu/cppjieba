#include "../src/Application.hpp"

using namespace CppJieba;

void LoadSentences(const string& filepath, vector<string>& sentences) {
  ifstream ifile(filepath.c_str());
  if(!ifile.is_open()) {
    LogFatal("open %s failed.", filepath.c_str());
  }
  string line;
  while(getline(ifile, line)) {
    if(!line.empty()) {
      sentences.push_back(line);
    }
  }
}

int main(int argc, char** argv) {
  CppJieba::Application app("../dict/jieba.dict.utf8",
                            "../dict/hmm_model.utf8",
                            "../dict/user.dict.utf8",
                            "../dict/idf.utf8",
                            "../dict/stop_words.utf8");
  vector<string> words;
  string result;
  string s;
  
  vector<string> sentences;
  LoadSentences("../test/testdata/testlines.utf8", sentences);

  cout << "\n\e[32m" << "[demo] METHOD_MP" << "\e[0m\n"; // colorful
  for (size_t i = 0; i < sentences.size(); i++) {
    app.cut(sentences[i], words, METHOD_MP);
    cout << join(words.begin(), words.end(), "/") << endl;
  }

  cout << "\n\e[32m" << "[demo] METHOD_HMM" << "\e[0m\n"; // colorful
  for (size_t i = 0; i < sentences.size(); i++) {
    app.cut(sentences[i], words, METHOD_HMM);
    cout << join(words.begin(), words.end(), "/") << endl;
  }

  cout << "\n\e[32m" << "[demo] METHOD_MIX" << "\e[0m\n"; // colorful
  for (size_t i = 0; i < sentences.size(); i++) {
    app.cut(sentences[i], words, METHOD_MIX);
    cout << join(words.begin(), words.end(), "/") << endl;
  }

  cout << "\n\e[32m" << "[demo] METHOD_FULL" << "\e[0m\n"; // colorful
  for (size_t i = 0; i < sentences.size(); i++) {
    app.cut(sentences[i], words, METHOD_FULL);
    cout << join(words.begin(), words.end(), "/") << endl;
  }

  cout << "\n\e[32m" << "[demo] METHOD_QUERY" << "\e[0m\n"; // colorful
  for (size_t i = 0; i < sentences.size(); i++) {
    app.cut(sentences[i], words, METHOD_QUERY);
    cout << join(words.begin(), words.end(), "/") << endl;
  }

  cout << "\n\e[32m" << "[demo] TAGGING" << "\e[0m\n"; // colorful
  vector<pair<string, string> > tagres;
  s = "我是蓝翔技工拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上总经理，出任CEO，迎娶白富美，走上人生巅峰。";
  app.tag(s, tagres);
  cout << s << endl;
  cout << tagres << endl;;

  cout << "\n\e[32m" << "[demo] KEYWORD" << "\e[0m\n"; // colorful
  vector<pair<string, double> > keywordres;
  s = "我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。";
  app.extract(s, keywordres, 5);
  cout << s << endl;
  cout << keywordres << endl;

  return EXIT_SUCCESS;
}
