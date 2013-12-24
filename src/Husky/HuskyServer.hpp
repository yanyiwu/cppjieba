#ifndef HUSKY_SERVERFRAME_H 
#define HUSKY_SERVERFRAME_H 

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
#include "ThreadManager.hpp"
#include "HttpReqInfo.hpp"

#define INVALID_SOCKET  -1 
#define SOCKET_ERROR    -1 
#define closesocket     close
#define  RECV_BUFFER     10240
#define  LISEN_QUEUR_LEN 1024


namespace Husky
{
    using namespace Limonp;
    typedef	int SOCKET;
    const struct timeval SOCKET_TIMEOUT = {2, 0};
    const char* const RESPONSE_FORMAT = "HTTP/1.1 200 OK\r\nConnection: close\r\nServer: HuskyServer/1.0.0\r\nContent-Type: text/json; charset=%s\r\nContent-Length: %d\r\n\r\n";
    const char* const RESPONSE_CHARSET_UTF8 = "UTF-8";
    const char* const RESPONSE_CHARSET_GB2312 = "GB2312";
    const char* const CLIENT_IP_K = "CLIENT_IP"; 
    typedef unsigned short  u_short;
    typedef unsigned int    u_int;

    class IRequestHandler 
    {
        public:
            virtual ~IRequestHandler(){};
        public:
            virtual bool do_GET(const HttpReqInfo& httpReq, string& res) = 0;

    };

    struct SPara
    {
        SOCKET hSock;
        IRequestHandler * pHandler;
        pthread_mutex_t * ppmAccept;
        bool * pShutdown;
    };

    class HuskyServer
    {
        private:
            pthread_mutex_t m_pmAccept;
            bool m_bShutdown;
        public:
            explicit HuskyServer(unsigned nPort, unsigned nThreadCount, IRequestHandler* pHandler)
            {
                m_bShutdown = false;
                m_nLsnPort = nPort;
                m_nThreadCount = nThreadCount;
                m_pHandler = pHandler;
                assert(pHandler);
                pthread_mutex_init(&m_pmAccept,NULL);
            };
            virtual ~HuskyServer(){pthread_mutex_destroy(&m_pmAccept);};
            virtual bool init()  
            {

                if (!BindToLocalHost(m_lsnSock,m_nLsnPort))
                {
                    LogFatal("BindToLocalHost failed.");
                    return false;
                }
                LogInfo("init ok {port:%d, threadNum:%d}", m_nLsnPort, m_nThreadCount);

                return true;
            }
            virtual bool dispose()    
            {
                m_bShutdown=true;
                if (SOCKET_ERROR==closesocket(m_lsnSock))
                {
                    LogError("error [%s]", strerror(errno));
                    return false;
                }

                int sockfd;
                struct sockaddr_in dest;

                if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                {
                    LogError("error [%s]", strerror(errno));
                    return false;
                }

                bzero(&dest, sizeof(dest));
                dest.sin_family = AF_INET;
                dest.sin_port = htons(m_nLsnPort);
                if (inet_aton("127.0.0.1", (struct in_addr *) &dest.sin_addr.s_addr) == 0)
                {
                    LogError("error [%s]", strerror(errno));
                    return false;
                }

                if (connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) < 0)
                {
                    LogError("error [%s]", strerror(errno));
                }
                close(sockfd);
                return true;
            }
            virtual bool run()
            {
                if(SOCKET_ERROR==listen(m_lsnSock,LISEN_QUEUR_LEN))
                {
                    LogError("error [%s]", strerror(errno));
                    return false;
                }	
                ThreadManager thrMngr;	
                int i;
                SPara para;
                para.hSock=m_lsnSock;
                para.pHandler=m_pHandler;
                para.ppmAccept=&m_pmAccept;
                para.pShutdown=&m_bShutdown;
                for (i=0;i<m_nThreadCount;i++)
                {		
                    if (0!=thrMngr.CreateThread(ServerThread, &para))
                    {
                        break;	
                    }		
                }	
                LogDebug("expect thread count %d, real count %d",m_nThreadCount,i);
                if(i==0)	
                {
                    LogError("error [%s]", strerror(errno));
                    return false;
                }

                LogInfo("server start to run.........");

                if (thrMngr.WaitMultipleThread()!=0)
                {
                    return false;
                }
                return true;

            }

        protected:

