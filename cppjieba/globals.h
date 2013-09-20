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
//#include <hash_map>
#include <tr1/unordered_map>
//#include <ext/hash_map>

namespace CppJieba
{

    using namespace std;
    using std::tr1::unordered_map;
    //using __gnu_cxx::hash_map;
    //using namespace stdext;
    //typedefs
    typedef std::vector<std::string>::iterator VSI;
    typedef std::vector<uint16_t> Unicode;
    typedef Unicode::const_iterator UniConIter;
    typedef unordered_map<uint16_t, struct TrieNode*> TrieNodeMap;
    typedef unordered_map<uint16_t, double> EmitProbMap;

    const double MIN_DOUBLE = -3.14e+100;
    const double MAX_DOUBLE = 3.14e+100;
}

#endif
