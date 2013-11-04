#include <unistd.h>
#include <algorithm>
#include <string>
#include <ctype.h>
#include <string.h>
#include "Husky/Limonp/ArgvContext.hpp"
#include "Husky/Daemon.h"
#include "Husky/ServerFrame.h"
#include "MPSegment.h"
#include "HMMSegment.h"
#include "MixSegment.h"

using namespace Husky;
using namespace CppJieba;

const char * const DEFAULT_DICTPATH = "../dicts/jieba.dict.utf8";
const char * const DEFAULT_MODELPATH = "../dicts/hmm_model.utf8";

class ServerDemo: public IRequestHandler
{
	public:
        ServerDemo(){};
        virtual ~ServerDemo(){};
		virtual bool init(){return _segment.init(DEFAULT_DICTPATH, DEFAULT_MODELPATH);};
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

int main(int argc,char* argv[])
{
	if(argc != 7)
	{
		printf("usage: %s -n THREAD_NUMBER -p LISTEN_PORT -k start|stop\n",argv[0]);
		return -1;
	}
    ArgvContext arg(argc, argv);
    unsigned int port = atoi(arg["-p"].c_str());
    unsigned int threadNum = atoi(arg["-n"].c_str());

    ServerDemo s;
    Daemon daemon(&s);
    if(arg["-k"] == "start")
    {
        return !daemon.Start(port, threadNum);
    }
    else
    {
        return !daemon.Stop();
    }
}

