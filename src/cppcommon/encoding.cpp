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
	string UnicodeEncoding::encode(const string& str)
	{
		if(!isUniStrValid(str))
		{
			return "";
		}
		if(UTF8ENC == _encoding)
		{
			return unicodeToUtf8(str);
		}
		else if(GBKENC  == _encoding)
		{
			return utf8ToGbk(unicodeToUtf8(str));
		}
		return "";
	}
	string UnicodeEncoding::decode(const string& str)
	{
		if(str.empty())
		{
			return "";
		}
		string res;
		if(UTF8ENC == _encoding)
		{
			
			res = utf8ToUnicode(str);
			if(isUniStrValid(res))
			{
				return res;
			}
		}
		else if(GBKENC == _encoding)
		{
			res = utf8ToUnicode(gbkToUtf8(str));
			if(isUniStrValid(res))
			{
				return res;
			}
		}
		return "";
	}
}

#ifdef ENCODING_UT
using namespace CPPCOMMON;
int main()
{
	UnicodeEncoding enc;
	ifstream ifile("testdata/dict.utf8");
	string line;
	//enc.setEncoding(UnicodeEncoding::UTF8ENC);
	//enc.setEncoding(UnicodeEncoding::GBKENC);
	//while(getline(ifile, line))
	//{
	//	cout<<line<<endl;
	//	cout<<enc.encode(enc.decode(line))<<endl;
	//	cout<<enc.decode(enc.encode(line))<<endl;
	//	cout<<enc.decode(line)<<endl;
	//	cout<<enc.encode(line)<<endl;
	//}
	ifile.close();
	ifile.open("testdata/dict.gbk");
	enc.setEncoding(GBKENC);
	while(getline(ifile, line))
	{
		
		cout<<line<<endl;
		cout<<line.size()<<endl;
		cout<<enc.encode(enc.decode(line))<<endl;
		cout<<enc.decode(enc.encode(line))<<endl;
		cout<<enc.decode(line)<<endl;
		cout<<enc.encode(line)<<endl;
	}
	ifile.close();
	
	return 0;
}

#endif
