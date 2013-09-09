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
			typedef bool (*pf_encode_t)(Unicode::const_iterator begin, Unicode::const_iterator end, string& res);
		private:
			static vector<string> _encVec;
			static bool _isInitted;
			static pf_decode_t _pf_decode;
			static pf_encode_t _pf_encode;
			
		public:
			static void setGbkEnc();
			static void setUtf8Enc();
			
		private:
			TransCode();
			~TransCode();
		public:
			static bool init();
		public:
			static inline bool decode(const string& str, vector<uint16_t>& vec);
			static inline bool encode(Unicode::const_iterator begin, Unicode::const_iterator end, string& res);
            static inline bool encode(const Unicode& sentence, string& res);

		public:
			static bool gbkToVec(const string& str, vector<uint16_t>& vec);
			static bool vecToGbk(Unicode::const_iterator begin, Unicode::const_iterator end, string& res);
		public:
			static bool utf8ToVec(const string& str, vector<uint16_t>& vec);
			static bool vecToUtf8(Unicode::const_iterator begin, Unicode::const_iterator end, string& res);
	};

	inline bool TransCode::decode(const string& str, vector<uint16_t>& vec)
	{
		if(NULL == _pf_decode)
		{
			return false;
		}
		return _pf_decode(str, vec);
	}
    inline bool TransCode::encode(const Unicode& sentence, string& res)
    {
        return encode(sentence.begin(), sentence.end(), res);
    }
	
	inline bool TransCode::encode(Unicode::const_iterator begin, Unicode::const_iterator end, string& res)
	{
		if(!_pf_encode)
		{
			return false;
		}
		return _pf_encode(begin, end, res);
	}
}

#endif
