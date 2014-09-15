#ifndef HUSKY_EPOLLSERVER_H 
#define HUSKY_EPOLLSERVER_H 

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
#include "WorkerThread.hpp"

namespace Husky
{
    using namespace Limonp;

    class ThreadPoolServer
    {
        private:
            static const size_t LISTEN_QUEUE_LEN = 1024;

        private:
            ThreadPool _pool;
            const IRequestHandler & _reqHandler;
            int _host_socket;
        public:
            ThreadPoolServer(size_t thread_number, size_t queue_max_size, size_t port, const IRequestHandler & handler): 
                _pool(thread_number, queue_max_size), _reqHandler(handler), _host_socket(-1)
            {
                _init_host_socket(port);
            };
            ~ThreadPoolServer(){};
        public:
            bool start()
            {
                _pool.start();
                sockaddr_in clientaddr;
                socklen_t nSize = sizeof(clientaddr);
                int clientSock;

                while(true)
                {
                    if(-1 == (clientSock = accept(_host_socket, (struct sockaddr*) &clientaddr, &nSize))) 
                    {
                        LogError(strerror(errno));
                        break;
                        //continue;
                    }
                    _pool.add(CreateTask<WorkerThread,int, const IRequestHandler&>(clientSock, _reqHandler));
                    //_response(clientSock);
                    //_closesocket(clientSock);

                }
                return true;
            }
        private:
            bool _init_host_socket(size_t port)
            { 
                _host_socket = socket(AF_INET, SOCK_STREAM, 0);
                if(-1 == _host_socket)
                {
                    LogFatal(strerror(errno));
                    return false;
                }

                int nRet = 1;
                if(-1 == setsockopt(_host_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&nRet, sizeof(nRet)))
                {
                    LogFatal(strerror(errno));
                    return false;
                }

                struct sockaddr_in addrSock;
                addrSock.sin_family = AF_INET;
                addrSock.sin_port = htons(port);
                addrSock.sin_addr.s_addr = htonl(INADDR_ANY);
                if(-1 == ::bind(_host_socket, (sockaddr*)&addrSock, sizeof(sockaddr)))
                {
                    LogFatal(strerror(errno));
                    _closesocket(_host_socket);
                    return false;
                }
                if(-1 == listen(_host_socket, LISTEN_QUEUE_LEN))
                {
                    LogFatal(strerror(errno));
                    return false;
                }

                LogInfo("create socket listening port[%u] init ok", port);
                return true;
            }
            void _closesocket(int sockfd)
            {
                if(-1 == close(sockfd))
                {
                    LogError(strerror(errno));
                    return;
                }
            }
    };
}
#endif
