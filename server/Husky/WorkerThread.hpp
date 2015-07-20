#ifndef HUSKY_WORKER_HPP
#define HUSKY_WORKER_HPP

#include "Limonp/ThreadPool.hpp"
#include "IRequestHandler.hpp"
#include "NetUtils.hpp"

namespace Husky {
const char* const CLIENT_IP_K = "CLIENT_IP";
const size_t RECV_BUFFER_SIZE = 16 * 1024;

const struct linger LNG = {1, 1};
const struct timeval SOCKET_TIMEOUT = {16, 0};

class WorkerThread: public ITask {
 public:
  WorkerThread(int sockfs, const IRequestHandler& reqHandler):
    _sockfd(sockfs), _reqHandler(reqHandler) {
  }
  virtual ~WorkerThread() {
  }
 private:
  int _sockfd;
  const IRequestHandler& _reqHandler;

 public:
  void run() {
    do {
      if(!_setsockopt(_sockfd)) {
        LogError("_getsockopt failed.");
        break;
      }
      string strSnd, strRetByHandler;
      HttpReqInfo httpReq;
      if(!_receive(_sockfd, httpReq)) {
        LogError("_receive failed.");
        break;
      }

      if(httpReq.isGET() && !_reqHandler.do_GET(httpReq, strRetByHandler)) {
        LogError("do_GET failed.");
        break;
      }
      if(httpReq.isPOST() && !_reqHandler.do_POST(httpReq, strRetByHandler)) {
        LogError("do_POST failed.");
        break;
      }
      strSnd = string_format(HTTP_FORMAT, CHARSET_UTF8, strRetByHandler.length(), strRetByHandler.c_str());

      if(!_send(_sockfd, strSnd)) {
        LogError("_send failed.");
        break;
      }
      LogInfo("response:%s", strRetByHandler.c_str());
    } while(false);


    if(-1 == close(_sockfd)) {
      LogError(strerror(errno));
    }
  }
 private:
  bool _receive(int sockfd, HttpReqInfo& httpInfo) const {
    char recvBuf[RECV_BUFFER_SIZE];
    int n = 0;
    while(!httpInfo.isBodyFinished() && (n = recv(sockfd, recvBuf, RECV_BUFFER_SIZE, 0)) > 0) {
      if(!httpInfo.isHeaderFinished()) {
        if(!httpInfo.parseHeader(recvBuf, n)) {
          LogError("parseHeader failed. ");
          return false;
        }
        continue;
      }
      httpInfo.appendBody(recvBuf, n);
    }
    if(n < 0) {
      LogError(strerror(errno));
      return false;
    }
    return true;
  }
  bool _send(int sockfd, const string& strSnd) const {
    if(-1 == send(sockfd, strSnd.c_str(), strSnd.length(), 0)) {
      LogError(strerror(errno));
      return false;
    }
    return true;
  }
  bool _setsockopt(int sockfd) const {
    if(-1 == setsockopt(sockfd, SOL_SOCKET, SO_LINGER, (const char*)&LNG, sizeof(LNG))) {
      LogError(strerror(errno));
      return false;
    }
    if(-1 == setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&SOCKET_TIMEOUT, sizeof(SOCKET_TIMEOUT))) {
      LogError(strerror(errno));
      return false;
    }
    if(-1 == setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&SOCKET_TIMEOUT, sizeof(SOCKET_TIMEOUT))) {
      LogError(strerror(errno));
      return false;
    }
    return true;
  }

};
}

#endif
