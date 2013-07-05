#include "Segment.h"

namespace CppJieba
{
	Segment::Segment():_trie()
	{
	}
	
	Segment::~Segment()
	{
	}

	bool Segment::init(const char* const dictFilePath)
	{
		_trie.init(dictFilePath);
	}

	bool Segment::destroy()
	{
		return _trie.destroy();
	}

	bool Segment::cutRMM(const string& chStr, vector<string>& res)
	{
		return false;
	}
}


#ifdef SEGMENT_UT
using namespace CppJieba;

int main()
{
	Segment segment;
	segment.init("dict.utf8");
	segment.destroy();
	return 0;
}

#endif
