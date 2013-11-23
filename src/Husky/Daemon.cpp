#include "Daemon.h"

namespace Husky
{

    IWorkHandler * Daemon::m_pHandler;
    int Daemon::m_nChildPid = 0;
    const char* Daemon::m_pidFile = NULL;

    bool Daemon::isAbnormalExit(int pid, int status)
    {
        bool bRestart = true;
        if (WIFEXITED(status)) //exit()or return 
        {
            LogDebug("child normal termination, exit pid = %d, status = %d", pid, WEXITSTATUS(status));
            bRestart = false;
        }
        else if (WIFSIGNALED(status)) //signal方式退出
        {
            LogError("abnormal termination, pid = %d, signal number = %d%s", pid, WTERMSIG(status),
#ifdef    WCOREDUMP
                        WCOREDUMP(status) ? " (core file generated)" : 
#endif
                        ""); 

            if (WTERMSIG(status) == SIGKILL)
            {
                bRestart = false;
                LogError("has been killed by user , exit pid = %d, status = %d", pid, WEXITSTATUS(status));
            }
        }
        else if (WIFSTOPPED(status)) //暂停的子进程退出
        {
            LogError("child stopped, pid = %d, signal number = %d", pid, WSTOPSIG(status));
        }
        else
        {
            LogError("child other reason quit, pid = %d, signal number = %d", pid, WSTOPSIG(status));
        }
        return bRestart;
    }

    bool Daemon::start()
    {
        string masterPidStr = loadFile2Str(m_pidFile);
        int masterPid = atoi(masterPidStr.c_str());
        if(masterPid)
        {
            if (kill(masterPid, 0) == 0)
            {
                LogError("Another instance exist, ready to quit!");
                return false;
            }
        }

        initAsDaemon();

        char buf[64];
        sprintf(buf, "%d", getpid());
        if (!WriteStr2File(m_pidFile,buf ,"w"))
        {
            LogFatal("Write master pid fail!");
        }

        while(true)
        {
            pid_t pid = fork();
            if (0 == pid)// child process do
            {
                signal(SIGUSR1, sigChildHandler);
                signal(SIGPIPE, SIG_IGN);
                signal(SIGTTOU, SIG_IGN);
                signal(SIGTTIN, SIG_IGN);
                signal(SIGTERM, SIG_IGN);
                signal(SIGINT,  SIG_IGN);
                signal(SIGQUIT, SIG_IGN);

                if(!m_pHandler->init())
                {
                    LogFatal("m_pHandler init failed!");
                    return false;
                }
#ifdef DEBUG
                LogDebug("Worker init  ok pid = %d",(int)getpid());
#endif

                if (!m_pHandler->run())
                {
                    LogError("m_pHandler run finish with failure!");
                    return false;
                }
#ifdef DEBUG
                LogDebug("run finish -ok!");
#endif

                //if(!m_pHandler->dispose())
                //{
                //    LogError("m_pHandler dispose with failure!");
                //    return false;
                //}
#ifdef DEBUG
                //LogDebug("Worker dispose -ok!");
#endif
                exit(0);
            }

            m_nChildPid=pid;
            int status;
            pid = wait(&status);
            if (!isAbnormalExit(pid, status))
            {
                LogDebug("child exit normally! and Daemon exit");
                break;
            }
        }
        return true;
    }


    bool Daemon::stop()
    {
        string masterPidStr = loadFile2Str(m_pidFile);
        int masterPid = atoi(masterPidStr.c_str());
        if(masterPid)
        {
#ifdef DEBUG
            LogDebug("read last masterPid[%d]",masterPid);
#endif
            if (kill(masterPid, 0) == 0)
            {
#ifdef DEBUG
                LogDebug("find previous daemon pid= %d, current pid= %d", masterPid, getpid());
#endif
                kill(masterPid, SIGTERM);

                int tryTime = 200;        
                while (kill(masterPid, 0) == 0 && --tryTime)
                {
                    sleep(1);            
                }

                if (!tryTime && kill(masterPid, 0) == 0)
                {
                    LogError("Time out shutdown fail!");        
                    return false;
                }

                LogInfo("previous daemon pid[%d] shutdown ok.", masterPid);
                return true;
            }

        }
        LogError("Another instance doesn't exist, ready to quit!");
        return false;
    }

    void Daemon::initAsDaemon()
    {
        if (fork() > 0)
          exit(0);
        setsid();

        signal(SIGPIPE, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTERM, sigMasterHandler);
        signal(SIGINT,  sigMasterHandler);
        signal(SIGQUIT, sigMasterHandler);
        signal(SIGKILL, sigMasterHandler);
    }

    void Daemon::sigMasterHandler(int sig)
    {        
        kill(m_nChildPid,SIGUSR1);
        LogDebug("master = %d sig child =%d!",getpid(),m_nChildPid);

    }

    void Daemon::sigChildHandler(int sig)
    {        
        if (sig == SIGUSR1)
        {
            m_pHandler->dispose();
            LogDebug("master = %d signal accept current pid =%d!",getppid(),getpid());
        }

    }

}


