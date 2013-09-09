/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#include "encoding.h"

namespace CPPCOMMON
{
	
	//UnicodeEncoding::UnicodeEncoding(const string& enc)
	//{
	//	
	//	_encVec.push_back(UTF8ENC);
	//	_encVec.push_back(GBKENC);
	//	
	//	if(!isInVec<string>(_encVec, enc))
	//	{
	//		//default
	//		_encoding = UTF8ENC;
	//	}
	//	else
	//	{
	//		_encoding = enc;
	//	}
	//}

	//UnicodeEncoding::~UnicodeEncoding()
	//{
	//}
	//
	//bool UnicodeEncoding::setEncoding(const string& enc)
	//{
	//	if(!isInVec<string>(_encVec, enc))
	//	{
	//		return false;
	//	}
	//	_encoding = enc;
	//	return true;
	//}

	//string UnicodeEncoding::encode(UnicodeConstIterator begin, UnicodeConstIterator end)
	//{
	//	if(begin >= end)
	//	{
	//		return "";
	//	}
	//	Unicode unicode(begin, end);
	//	return encode(unicode);
	//}

	//string UnicodeEncoding::encode(const Unicode& unicode)
	//{
	//	if(unicode.empty())
	//	{
	//		return "";
	//	}
	//	if(UTF8ENC == _encoding)
	//	{
	//		return unicodeToUtf8(unicode);
	//	}
	//	else if(GBKENC  == _encoding)
	//	{
	//		return utf8ToGbk(unicodeToUtf8(unicode));
	//	}
	//	return "";
	//}

	//bool UnicodeEncoding::decode(const string& str, Unicode& unicode)
	//{
	//	if(str.empty())
	//	{
	//		return false;
	//	}
	//	if(UTF8ENC == _encoding)
	//	{
	//		return utf8ToUnicode(str, unicode);
	//	}
	//	else if(GBKENC == _encoding)
	//	{
	//		return utf8ToUnicode(gbkToUtf8(str), unicode);
	//	}
	//	return false;
	//}

	//size_t UnicodeEncoding::getWordLength(const string& str)
	//{
	//	Unicode unicode;
	//	decode(str, unicode);
	//	return unicode.size();
	//}

}

#ifdef ENCODING_UT
using namespace CPPCOMMON;
int main()
{
	UnicodeEncoding enc(GBKENC);
	ifstream ifile("testdata/dict.gbk");
	Unicode unicode;
	string line;
	while(getline(ifile, line))
	{
		
		cout<<line<<endl;
		cout<<line.size()<<endl;
		cout<<enc.getWordLength(line)<<endl;
		enc.decode(line, unicode);
		printUnicode(unicode);
		cout<<enc.encode(unicode)<<endl;
	}
	ifile.close();
	
	return 0;
}

#endif
