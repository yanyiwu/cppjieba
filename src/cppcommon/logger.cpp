/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#include "logger.h"
namespace CPPCOMMON
{
	const char * Logger::_logFormat =  "%s [File:%s] [Line:%d] [%s] Msg:%s\n";
	const char * Logger::_timeFormat = "%Y-%m-%d %H:%M:%S";
	Logger::Logger()
	{
		_logLevel[LL_DEBUG] = "DEBUG";
		_logLevel[LL_INFO] = "INFO";
		_logLevel[LL_WARN] = "WARN";
		_logLevel[LL_ERROR] = "ERROR";
		_logLevel[LL_FATAL] = "FATAL";

	}

	Logger::~Logger()
	{
	}

	bool Logger::Logging(uint level, const char * msg, const char* fileName, int lineNo)
	{
		if(level > LL_FATAL)
		{
			cerr<<"level's value is out of range"<<endl;
			return false;
		}
		time(&_timeNow);
		size_t ret = strftime(_cStrBuf, sizeof(_cStrBuf), _timeFormat, localtime(&_timeNow));
		if(0 == ret)
		{
			fprintf(stderr, "stftime failed.\n");
			return false;
		}
		if(level >= LL_WARN)
		{
			fprintf(stderr, _logFormat, _cStrBuf, fileName, lineNo, _logLevel[level], msg);
		}
		else
		{
			fprintf(stdout, _logFormat, _cStrBuf, fileName, lineNo, _logLevel[level], msg);
			fflush(stdout);
		}
		return true;
	}
}
namespace CPPCOMMON
{
	Logger loggerSingleTon;
}


#ifdef LOGGER_UT
using namespace CPPCOMMON;
int main()
{
	LogDebug("debug log!");
	LogInfo("test info log");
	LogWarn("warning log");
	LogError("error log");
	LogFatal("fatal !!!!");
	return 0;
}
#endif

