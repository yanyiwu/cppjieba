/************************************
 * file enc : ascii
 * author   : wuyanyi09@gmail.com
************************************/
#ifndef LIMONP_VEC_FUNCTS_H
#define LIMONP_VEC_FUNCTS_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <sstream>

#define FOR_VECTOR(vec, i) for(size_t i = 0; i < vec.size(); i++)

#define PRINT_VECTOR(vec) FOR_VECTOR(vec, i)\
{\
    cout<<vec[i]<<endl;\
}

#define PRINT_MATRIX(mat) FOR_VECTOR(mat, i) \
{\
    FOR_VECTOR(mat[i], j)\
    {\
        cout<<"["<<i<<","<<j<<"]:"<<mat[i][j]<<endl;\
    }\
}

namespace Limonp
{
    using namespace std;
    template <typename T>
        bool vecToString(const vector<T>& vec, string& res)
        {
            if(vec.empty())
            {
                res = "[]";
                return false;
            }
            stringstream ss;
            ss<<"[\""<<vec[0];
            for(uint i = 1; i < vec.size(); i++)
            {
                ss<<"\", \""<<vec[i];
            }
            ss<<"\"]";
            res = ss.str();
            return true;
        }

    template <typename T>
        string vecToString(const vector<T>& vec)
        {
            string res;
            vecToString(vec, res);
            return res;
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
