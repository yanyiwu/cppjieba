#include <unistd.h>
#include <algorithm>
#include <string>
#include <ctype.h>
#include <string.h>
#include "Limonp/Config.hpp"
#include "Limonp/io_functs.hpp"
#include "Husky/EpollServer.hpp"
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
        virtual bool do_GET(const HttpReqInfo& httpReq, string& strSnd) const
        {
            string sentence, tmp;
            vector<string> words;
            httpReq.GET("key", tmp); 
            URLDecode(tmp, sentence);
            _segment.cut(sentence, words);
            if(httpReq.GET("format", tmp) && tmp == "simple")
            {
                join(words.begin(), words.end(), strSnd, " ");
                return true;
            }
            strSnd << words;
            return true;
        }
        virtual bool do_POST(const HttpReqInfo& httpReq, string& strSnd) const
        {
            vector<string> words;
            _segment.cut(httpReq.getBody(), words);
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
    size_t port = 0;
    string dictPath;
    string modelPath;
    string val;
    if(!conf.get("port", val))
    {
        LogFatal("conf get port failed.");
        return false;
    }
    port = atoi(val.c_str());

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
    EpollServer sf(port, reqHandler);
    return sf.start();
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

