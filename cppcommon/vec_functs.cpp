#include "vec_functs.h"

#ifdef TEST_VEC_FUNCTS
using namespace CPPCOMMON;
int main()
{
	vector<int> vec;
	for(int i=0;i<5;i++)
	  vec.push_back(i);
	vector<int> pats;
	pats.push_back(0);
	pats.push_back(3);
	//pats.push_back(4);
	vector<pair<int, vector<int> > > res;
	splitVec<int>(vec,res,pats);
	cout<<isInVec<int>(vec, 0)<<endl;
	cout<<isInVec<int>(vec, -1)<<endl;
	for(int i =0;i<res.size();i++)
	{
		cout<<"first:"<<res[i].first<<endl;
		cout<<"seconde:"<<endl;
		for(int j = 0;j<res[i].second.size();j++)
		{
			cout<<res[i].second[j]<<endl;
		}
		cout<<endl;
	}
	return 0;
}
#endif
