#include <iostream>
#include <fstream>
#include "src/Segment.h"
using namespace std;

using namespace CppJieba;

int main()
{
	Segment segment;
	
	if(!segment.init("./dicts/segdict.utf8.v2.1"))
	{
		cerr<<"1"<<endl;
		return 1;
	}
	//segment.init("dicts/jieba.dict.utf8");
	
	vector<string> res;
	string title;
	title = "我来到北京清华大学";
	res.clear();
	segment.extract(title, res, 10);
	
	title = "特价！camel骆驼 柔软舒适头层牛皮平底凉鞋女 休闲平跟妈妈鞋夏";
	res.clear();
	segment.extract(title, res, 10);

	title = "包邮拉菲草18cm大檐进口草帽子超强遮阳防晒欧美日韩新款夏天 女";
	res.clear();
	segment.extract(title, res, 10);

	title = "2013新款19CM超大檐帽 遮阳草帽子 沙滩帽防晒大檐欧美新款夏天女";
	res.clear();
	segment.extract(title, res, 10);

	segment.destroy();
	
    return 0;
}
