/************************************
 * file enc : ASCII
 * author   : wuyanyi09@gmail.com
************************************/
#include "TransCode.h"

namespace CppJieba
{
	TransCode::TransCode()
	{
	}
	TransCode::~TransCode()
	{
	}

	bool TransCode::strToVec(const string& str, vector<uint16_t>& vec)
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

	string TransCode::vecToStr(const vector<uint16_t>& vec)
	{
		if(vec.empty())
		{
			return "";
		}
		return vecToStr(vec.begin(), vec.end());
	}

	size_t TransCode::getWordLength(const string& str)
	{
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
	ifstream ifile("/home/wuyanyi/code/SevKeyword/log.2.txt");
	string line;
	VUINT16 vec;
	while(getline(ifile, line))
	{
		
		cout<<line<<endl;
		cout<<line.size()<<endl;
		if(!TransCode::strToVec(line, vec))
		{
			cout<<"error"<<endl;
		}
		PRINT_VECTOR(vec);
		cout<<TransCode::vecToStr(vec)<<endl;
	}
	ifile.close();
	return 0;
}
#endif
