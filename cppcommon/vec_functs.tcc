/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/

#ifndef CPPCOMMON_VEC_FUNCTS_TCC
#define CPPCOMMON_VEC_FUNCTS_TCC

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <sstream>
namespace CPPCOMMON
{
	using namespace std;
	template<typename T>
		string vecToString(const vector<T>& vec)
		{
			if(vec.empty())
			{
				return "[]";
			}
			stringstream ss;
			ss<<"["<<vec[0];
			for(unsigned int i = 1; i < vec.size(); i++)
			{
				ss<<","<<vec[i];
			}
			ss<<"]";
			return ss.str();
		}
	template<typename T>
		bool isInVec(const vector<T>& vec, const T& item)
		{
			typename vector<T>::const_iterator it	= find(vec.begin(), vec.end(), item);
			return it != vec.end();
		}
	template<typename T>
		void splitVec(const vector<T>& vecSrc, vector< pair<T, vector<T> > >& outVec, const vector<T>& patterns)
		{
			vector<T> tmp;
			T pattern;
			size_t patternSize = patterns.size();
			for(size_t i = 0; i < vecSrc.size(); i++)
			{
				size_t patternPos = patternSize;
				for(size_t j = 0; j < patternSize; j++)
				{
					if(patterns[j] == vecSrc[i])
					{
						patternPos = j;
						break;
					}
				}
				if(patternPos != patternSize)
				{
					if(!tmp.empty())
					{
						outVec.push_back(make_pair<T, vector<T> >(pattern, tmp));
						tmp.clear();
					}
					pattern = patterns[patternPos];
				}
				else
				{
					tmp.push_back(vecSrc[i]);
				}
			}
			if(!tmp.empty())
			{
				outVec.push_back(make_pair<T, vector<T> >(pattern, tmp));
			}
		}

	template<typename T>
		void splitVec(const vector<T>& vecSrc, vector< vector<T> >& outVec, const vector<T>& patternVec)
		{
			vector<T> tmp;
			for(size_t i = 0; i < vecSrc.size(); i++)
			{
				bool flag = false;
				for(size_t j = 0; j < patternVec.size(); j++)
				{
					if(patternVec[j] == vecSrc[i])
					{
						flag = true;
						break;
					}
				}
				if(flag)
				{
					if(!tmp.empty())
					{
						outVec.push_back(tmp);
						tmp.clear();
					}
				}
				else
				{
					tmp.push_back(vecSrc[i]);
				}
			}
			if(!tmp.empty())
			{
				outVec.push_back(tmp);
			}
		}

}
#endif

