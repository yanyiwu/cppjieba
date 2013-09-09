/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#ifndef CPPCOMMON_LOGGER_H
#define CPPCOMMON_LOGGER_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "file_functs.h"
#include "str_functs.h"
#include "typedefs.h"

#define LL_DEBUG 0
#define LL_INFO 1
#define LL_WARN 2
#define LL_ERROR 3
#define LL_FATAL 4
#define LEVEL_ARRAY_SIZE 5
#define CSTR_BUFFER_SIZE 1024


#define LogDebug(msg) Logger::Logging(LL_DEBUG, msg, __FILE__, __LINE__)
#define LogInfo(msg) Logger::Logging(LL_INFO, msg, __FILE__, __LINE__)
#define LogWarn(msg) Logger::Logging(LL_WARN, msg, __FILE__, __LINE__)
#define LogError(msg) Logger::Logging(LL_ERROR, msg, __FILE__, __LINE__)
#define LogFatal(msg) Logger::Logging(LL_FATAL, msg, __FILE__, __LINE__)



namespace CPPCOMMON
{
	using namespace std;
	class Logger
	{
		public:
			Logger();
			~Logger();
		public:
			static bool Logging(uint level, const string& msg, const char* fileName, int lineNo);
			static bool Logging(uint level, const char * msg, const char* fileName, int lineNo);
		private:
			static char _cStrBuf[CSTR_BUFFER_SIZE];
			static const char * _logLevel[LEVEL_ARRAY_SIZE];
			static const char * _logFormat;
			static const char * _timeFormat;
			static time_t _timeNow;
	};
}

#endif
