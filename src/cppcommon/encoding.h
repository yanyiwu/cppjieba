/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/

#ifndef CPPCOMMON_ENCODING_H
#define CPPCOMMON_ENCODING_H

#include "str_functs.h"
#include "vec_functs.h"

namespace CPPCOMMON
{
	using namespace std;

	const char* const  UTF8ENC = "utf-8";
	const char* const  GBKENC = "gbk";

	class UnicodeEncoding
	{
		private:
			string _encoding;
			vector<string> _encVec;
		public:
			UnicodeEncoding(const string& enc);
			~UnicodeEncoding();
		public:
			bool setEncoding(const string& enc);
			string encode(const vector<uint16_t>& unicode);
			bool decode(const string& str, vector<uint16_t>& unicode);
			
	};
}

#endif
