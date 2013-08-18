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
		public:
			typedef bool (*pf_strToVec_t)(const string&, vector<uint16_t>&);
			typedef string (*pf_vecToStr_t)(VUINT16_CONST_ITER begin, VUINT16_CONST_ITER end);
			typedef size_t (*pf_getWordLength_t)(const string& str);
		private:
			static string _enc;
			static vector<string> _encVec;
			static bool _isInitted;
			static pf_strToVec_t _pf_strToVec;
			static pf_vecToStr_t _pf_vecToStr;
			static pf_getWordLength_t _pf_getWordLength;
			
		public:
			static bool setEnc(const string& enc);
			
		public:
			TransCode();
			~TransCode();
		public:
			static bool init();
		public:
			static bool strToVec(const string& str, vector<uint16_t>& vec);
			static string vecToStr(VUINT16_CONST_ITER begin, VUINT16_CONST_ITER end);
			static size_t getWordLength(const string& str);
		public:
			static bool gbkToVec(const string& str, vector<uint16_t>& vec);
			static string vecToGbk(VUINT16_CONST_ITER begin, VUINT16_CONST_ITER end);
			static size_t getGbkLength(const string& str);
		public:
			static bool utf8ToVec(const string& str, vector<uint16_t>& vec);
			static string vecToUtf8(VUINT16_CONST_ITER begin, VUINT16_CONST_ITER end);
			static size_t getUtf8Length(const string& str);
	};
}

#endif
