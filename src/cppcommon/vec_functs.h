/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#ifndef CPPCOMMON_VEC_FUNCTS_H
#define CPPCOMMON_VEC_FUNCTS_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string>

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

namespace CPPCOMMON
{
	using namespace std;
}

#include "vec_functs.tcc"

#endif
