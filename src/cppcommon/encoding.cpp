/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#include "encoding.h"

namespace CPPCOMMON
{
	
	UnicodeEncoding::UnicodeEncoding(const string& enc)
	{
		
		_encVec.push_back(UTF8ENC);
		_encVec.push_back(GBKENC);
		
		if(!isInVec<string>(_encVec, enc))
		{
			//default
			_encoding = UTF8ENC;
		}
		else
		{
			_encoding = enc;
		}
	}

	UnicodeEncoding::~UnicodeEncoding()
	{
	}
	
	bool UnicodeEncoding::setEncoding(const string& enc)
	{
		if(!isInVec<string>(_encVec, enc))
		{
			return false;
		}
		_encoding = enc;
		return true;
	}

	string UnicodeEncoding::encode(const vector<uint16_t>& unicode)
	{
		if(unicode.empty())
		{
			return "";
		}
		if(UTF8ENC == _encoding)
		{
			return unicodeToUtf8(unicode);
		}
		else if(GBKENC  == _encoding)
		{
			return utf8ToGbk(unicodeToUtf8(unicode));
		}
		return "";
	}

	bool UnicodeEncoding::decode(const string& str, vector<uint16_t>& unicode)
	{
		if(str.empty())
		{
			return false;
		}
		if(UTF8ENC == _encoding)
		{
			return utf8ToUnicode(str, unicode);
		}
		else if(GBKENC == _encoding)
		{
			return utf8ToUnicode(gbkToUtf8(str), unicode);
		}
		return false;
	}

}

#ifdef ENCODING_UT
using namespace CPPCOMMON;
int main()
{
	UnicodeEncoding enc(GBKENC);
	ifstream ifile("testdata/dict.gbk");
	vector<uint16_t> unicode;
	string line;
	while(getline(ifile, line))
	{
		
		cout<<line<<endl;
		cout<<line.size()<<endl;
		enc.decode(line, unicode);
		printUnicode(unicode);
		cout<<enc.encode(unicode)<<endl;
	}
	ifile.close();
	
	return 0;
}

#endif
