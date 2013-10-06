#include "sort_functs.h"

#ifdef UT
using namespace CPPCOMMON;
struct NodeTest
{
	int sid;
	char s[128];
	bool operator==(const NodeTest& rhs)
	{
		return sid ==  rhs.sid && 0 == strcmp(s, rhs.s);
	}
};

bool cmp(const NodeTest& lhs, const NodeTest& rhs)
{
	return lhs.sid < rhs.sid;
}



int main()
{
	const char* a = "fa";
	const char* b = "fb";
	FILE * fa = fopen(a, "w");
	NodeTest n1, n2, n3;
	n1.sid = 2;
	strcpy(n1.s, "n1s");
	fwrite(&n1, sizeof(NodeTest), 1, fa);
	n2.sid = 1;
	strcpy(n2.s, "n2s");
	fwrite(&n2, sizeof(NodeTest), 1, fa);
	n3.sid = 3;
	strcpy(n3.s, "n3s");
	fwrite(&n3, sizeof(NodeTest), 1, fa);
	fclose(fa);
	int memSize = 32*1024*1024;
	K_MergeFile<NodeTest>(a, b, cmp, memSize);
	
	NodeTest res[3];
	FILE * fb = fopen(b,"r");
	fread(res, sizeof(NodeTest), 3, fb);
	for(int i =0;i<3;i++)
	{
		printf("%d %s\n", res[i].sid, res[i].s);
	}
	
	return 0;
}
#endif
