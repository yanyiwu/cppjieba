#ifndef HUSKY_THREADPOOLSERVER_H
#define HUSKY_THREADPOOLSERVER_H

#include "NetUtils.hpp"
#include "WorkerThread.hpp"

namespace husky {
using namespace limonp;

class ThreadPoolServer {
 public:
  ThreadPoolServer(size_t thread_number, size_t queue_max_size, size_t port, IRequestHandler & handler):
    _pool(thread_number, queue_max_size), _reqHandler(handler), _host_socket(-1) {
    _host_socket = CreateAndListenSocket(port);
  }
  ~ThreadPoolServer() {};

  bool start() {
    _pool.start();
    sockaddr_in clientaddr;
    socklen_t nSize = sizeof(clientaddr);
    int clientSock;

    while(true) {
      if(-1 == (clientSock = accept(_host_socket, (struct sockaddr*) &clientaddr, &nSize))) {
        LogError(strerror(errno));
        break;
      }
      _pool.add(CreateTask<WorkerThread,int, IRequestHandler&>(clientSock, _reqHandler));
    }
    return true;
  }

 private:
  ThreadPool _pool;
  IRequestHandler & _reqHandler;
  int _host_socket;
}; // class ThreadPoolServer
} // namespace husky

#endif
