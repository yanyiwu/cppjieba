#include <unistd.h>
#include <algorithm>
#include <string>
#include <ctype.h>
#include <string.h>
#include "limonp/Config.hpp"
#include "husky/thread_pool_server.h"
#include "Jieba.hpp"

using namespace husky;
using namespace cppjieba;

class ReqHandler: public IRequestHandler {
 public:
  ReqHandler(const cppjieba::Jieba& jieba)
   : jieba_(jieba) {
  }

  virtual ~ReqHandler() {
  }

  virtual bool DoGET(const HttpReqInfo& httpReq, string& strSnd) {
    string sentence, method, format;
    string tmp;
    vector<string> words;
    httpReq.GET("key", tmp);
    URLDecode(tmp, sentence);
    httpReq.GET("method", method);
    jieba_.Cut(sentence, words, true);
    httpReq.GET("format", format);
    Run(sentence, method, format, strSnd);
    return true;
  }

  virtual bool DoPOST(const HttpReqInfo& httpReq, string& strSnd) {
    vector<string> words;
    Run(httpReq.GetBody(), "MIX", "simple", strSnd);
    return true;
  }

  void Run(const string& sentence, 
           const string& method, 
           const string& format,
           string& strSnd) const {
    vector<string> words;
    if ("MP" == method) {
      jieba_.Cut(sentence, words, false);
    } else if ("HMM" == method) {
      jieba_.CutHMM(sentence, words);
    } else if ("MIX" == method) {
      jieba_.Cut(sentence, words, true);
    } else if ("FULL" == method) {
      jieba_.CutAll(sentence, words);
    } else if ("QUERY" == method) {
      jieba_.CutForSearch(sentence, words);
    } else { // default
      jieba_.Cut(sentence, words, false);
    }
    if (format == "simple") {
      Join(words.begin(), words.end(), strSnd, " ");
    } else {
      strSnd << words;
    }
  }
 private:
  const cppjieba::Jieba& jieba_;
};

bool Run(int argc, char** argv) {
  if (argc < 2) {
    return false;
  }
  Config conf(argv[1]);
  if (!conf) {
    return false;
  }
  int port = conf.Get("port", 1339);
  int threadNumber = conf.Get("thread_number", 4);
  int queueMaxSize = conf.Get("queue_max_size", 1024);
  string dictPath = conf.Get("dict_path", "");
  string modelPath = conf.Get("model_path", "");
  string userDictPath = conf.Get("user_dict_path", "");

  LOG(INFO) << "config info: " << conf.GetConfigInfo();
  
  cppjieba::Jieba jieba(dictPath, 
        modelPath, 
        userDictPath);
  
  ReqHandler reqHandler(jieba);
  ThreadPoolServer server(threadNumber, queueMaxSize, port, reqHandler);
  return server.Start();
}

int main(int argc, char* argv[]) {
  if (!Run(argc, argv)) {
    printf("usage: %s <config_file>\n", argv[0]);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

