/************************************
 * file enc : utf-8
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
			typedef bool (*pf_decode_t)(const string&, vector<uint16_t>&);
			typedef string (*pf_encode_t)(Unicode::const_iterator begin, Unicode::const_iterator end);
			typedef size_t (*pf_getWordLength_t)(const string& str);
		private:
			static vector<string> _encVec;
			static bool _isInitted;
			static pf_decode_t _pf_decode;
			static pf_encode_t _pf_encode;
			static pf_getWordLength_t _pf_getWordLength;
			
		public:
			static void setGbkEnc();
			static void setUtf8Enc();
			
		public:
			TransCode();
			~TransCode();
		public:
			static bool init();
		public:
			static bool decode(const string& str, vector<uint16_t>& vec);
			static string encode(Unicode::const_iterator begin, Unicode::const_iterator end);
			//static size_t getWordLength(const string& str);
		public:
			static bool gbkToVec(const string& str, vector<uint16_t>& vec);
			static string vecToGbk(Unicode::const_iterator begin, Unicode::const_iterator end);
			//static size_t getGbkLength(const string& str);
		public:
			static bool utf8ToVec(const string& str, vector<uint16_t>& vec);
			static string vecToUtf8(Unicode::const_iterator begin, Unicode::const_iterator end);
			//static size_t getUtf8Length(const string& str);
	};
}

#endif
