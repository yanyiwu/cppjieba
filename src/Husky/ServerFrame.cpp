#include "ServerFrame.h"
namespace Husky
{
    const struct timeval ServerFrame::m_timev = {SOCKET_TIMEOUT, 0};

    pthread_mutex_t ServerFrame::m_pmAccept; 
    bool ServerFrame::m_bShutdown = false;

    bool ServerFrame::CloseServer()
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
        LogInfo("CloseServer ok.");
        return true;

    }

    bool ServerFrame::RunServer()
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

    void* ServerFrame::ServerThread(void *lpParameter )
    {
        SPara *pPara=(SPara*)lpParameter;
        SOCKET hSockLsn=pPara->hSock;
        IRequestHandler *pHandler=pPara->pHandler;
        int nRetCode;
        linger lng;
        char chRecvBuf[RECV_BUFFER];

        SOCKET hClientSock;
        string strHttpResp;

        sockaddr_in clientaddr;
        socklen_t nSize = sizeof(clientaddr);
        while(!m_bShutdown)
        {		
            HttpReqInfo httpReq;
            pthread_mutex_lock(&m_pmAccept);
            hClientSock=accept(hSockLsn,(sockaddr *)&clientaddr, &nSize);
            pthread_mutex_unlock(&m_pmAccept);

            if(hClientSock==SOCKET_ERROR)
            {
                if(!m_bShutdown)
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

            if(SOCKET_ERROR==setsockopt(hClientSock,SOL_SOCKET,SO_RCVTIMEO,(char*)&m_timev,sizeof(m_timev)))
            {
                LogError("error [%s]", strerror(errno));
            }

            if(SOCKET_ERROR==setsockopt(hClientSock,SOL_SOCKET,SO_SNDTIMEO,(char*)&m_timev,sizeof(m_timev)))
            {
                LogError("error [%s]", strerror(errno));
            }


            string strRec;
            string strSnd;
            memset(chRecvBuf,0,sizeof(chRecvBuf));
            nRetCode = recv(hClientSock, chRecvBuf, RECV_BUFFER, 0);	
            strRec = chRecvBuf;

#ifdef DEBUG
            LogDebug("response[%s]", strRec.c_str());
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

            sprintf(chHttpHeader, RESPONSE_FORMAT, RESPONSE_CHARSET_UTF8, strSnd.length());

            strHttpResp=chHttpHeader;
            strHttpResp+=strSnd;

            if (SOCKET_ERROR==send(hClientSock,strHttpResp.c_str(),strHttpResp.length(),0))
            {
                LogError("error [%s]", strerror(errno));
            }

#ifdef DEBUG
            LogDebug("send response [%s] ", strHttpResp.c_str());
#endif

            closesocket(hClientSock);
        }

        return 0;

    }

    bool ServerFrame::CreateServer(u_short nPort,u_short nThreadCount,IRequestHandler *pHandler)
    {
        m_nLsnPort=nPort;
        m_nThreadCount=nThreadCount;
        m_pHandler=pHandler;

        if (!BindToLocalHost(m_lsnSock,m_nLsnPort))
        {
            return false;
        }
        pthread_mutex_init(&m_pmAccept,NULL);
        LogInfo("CreatServer ok {port:%d, threadNum:%d}", nPort, nThreadCount);
        return true;
    }

    bool  ServerFrame::BindToLocalHost(SOCKET &sock,u_short nPort)
    {
        sock=socket(AF_INET,SOCK_STREAM,0);
        if(INVALID_SOCKET==sock)
        {
            LogError("error [%s]", strerror(errno));
            return false;
        }

        /* 使地址马上可以重用 */
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
        retval=bind(sock,(sockaddr*)&addrSock,sizeof(sockaddr));
        if(SOCKET_ERROR==retval)
        {
            LogError("error [%s]", strerror(errno));
            closesocket(sock);
            return false;
        }

        return true;

    }
}
