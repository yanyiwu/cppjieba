#include "TransCode.h"

namespace CppJieba
{
	string TransCode::_enc;
	vector<string> TransCode::_encVec;
	bool TransCode::_isInitted = TransCode::init();
	TransCode::pf_strToVec_t TransCode::_pf_strToVec = NULL;
	TransCode::pf_vecToStr_t TransCode::_pf_vecToStr = NULL;

	bool TransCode::init()
	{
		_encVec.push_back("utf-8");
		_encVec.push_back("gbk");
		_enc = _encVec[1];
		_pf_strToVec = gbkToVec;
		_pf_vecToStr = vecToGbk;
		return true;
	}

	TransCode::TransCode()
	{
		
	}
	TransCode::~TransCode()
	{
	}

	bool TransCode::setEnc(const string& enc)
	{
		if(_encVec.empty())
		{
			return false;
		}
		
		if(isInVec<string>(_encVec, enc))
		{
			_enc = enc;
		}
		else
		{
			return false;
		}
		return true;
	}
	
	bool TransCode::strToVec(const string& str, vector<uint16_t>& vec)
	{
		if(NULL == _pf_strToVec)
		{
			return false;
		}
		return _pf_strToVec(str, vec);
	}

	bool TransCode::utf8ToVec(const string& str, vector<uint16_t>& vec)
	{
		return true;
	}

	bool TransCode::gbkToVec(const string& str, vector<uint16_t>& vec)
	{
		vec.clear();
		if(str.empty())
		{
			return false;
		}
		uint i = 0;
		while(i < str.size())
		{
			if(0 == (str[i] & 0x80))
			{
				vec.push_back(uint16_t(str[i]));
				i++;
			}
			else
			{
				if(i + 1 < str.size()) //&& (str[i+1] & 0x80))
				{
					vec.push_back(twocharToUint16(str[i], str[i + 1]));
					i += 2;
				}
				else
				{
					return false;
				}
			}
		}
		return true;
	}
	
	string TransCode::vecToStr(VUINT16_CONST_ITER begin, VUINT16_CONST_ITER end)
	{
		if(NULL == _pf_vecToStr)
		{
			return "";
		}
		return _pf_vecToStr(begin, end);
	}

	string TransCode::vecToUtf8(VUINT16_CONST_ITER begin, VUINT16_CONST_ITER end)
	{
		return "";
	}

	string TransCode::vecToGbk(VUINT16_CONST_ITER begin, VUINT16_CONST_ITER end)
	{
		if(begin >= end)
		{
			return "";
		}
		string res("");
		while(begin != end)
		{
			pair<char, char> pa = uint16ToChar2(*begin);
			if(pa.first & 0x80)
			{
				res += pa.first;
				res += pa.second;
			}
			else
			{
				res += pa.second;
			}
			begin++;
		}
		return res;
	}

	size_t TransCode::getWordLength(const string& str)
	{
		if(NULL == _pf_strToVec)
		{
			return 0;
		}
		vector<uint16_t> vec;
		bool ret = strToVec(str, vec);
		if(!ret)
		{
			return 0;
		}
		else
		{
			return vec.size();
		}
	}
}


#ifdef CPPJIEBA_TRANSCODE_UT
using namespace CPPCOMMON;
using namespace CppJieba;
int main()
{
	//ifstream ifile("/home/wuyanyi/code/SevKeyword/log.2.txt");
	//string line;
	//VUINT16 vec;
	//while(getline(ifile, line))
	//{
	//	
	//	cout<<line<<endl;
	//	cout<<line.size()<<endl;
	//	if(!TransCode::strToVec(line, vec))
	//	{
	//		cout<<"error"<<endl;
	//	}
	//	PRINT_VECTOR(vec);
	//	cout<<TransCode::vecToStr(vec)<<endl;
	//}
	//ifile.close();
	typedef bool (* pf)(const string& , vector<uint16_t>&);
	pf tmp = TransCode::a;
	vector<uint16_t> vec;
	tmp("1",vec);
	return 0;
}
#endif
