#ifndef HUSKY_THREADPOOLSERVER_H
#define HUSKY_THREADPOOLSERVER_H

#include "net_util.h"
#include "worker_thread.h"

namespace husky {
using namespace limonp;

class ThreadPoolServer {
 public:
  ThreadPoolServer(size_t thread_number, size_t queue_max_size, size_t port, IRequestHandler & handler):
    pool_(thread_number, queue_max_size), req_handler_(handler), host_socket_(-1) {
    host_socket_ = CreateAndListenSocket(port);
  }
  ~ThreadPoolServer() {};

  bool Start() {
    pool_.Start();
    sockaddr_in clientaddr;
    socklen_t nSize = sizeof(clientaddr);
    int clientSock;

    while(true) {
      if(-1 == (clientSock = accept(host_socket_, (struct sockaddr*) &clientaddr, &nSize))) {
        LOG(ERROR) << strerror(errno);
        break;
      }
      pool_.Add(CreateTask<WorkerThread,int, IRequestHandler&>(clientSock, req_handler_));
    }
    return true;
  }

 private:
  ThreadPool pool_;
  IRequestHandler & req_handler_;
  int host_socket_;
}; // class ThreadPoolServer
} // namespace husky

#endif
