#ifndef HUSKY_SERVERFRAME_H 
#define HUSKY_SERVERFRAME_H 

#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <vector>
#include "globals.h"
#include "ThreadManager.hpp"
#include "HttpReqInfo.hpp"
#include "Daemon.h"

#define INVALID_SOCKET  -1 
#define SOCKET_ERROR    -1 
#define closesocket     close
#define  RECV_BUFFER     10240
#define  LISEN_QUEUR_LEN 1024


namespace Husky
{
    using namespace Limonp;
    typedef	int SOCKET;

    class IRequestHandler 
    {
        public:
            virtual ~IRequestHandler(){};
        public:
            virtual bool init() = 0;
            virtual bool dispose() = 0;

            virtual bool do_GET(const HttpReqInfo& httpReq, string& res) = 0;

    };

    struct SPara
    {
        SOCKET hSock;
        IRequestHandler * pHandler;
    };

    class ServerFrame: public IWorkHandler
    {
        public:
            ServerFrame(unsigned nPort, unsigned nThreadCount, IRequestHandler* pHandler)
            {
                m_nLsnPort = nPort;
                m_nThreadCount = nThreadCount;
                m_pHandler = pHandler;
                pthread_mutex_init(&m_pmAccept,NULL);
            };
            virtual ~ServerFrame(){pthread_mutex_destroy(&m_pmAccept);};
            virtual bool init();
            virtual bool dispose();
            virtual bool run();

        protected:

            bool BindToLocalHost(SOCKET &sock,u_short nPort);

            static void * ServerThread(void * lpParameter );

        private:
            u_short  m_nLsnPort;
            u_short  m_nThreadCount;
            SOCKET   m_lsnSock;
            IRequestHandler *m_pHandler;
            static bool m_bShutdown;
            static pthread_mutex_t m_pmAccept;
            static const struct timeval m_timev;

    }; 


}
#endif
