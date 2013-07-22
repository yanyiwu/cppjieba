#ifndef CPPJIEBA_TRANSCODE_H
#define CPPJIEBA_TRANSCODE_H


#include "globals.h"
#include "cppcommon/str_functs.h"
#include "cppcommon/vec_functs.h"

namespace CppJieba
{
	using namespace CPPCOMMON;
	class TransCode
	{
		public:
			TransCode();
			~TransCode();
		public:
			static bool strToVec(const string& str, vector<uint16_t>& vec);
			static string vecToStr(const vector<uint16_t>& vec);
			static string vecToStr(VUINT16_CONST_ITER begin, VUINT16_CONST_ITER end);

	};
}

#endif
