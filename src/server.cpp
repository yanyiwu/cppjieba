#include <unistd.h>
#include <algorithm>
#include <string>
#include <ctype.h>
#include <string.h>
#include "Limonp/Config.hpp"
#include "Husky/ServerFrame.hpp"
#include "MPSegment.hpp"
#include "HMMSegment.hpp"
#include "MixSegment.hpp"

using namespace Husky;
using namespace CppJieba;

class ReqHandler: public IRequestHandler
{
    private:
        string _dictPath;
        string _modelPath;
    public:
        ReqHandler(const string& dictPath, const string& modelPath): _dictPath(dictPath), _modelPath(modelPath){};
        virtual ~ReqHandler(){};
        virtual bool init(){return _segment.init(_dictPath.c_str(), _modelPath.c_str());};
        virtual bool dispose(){return _segment.dispose();};
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
    Config conf;
    if(!conf.loadFile(argv[1]))
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

    ReqHandler reqHandler(dictPath, modelPath);
    ServerFrame sf(port, threadNum, &reqHandler);
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

