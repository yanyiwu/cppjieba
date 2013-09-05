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
			if(strStartsWith(argv[i], "-"))
			{
				if(i + 1 < argc && !strStartsWith(argv[i + 1], "-"))
				{
					_mpss[argv[i]] = argv[i+1];
					i++;
				}
				else
				{
					_sset.insert(argv[i]);
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
		stringstream ss;
		ss<<vecToString<string>(_args)<<mapToString<string, string>(_mpss)<<setToString<string>(_sset);
		return ss.str();
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
	
	bool ArgvContext::isKeyExist(const string& key)
	{
		if(_mpss.find(key) != _mpss.end() || _sset.find(key) != _sset.end())
		{
			return true;
		}
		return false;
	}

	uint ArgvContext::getSize() const
	{
		return _args.size();
	}
}


#ifdef ARGV_FUNCTS_UT

using namespace CPPCOMMON;

int main(int argc, char** argv)
{
	return 0;
}

#endif
