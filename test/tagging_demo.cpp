#include "../src/PosTagger.hpp"
using namespace CppJieba;

int main(int argc, char ** argv) {
  PosTagger tagger("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8", "../dict/user.dict.utf8");
  string s("我是蓝翔技工拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上总经理，出任CEO，迎娶白富美，走上人生巅峰。");
  vector<pair<string, string> > res;
  tagger.tag(s, res);
  cout << res << endl;
  return EXIT_SUCCESS;
}
