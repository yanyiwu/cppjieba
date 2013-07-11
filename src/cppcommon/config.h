#ifndef CPPCOMMON_CONFIG_H
#define CPPCOMMON_CONFIG_H


#include <map>
#include <fstream>
#include <iostream>
#include "logger.h"

namespace CPPCOMMON
{
	using std::map;
	using std::string;
	using std::cout;
	using std::endl;
	using std::ifstream;
	class Config
	{
		public:
			Config();
			~Config();
			bool init(const string& configFile);
			void display();
			string getByKey(const string& key);
		private:
			string _stripComment(const string& line);
			map<string, string> _map;
			bool _isInit;

	};
}

namespace CPPCOMMON
{
	extern Config gConfig;
}

#endif
