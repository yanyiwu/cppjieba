#include "encoding.h"

namespace CPPCOMMON
{
	const string& UnicodeEncoding::UTF8ENC = "utf-8";
	const string& UnicodeEncoding::GBKENC = "gbk";
	
	UnicodeEncoding::UnicodeEncoding()
	{
		_encVec.push_back(UTF8ENC);
		_encVec.push_back(GBKENC);
		_encoding = UTF8ENC;
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
			cout<<__FILE__<<__LINE__<<endl;
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
		cout<<__FILE__<<__LINE__<<endl;
		return "";
	}
	string UnicodeEncoding::decode(const string& str)
	{
		if(str.empty())
		{
			cout<<__FILE__<<__LINE__<<endl;
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
		cout<<__FILE__<<__LINE__<<endl;
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
	enc.setEncoding(UnicodeEncoding::GBKENC);
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
