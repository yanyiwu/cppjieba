#include <unistd.h>
#include <algorithm>
#include <string>
#include <ctype.h>
#include <string.h>
#include "Limonp/Config.hpp"
#include "Husky/ThreadPoolServer.hpp"
#include "MPSegment.hpp"
#include "HMMSegment.hpp"
#include "MixSegment.hpp"

using namespace Husky;
using namespace CppJieba;

class ReqHandler: public IRequestHandler {
 public:
  ReqHandler(const string& dictPath, const string& modelPath, const string& userDictPath): _segment(dictPath, modelPath, userDictPath) {};
  virtual ~ReqHandler() {};
 public:
  virtual bool do_GET(const HttpReqInfo& httpReq, string& strSnd) const {
    string sentence, tmp;
    vector<string> words;
    httpReq.GET("key", tmp);
    URLDecode(tmp, sentence);
    _segment.cut(sentence, words);
    if(httpReq.GET("format", tmp) && tmp == "simple") {
      join(words.begin(), words.end(), strSnd, " ");
      return true;
    }
    strSnd << words;
    return true;
  }
  virtual bool do_POST(const HttpReqInfo& httpReq, string& strSnd) const {
    vector<string> words;
    _segment.cut(httpReq.getBody(), words);
    strSnd << words;
    return true;
  }
 private:
  MixSegment _segment;
};

bool run(int argc, char** argv) {
  if(argc < 2) {
    return false;
  }
  Config conf(argv[1]);
  if(!conf) {
    return false;
  }
  int port = 0;
  int threadNumber = 0;
  int queueMaxSize = 0;
  string dictPath;
  string modelPath;
  string userDictPath;
  LIMONP_CHECK(conf.get("port", port));
  LIMONP_CHECK(conf.get("thread_number", threadNumber));
  LIMONP_CHECK(conf.get("queue_max_size", queueMaxSize));
  LIMONP_CHECK(conf.get("dict_path", dictPath));
  LIMONP_CHECK(conf.get("model_path", modelPath));
  if(!conf.get("user_dict_path", userDictPath)) { //optional
    userDictPath = "";
  }

  LogInfo("config info: %s", conf.getConfigInfo().c_str());

  ReqHandler reqHandler(dictPath, modelPath, userDictPath);
  ThreadPoolServer sf(threadNumber, queueMaxSize, port, reqHandler);
  return sf.start();

}

int main(int argc, char* argv[]) {
  if(!run(argc, argv)) {
    printf("usage: %s <config_file>\n", argv[0]);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

