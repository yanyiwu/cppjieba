/************************************
 * file enc : ascii
 * author   : wuyanyi09@gmail.com
************************************/


#ifndef CPPCOMMON_MAP_FUNCTS_H
#define CPPCOMMON_MAP_FUNCTS_H

#define PRINT_MAPSS(mpss) \
{\
	for(map<string, string>::const_iterator it = mpss.begin(); it != mpss.end(); it++)\
	{\
		cout<<it->first<<' '<<it->second<<endl;\
	}\
}

#endif