            bool BindToLocalHost(SOCKET &sock,u_short nPort)   
            {
                sock=socket(AF_INET,SOCK_STREAM,0);
                if(INVALID_SOCKET==sock)
                {
                    LogError("error [%s]", strerror(errno));
                    return false;
                }

                int nRet = 1;
                if(SOCKET_ERROR==setsockopt(m_lsnSock, SOL_SOCKET, SO_REUSEADDR, (char*)&nRet, sizeof(nRet)))
                {	
                    LogError("error [%s]", strerror(errno));
                }

                struct sockaddr_in addrSock;
                addrSock.sin_family=AF_INET;
                addrSock.sin_port=htons(nPort);
                addrSock.sin_addr.s_addr=htonl(INADDR_ANY);
                int retval;
                retval = ::bind(sock,(sockaddr*)&addrSock,sizeof(sockaddr));
                if(SOCKET_ERROR==retval)
                {
                    LogError("error [%s]", strerror(errno));
                    closesocket(sock);
                    return false;
                }

                return true;

            }

            static void * ServerThread(void * lpParameter )
            {
                SPara *pPara=(SPara*)lpParameter;
                SOCKET hSockLsn=pPara->hSock;
                IRequestHandler *pHandler=pPara->pHandler;
                bool* pShutdown = pPara->pShutdown;
                pthread_mutex_t* ppmAccept = pPara->ppmAccept;

                int nRetCode;
                linger lng;
                char chRecvBuf[RECV_BUFFER];

                SOCKET hClientSock;
                string strHttpResp;

                sockaddr_in clientaddr;
                socklen_t nSize = sizeof(clientaddr);
                while(!(*pShutdown))
                {		
                    HttpReqInfo httpReq;
                    pthread_mutex_lock(ppmAccept);
                    hClientSock=accept(hSockLsn,(sockaddr *)&clientaddr, &nSize);
                    pthread_mutex_unlock(ppmAccept);

                    if(hClientSock==SOCKET_ERROR)
                    {
                        if(!(*pShutdown))
                          LogError("error [%s]", strerror(errno));
                        continue;
                    }

                    httpReq[CLIENT_IP_K] = inet_ntoa(clientaddr.sin_addr);// inet_ntoa is not thread safety at some version 

                    lng.l_linger=1;
                    lng.l_onoff=1;				
                    if(SOCKET_ERROR==setsockopt(hClientSock,SOL_SOCKET,SO_LINGER,(char*)&lng,sizeof(lng)))			
                    {
                        LogError("error [%s]", strerror(errno));
                    }

                    if(SOCKET_ERROR==setsockopt(hClientSock,SOL_SOCKET,SO_RCVTIMEO,(char*)&SOCKET_TIMEOUT,sizeof(SOCKET_TIMEOUT)))
                    {
                        LogError("error [%s]", strerror(errno));
                    }

                    if(SOCKET_ERROR==setsockopt(hClientSock,SOL_SOCKET,SO_SNDTIMEO,(char*)&SOCKET_TIMEOUT,sizeof(SOCKET_TIMEOUT)))
                    {
                        LogError("error [%s]", strerror(errno));
                    }


                    string strRec;
                    string strSnd;
                    memset(chRecvBuf,0,sizeof(chRecvBuf));
                    nRetCode = recv(hClientSock, chRecvBuf, RECV_BUFFER, 0);	
                    strRec = chRecvBuf;

#ifdef HUKSY_DEBUG
                    LogDebug("request[%s]", strRec.c_str());
#endif

                    if(SOCKET_ERROR==nRetCode)
                    {
                        LogDebug("error [%s]", strerror(errno));
                        closesocket(hClientSock);
                        continue;
                    }
                    if(0==nRetCode)
                    {
                        LogDebug("connection has been gracefully closed");
                        closesocket(hClientSock);
                        continue;
                    }
                    httpReq.load(strRec);

                    pHandler->do_GET(httpReq, strSnd);

                    char chHttpHeader[2048];

                    sprintf(chHttpHeader, RESPONSE_FORMAT, RESPONSE_CHARSET_UTF8, int(strSnd.length()));

                    strHttpResp=chHttpHeader;
                    strHttpResp+=strSnd;
#ifdef HUKSY_DEBUG
                    LogDebug("response'body [%s]", strSnd.c_str());
#endif

                    if (SOCKET_ERROR==send(hClientSock,strHttpResp.c_str(),strHttpResp.length(),0))
                    {
                        LogError("error [%s]", strerror(errno));
                    }


                    closesocket(hClientSock);
                }

                return 0;

            }

        private:
            u_short  m_nLsnPort;
            u_short  m_nThreadCount;
            SOCKET   m_lsnSock;
            IRequestHandler *m_pHandler;

    }; 


}
#endif
