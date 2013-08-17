/************************************
 * file enc : ASCII
 * author   : wuyanyi09@gmail.com
************************************/
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
		private:
			static string _enc;
			static vector<string> _encVec;
			static bool _isInitted;
			
		public:
			static bool setEnc(const string& enc);
			
		public:
			TransCode();
			~TransCode();
		public:
			static bool init();
		public:
			static bool strToVec(const string& str, vector<uint16_t>& vec);
			static string vecToStr(const vector<uint16_t>& vec);
			static string vecToStr(VUINT16_CONST_ITER begin, VUINT16_CONST_ITER end);
			static size_t getWordLength(const string& str);

	};
}

#endif
