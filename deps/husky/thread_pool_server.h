#ifndef HUSKY_THREADPOOLSERVER_H
#define HUSKY_THREADPOOLSERVER_H

#include "net_util.h"
#include "irequest_handler.h"
#include "limonp/ThreadPool.hpp"

namespace husky {
using namespace limonp;

const char* const CLIENT_IP_K = "CLIENT_IP";
const size_t RECV_BUFFER_SIZE = 16 * 1024;

const struct linger LNG = {1, 1};
const struct timeval SOCKET_TIMEOUT = {16, 0};


class ThreadPoolServer {
 public:
  ThreadPoolServer(size_t thread_number, size_t port, IRequestHandler & handler):
    pool_(thread_number), req_handler_(handler), host_socket_(-1) {
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
      pool_.Add(NewClosure(this, &ThreadPoolServer::Run, clientSock));
      //pool_.Add(CreateTask<Worker,int, IRequestHandler&>(clientSock, req_handler_));
    }
    return true;
  }

 private:
  void Run(int sockfd) {
    do {
      if(!SetSockopt(sockfd)) {
        LOG(ERROR) << "_getsockopt failed.";
        break;
      }
      string strSnd, strRetByHandler;
      HttpReqInfo httpReq;
      if(!Receive(sockfd, httpReq)) {
        LOG(ERROR) << "Receive failed.";
        break;
      }

      if(httpReq.IsGET() && !req_handler_.DoGET(httpReq, strRetByHandler)) {
        LOG(ERROR) << "DoGET failed.";
        break;
      }
      if(httpReq.IsPOST() && !req_handler_.DoPOST(httpReq, strRetByHandler)) {
        LOG(ERROR) << "DoPOST failed.";
        break;
      }
      strSnd = StringFormat(HTTP_FORMAT, CHARSET_UTF8, strRetByHandler.length(), strRetByHandler.c_str());

      if(!Send(sockfd, strSnd)) {
        LOG(ERROR) << "Send failed.";
        break;
      }
    } while(false);


    if(-1 == close(sockfd)) {
      LOG(ERROR) << strerror(errno);
    }
  }
  bool Receive(int sockfd, HttpReqInfo& httpInfo) const {
    char recvBuf[RECV_BUFFER_SIZE];
    int n = 0;
    while(!httpInfo.IsBodyFinished() && (n = recv(sockfd, recvBuf, RECV_BUFFER_SIZE, 0)) > 0) {
      if(!httpInfo.IsHeaderFinished()) {
        if(!httpInfo.ParseHeader(recvBuf, n)) {
          LOG(ERROR) << "ParseHeader failed. ";
          return false;
        }
        continue;
      }
      httpInfo.AppendBody(recvBuf, n);
    }
    if(n < 0) {
      LOG(ERROR) << strerror(errno);
      return false;
    }
    return true;
  }
  bool Send(int sockfd, const string& strSnd) const {
    if(-1 == send(sockfd, strSnd.c_str(), strSnd.length(), 0)) {
      LOG(ERROR) << strerror(errno);
      return false;
    }
    return true;
  }
  bool SetSockopt(int sockfd) const {
    if(-1 == setsockopt(sockfd, SOL_SOCKET, SO_LINGER, (const char*)&LNG, sizeof(LNG))) {
      LOG(ERROR) << strerror(errno);
      return false;
    }
    if(-1 == setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&SOCKET_TIMEOUT, sizeof(SOCKET_TIMEOUT))) {
      LOG(ERROR) << strerror(errno);
      return false;
    }
    if(-1 == setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&SOCKET_TIMEOUT, sizeof(SOCKET_TIMEOUT))) {
      LOG(ERROR) << strerror(errno);
      return false;
    }
    return true;
  }

  ThreadPool pool_;
  IRequestHandler & req_handler_;
  int host_socket_;
}; // class ThreadPoolServer
} // namespace husky

#endif
