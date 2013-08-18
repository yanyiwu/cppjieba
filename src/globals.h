/************************************
 * file enc : ASCII
 * author   : wuyanyi09@gmail.com
************************************/
#ifndef CPPJIEBA_GLOBALS_H
#define CPPJIEBA_GLOBALS_H

#include <map>
#include <vector>
#include <string>
#include <sys/types.h>
#include <stdint.h>

namespace CppJieba
{

	using namespace std;
	//typedefs
	typedef unsigned int uint;
	typedef std::vector<std::string>::iterator VSI;
	typedef std::vector<uint16_t> VUINT16;
	typedef std::vector<uint16_t>::const_iterator VUINT16_CONST_ITER;
	typedef map<uint16_t, struct TrieNode*> TrieNodeMap;

}

#endif
