/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#ifndef LIMONP_TYPEDEFS_H
#define LIMONP_TYPEDEFS_H

#include <stdint.h>
#include <vector>
#include <sys/types.h>
#include <tr1/unordered_map>
#define HashMap std::tr1::unordered_map

namespace Limonp
{
    typedef std::vector<uint16_t> Unicode;
    typedef std::vector<uint16_t>::const_iterator UnicodeConstIterator;
}


#endif
