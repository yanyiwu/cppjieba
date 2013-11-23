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

namespace Husky
{
    using namespace Limonp;

    class IWorkHandler
    {
        public:
            virtual ~IWorkHandler(){}
            virtual bool init() = 0;
            virtual bool dispose() = 0;
            virtual bool run() = 0;
    };

    class Daemon
    {
        public:
            Daemon(IWorkHandler * workHandler, const char* pidFile)
            {
                m_pHandler = workHandler;
                m_pidFile = pidFile;
            }
            ~Daemon(){};
        public:
            bool start();
            bool stop();
        public:
            static void initAsDaemon();
            static void sigMasterHandler(int sig);
            static void sigChildHandler(int sig);
            static bool isAbnormalExit(int pid, int status);
        private:
            //static IRequestHandler* m_pHandler;
            //static ServerFrame m_ServerFrame;
            static int m_nChildPid;
            static IWorkHandler * m_pHandler;
            static const char* m_pidFile;
    };
}
#endif
