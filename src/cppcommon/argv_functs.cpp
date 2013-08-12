#include "argv_functs.h"

namespace CPPCOMMON
{
	bool getArgvMap(int argc, char** argv, map<string,string>& mpss)
	{
		mpss.clear();
		for(int i = 0; i < argc; i++)
		{
			if(strStartsWith(argv[i], "--"))
			{
				if(i + 1 < argc && !strStartsWith(argv[i+1], "--"))
				{
					mpss[argv[i]] = argv[i+1];
					i++;
				}
				else
				{
					return false;
				}
			}
		}
		return true;
	}

	
}


#ifdef ARGV_FUNCTS_UT

using namespace CPPCOMMON;

int main(int argc, char** argv)
{
	map<string,string> argvMap;
	getArgvMap(argc, argv, argvMap);
	PRINT_MAPSS(argvMap);
	return 0;
}

#endif
