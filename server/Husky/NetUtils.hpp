#ifndef HUSKY_NET_UTILS_HPP
#define HUSKY_NET_UTILS_HPP

#include <stdio.h>
#include <string.h>

#include <cassert>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <vector>

#include "Limonp/StdExtension.hpp"
#include "Limonp/HandyMacro.hpp"

namespace Husky {
static const size_t LISTEN_QUEUE_LEN = 1024;

typedef int SocketFd;
SocketFd CreateAndListenSocket(int port) {
  SocketFd sock;
  sock = socket(AF_INET, SOCK_STREAM, 0);
  LIMONP_CHECK(sock != -1);

  int optval = 1; // nozero
  int ret;
  ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  LIMONP_CHECK(-1 != ret);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  ret = ::bind(sock, (sockaddr*)&addr, sizeof(addr));
  LIMONP_CHECK(-1 != ret);

  ret = listen(sock, LISTEN_QUEUE_LEN);
  LIMONP_CHECK(-1 != ret);

  return sock;
}

const char* const HTTP_FORMAT = "HTTP/1.1 200 OK\r\nConnection: close\r\nServer: HuskyServer/1.0.0\r\nContent-Type: text/json; charset=%s\r\nContent-Length: %d\r\n\r\n%s";
const char* const CHARSET_UTF8 = "UTF-8";
} // namespace Husky


#endif
