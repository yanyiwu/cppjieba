/************************************
 * file enc : ascii
 * author   : wuyanyi09@gmail.com
************************************/

#ifndef CPPCOMMON_ARGV_FUNCTS_H
#define CPPCOMMON_ARGV_FUNCTS_H

#include "str_functs.h"
#include "map_functs.h"

namespace CPPCOMMON
{
	using namespace std;
	bool getArgvMap(int argc, const char* const* argv, map<string, string>& mpss);
	class ArgvContext
	{
		public :
			ArgvContext(int argc, const char* const * argv);
			~ArgvContext();
		public:
			string toString();
			string operator [](int i);
			string operator [](const string& key);
		private:
			vector<string> _args;
			map<string, string> _mpss;

	};
}

#endif
