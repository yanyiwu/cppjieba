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
	bool getArgvMap(int argc,  char** argv, map<string, string>& mpss);
}

#endif
