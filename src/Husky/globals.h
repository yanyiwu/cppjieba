#ifndef HUSKY_GLOBALS_H
#define HUSKY_GLOBALS_H
#include <string>
#include <vector>
#include <set>
#include <string>
#include <stdlib.h>


namespace Husky
{

    const char* const RESPONSE_CHARSET_UTF8 = "UTF-8";
    const char* const RESPONSE_CHARSET_GB2312 = "GB2312";
    const char* const CLIENT_IP_K = "CLIENT_IP"; 

    const unsigned int SOCKET_TIMEOUT = 2;

    const char* const RESPONSE_FORMAT = "HTTP/1.1 200 OK\r\nConnection: close\r\nServer: FrameServer/1.0.0\r\nContent-Type: text/json; charset=%s\r\nContent-Length: %d\r\n\r\n";
    typedef unsigned short  u_short;
    typedef unsigned int    u_int;

}
#endif
