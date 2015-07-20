#include <unistd.h>
#include <algorithm>
#include <string>
#include <ctype.h>
#include <string.h>
#include "Limonp/Config.hpp"
#include "Husky/ThreadPoolServer.hpp"
#include "Application.hpp"

using namespace Husky;
using namespace CppJieba;

class ReqHandler: public IRequestHandler {
 public:
  ReqHandler(const CppJieba::Application& app): app_(app) {
  }
  virtual ~ReqHandler() {
  }

  virtual bool do_GET(const HttpReqInfo& httpReq, string& strSnd) const {
    string sentence, method, format;
    string tmp;
    vector<string> words;
    httpReq.GET("key", tmp);
    URLDecode(tmp, sentence);
    httpReq.GET("method", method);
    app_.cut(sentence, words, CppJieba::METHOD_MIX);
    httpReq.GET("format", format);
    run(sentence, method, format, strSnd);
    return true;
  }

  virtual bool do_POST(const HttpReqInfo& httpReq, string& strSnd) const {
    vector<string> words;
    run(httpReq.getBody(), "MIX", "simple", strSnd);
    return true;
  }

  void run(const string& sentence, 
           const string& method, 
           const string& format,
           string& strSnd) const {
    vector<string> words;
    if ("MP" == method) {
      app_.cut(sentence, words, CppJieba::METHOD_MP);
    } else if ("HMM" == method) {
      app_.cut(sentence, words, CppJieba::METHOD_HMM);
    } else if ("MIX" == method) {
      app_.cut(sentence, words, CppJieba::METHOD_MIX);
    } else if ("FULL" == method) {
      app_.cut(sentence, words, CppJieba::METHOD_FULL);
    } else if ("QUERY" == method) {
      app_.cut(sentence, words, CppJieba::METHOD_QUERY);
    } else { // default
      app_.cut(sentence, words, CppJieba::METHOD_MIX);
    }
    if(format == "simple") {
      join(words.begin(), words.end(), strSnd, " ");
    } else {
      strSnd << words;
    }
  }
 private:
  const CppJieba::Application& app_;
};

bool run(int argc, char** argv) {
  if(argc < 2) {
    return false;
  }
  Config conf(argv[1]);
  if(!conf) {
    return false;
  }
  int port = conf.get("port", 1339);
  int threadNumber = conf.get("thread_number", 4);
  int queueMaxSize = conf.get("queue_max_size", 1024);
  string dictPath = conf.get("dict_path", "");
  string modelPath = conf.get("model_path", "");
  string userDictPath = conf.get("user_dict_path", "");
  string idfPath = conf.get("idf_path", "");
  string stopWordsPath = conf.get("stop_words_path", "");

  LogInfo("config info: %s", conf.getConfigInfo().c_str());
  
  CppJieba::Application app(dictPath, 
        modelPath, 
        userDictPath, 
        idfPath, 
        stopWordsPath);
  
  ReqHandler reqHandler(app);
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

