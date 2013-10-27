#ifndef HUSKY_THREAD_MANAGER_H
#define HUSKY_THREAD_MANAGER_H
#include <pthread.h>
#include <algorithm>
#include <vector>
#include <map>

#define INFINITE 0

namespace Husky
{
    using namespace std;

    class ThreadManager
    {
        private:
            typedef int HANDLE;
            typedef int DWORD;
            typedef void *(* PThreadFunc)(void* param);
        public:
            ThreadManager(){;}
            ~ThreadManager(){}

            unsigned int HandleCount(){return m_vecHandle.size();}

            void clear()
            {
                m_vecHandle.clear();
            }

            HANDLE CreateThread( PThreadFunc pFunc,void *pPara)
            {	
                pthread_t pt;
                int nErrorCode=pthread_create(&pt,NULL,pFunc,pPara);
                if(nErrorCode!=0)
                  return nErrorCode;
                m_vecHandle.push_back(pt);	//加入线程列表 为WaitForMultipleObjects准备	
                return nErrorCode;

            }

            //hThread (thread handler)        : 为0时为默认最后一个加入管理器的线程句柄 
            //dwMilliseconds等待时间  : 单位毫秒，默认值无穷时间
            //return value            : -1句柄无效，其他值 WaitForSingleObject函数的返回值
            DWORD Wait(HANDLE hThread=0,DWORD dwMilliseconds=INFINITE )
            {
                if( hThread==0)//最后一个加入的线程
                {   
                    if(!m_vecHandle.empty())
                    {
                        return pthread_join(m_vecHandle.back(),NULL);
                    }
                    else
                      return -1;
                }
                else
                {
                    if (find(m_vecHandle.begin(),m_vecHandle.end(),hThread)==m_vecHandle.end())//不存在此句柄
                    {
                        return -1;
                    }

                    return pthread_join(hThread, NULL);
                }

            }


            //等待所有线程执行完毕
            //bWaitAll是否所有线程  : 默认值1等待所有线程,0有任何线程结束，此函数返回
            //dwMilliseconds        : 单位毫秒，默认值无穷时间
            //return value          : -1没有任何句柄，其他值 WaitForMultipleObjects函数的返回值
            DWORD  WaitMultipleThread( bool bWaitAll=1,DWORD dwMilliseconds=INFINITE)
            {
                if (m_vecHandle.empty())		
                  return -1;	
                int nErrorcode;
                for (uint i=0;i<m_vecHandle.size();++i)
                {
                    nErrorcode=pthread_join(m_vecHandle[i], NULL); 
                    if (nErrorcode!=0)
                      return nErrorcode;	
                }	
                return 0;
            }

        private:
            vector<pthread_t> m_vecHandle; 

        private:
            ThreadManager(const ThreadManager&){;}// copy forbidden
            void operator=(const ThreadManager &){}// copy forbidden			
    };
}



#endif
