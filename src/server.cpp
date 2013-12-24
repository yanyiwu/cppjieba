#include <unistd.h>
#include <algorithm>
#include <string>
#include <ctype.h>
#include <string.h>
#include "Limonp/Config.hpp"
#include "Husky/HuskyServer.hpp"
#include "MPSegment.hpp"
#include "HMMSegment.hpp"
#include "MixSegment.hpp"

using namespace Husky;
using namespace CppJieba;

class ReqHandler: public IRequestHandler
{
    public:
        ReqHandler(const string& dictPath, const string& modelPath): _segment(dictPath, modelPath){};
        virtual ~ReqHandler(){};
    public:
        virtual bool do_GET(const HttpReqInfo& httpReq, string& strSnd)
        {
            string sentence, tmp;
            vector<string> words;
            httpReq.GET("key", tmp); 
            URLDecode(tmp, sentence);
            _segment.cut(sentence, words);
            strSnd << words;
            return true;
        }
    private:
        MixSegment _segment;
};

bool run(int argc, char** argv)
{
    if(argc < 2)
    {
        return false;
    }
    Config conf(argv[1]);
    if(!conf)
    {
        return false;
    }
    unsigned int port = 0;
    unsigned int threadNum = 0; 
    string dictPath;
    string modelPath;
    string val;
    if(!conf.get("port", val))
    {
        LogFatal("conf get port failed.");
        return false;
    }
    port = atoi(val.c_str());
    if(!conf.get("thread_num", val))
    {
        LogFatal("conf get thread_num failed.");
        return false;
    }
    threadNum = atoi(val.c_str());

    if(!conf.get("dict_path", dictPath))
    {
        LogFatal("conf get dict_path failed.");
        return false;
    }
    if(!conf.get("model_path", modelPath))
    {
        LogFatal("conf get model_path failed.");
        return false;
    }
    if(conf.get("daemonize", val) && "true" == val)
    {
        if(fork() > 0)
          exit(0);
        setsid();
        if(!conf.get("pid_file", val))
        {
            LogFatal("conf get pid_file failed.");
            return false;
        }

        int pid = getpid();
        string pidStr = to_string(pid);
        loadStr2File(val.c_str(), ios::out, pidStr);
        LogInfo("write pid[%s] into file[%s]", pidStr.c_str(), val.c_str());
        
    }

    ReqHandler reqHandler(dictPath, modelPath);
    HuskyServer sf(port, threadNum, &reqHandler);
    return sf.init() && sf.run();
}

int main(int argc, char* argv[])
{
    if(!run(argc, argv))
    {
        printf("usage: %s <config_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

