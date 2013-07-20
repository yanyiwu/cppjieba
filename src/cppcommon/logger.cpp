/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#include "logger.h"
namespace CPPCOMMON
{
	const char * Logger::_logFormat =  "%s [File:%s] [Line:%d] [%s] Msg:%s";
	const char * Logger::_timeFormat = "%Y-%m-%d %H:%M:%S";
	const char * Logger::_logDir = "./log/";
	const char * Logger::_logName = "run.log";
	Logger::Logger()
	{

		_isCoutOpen = true;
		_logLevel[LL_DEBUG] = "DEBUG";
		_logLevel[LL_INFO] = "INFO";
		_logLevel[LL_WARN] = "WARN";
		_logLevel[LL_ERROR] = "ERROR";
		_logLevel[LL_FATAL] = "FATAL";
		InitDefault();
	}
	Logger::~Logger()
	{
		if(_logFile)
		{
			_logFile.close();
		}
	}
	void Logger::InitDefault()
	{
		_logCoutLevel = LL_INFO;
		_logFileLevel = LL_DEBUG;
		if(!checkDirExist(_logDir))
		{
			createDir(_logDir);
		}
		_logFile.open((string(_logDir) + string(_logName)).c_str(), ios::app);

	}
	bool Logger::Logging(unsigned int level, const string& msg, const string& fileName, const int& lineNo)
	{
		if(level < LL_DEBUG || level > LL_FATAL)
		{
			cerr<<"level's value is out of range"<<endl;
			return false;
		}
		time(&_timeNow);
		strftime(_cStrBuf, sizeof(_cStrBuf), _timeFormat, localtime(&_timeNow));
		string timeStr = _cStrBuf;
		sprintf(_cStrBuf, _logFormat, timeStr.c_str(), fileName.c_str(), lineNo, _logLevel[level], msg.c_str());
		if(_isCoutOpen && level >= _logCoutLevel)
		{
			cerr<<_cStrBuf<<endl;
		}
		if(_logFile && level >= _logFileLevel)
		{
			_logFile<<_cStrBuf<<endl;
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

