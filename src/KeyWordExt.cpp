#include "KeyWordExt.h"


namespace CppJieba
{

	KeyWordExt::KeyWordExt()
	{
	}
	
	KeyWordExt::~KeyWordExt()
	{
	}

	bool KeyWordExt::init(const char * const filePath)
	{
		return _segment.init(filePath);
	}
	
	bool KeyWordExt::destroy()
	{
		_segment.destroy();
		return true;
	}

	bool KeyWordExt::extract(const string& utf8Str)
	{
		return true;
	}
	
}


#ifdef KEYWORDEXT_UT

using namespace CppJieba;

int main()
{
	KeyWordExt ext;
	return 0;
}

#endif
