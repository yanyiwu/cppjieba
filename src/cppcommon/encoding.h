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
	class UnicodeEncoding
	{
		public:
			static const string& UTF8ENC;
			static const string& GBKENC;
		private:
			string _encoding;
			vector<string> _encVec;
		public:
			UnicodeEncoding(const string& enc);
			~UnicodeEncoding();
		public:
			bool setEncoding(const string& enc);
			string encode(const string& str);
			string decode(const string& str);
		public:
			bool isUniStrValid(const string& unistr)
			{
				return !(unistr.empty() || unistr.size() % 2);
			}
			
	};
}

#endif
