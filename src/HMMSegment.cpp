#include "HMMSegment.h"

namespace CppJieba
{
	HMMSegment::HMMSegment()
	{
	}
	
	HMMSegment::~HMMSegment()
	{
	}

	bool HMMSegment::init()
	{
		return true;
	}
	
	bool HMMSegment::dispose()
	{
		return true;
	}

	bool HMMSegment::loadModel()
	{
		return true;
	}
}


#ifdef HMMSEGMENT_UT
using namespace CppJieba;

int main()
{
	HMMSegment hmm;
	
	return 0;
}

#endif
