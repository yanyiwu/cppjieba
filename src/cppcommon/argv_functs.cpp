#include "argv_functs.h"

namespace CPPCOMMON
{
	bool getArgvMap(int argc, const char* const * argv, map<string,string>& mpss)
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

	ArgvContext::ArgvContext(int argc, const char* const * argv)
	{
		for(int i = 0; i < argc; i++)
		{
			if(strStartsWith(argv[i], "--"))
			{
				if(i + 1 < argc && !strStartsWith(argv[i+1], "--"))
				{
					_mpss[argv[i]] = argv[i+1];
					i++;
				}
				else
				{
					break;
				}
			}
			else
			{
				_args.push_back(argv[i]);
			}
		}
	}

	ArgvContext::~ArgvContext()
	{
	}

	string ArgvContext::toString()
	{
		string res;
		res += string_format("[%s]\n", joinStr(_args, ", ").c_str());
		res += mapToString<string, string>(_mpss);
		return res;
	}

	string ArgvContext::operator [](uint i)
	{
		if(i < _args.size())
		{
			return _args[i];
		}
		return "";
	}
	
	string ArgvContext::operator [](const string& key)
	{
		map<string, string>::const_iterator it = _mpss.find(key);
		if(it != _mpss.end())
		{
			return it->second;
		}
		return "";
	}
	
}


#ifdef ARGV_FUNCTS_UT

using namespace CPPCOMMON;

int main(int argc, char** argv)
{
	//map<string,string> argvMap;
	//getArgvMap(argc, argv, argvMap);
	//PRINT_MAPSS(argvMap);
	ArgvContext arg(argc, argv);
	cout<<arg.toString()<<endl;
	cout<<arg[1]<<endl;
	cout<<arg["--hehe"]<<endl;
	cout<<pairToString<int,double>(pair<int, double>(1,1.2))<<endl;
	return 0;
}

#endif
