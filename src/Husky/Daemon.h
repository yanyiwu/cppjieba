#ifndef HUSKY_DAEMON_H_
#define HUSKY_DAEMON_H_
#include "globals.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include "../Limonp/logger.hpp"
#include "ServerFrame.h"

namespace Husky
{
    using namespace Limonp;

    class Daemon
    {
        public:
            Daemon(IRequestHandler * pHandler)
            {
                m_pHandler = pHandler;
            }
            ~Daemon(){};
        public:
            bool Start(unsigned int port, unsigned int threadNum);
            bool Stop();
        public:
            static void initAsDaemon();
            static void sigMasterHandler(int sig);
            static void sigChildHandler(int sig);
            static bool isAbnormalExit(int pid, int status);
        private:
            static    IRequestHandler* m_pHandler;
            static    ServerFrame m_ServerFrame;
            static    int m_nChildPid;
    };
}
#endif
