#ifndef HUSKY_THREAD_MANAGER_H
#define HUSKY_THREAD_MANAGER_H
#include <pthread.h>
#include <algorithm>
#include <vector>
#include <map>

namespace Husky
{
    using namespace std;

    class ThreadManager
    {
        private:
            typedef int HANDLE;
            typedef void *(* PThreadFunc)(void* param);
        public:
            ThreadManager(){;}
            ~ThreadManager(){}

            size_t HandleCount(){return _handles.size();}

            void clear()
            {
                _handles.clear();
            }

            HANDLE CreateThread( PThreadFunc pFunc,void *pPara)
            {	
                pthread_t pt;
                int nErrorCode = pthread_create(&pt,NULL,pFunc,pPara);
                if(nErrorCode != 0)
                  return nErrorCode;
                _handles.push_back(pt);
                return nErrorCode;

            }

            int Wait(HANDLE hThread = 0)
            {
                if( hThread == 0)//the last handle
                {   
                    if(!_handles.empty())
                    {
                        return pthread_join(_handles.back(),NULL);
                    }
                    else
                      return -1;
                }
                else
                {
                    if (find(_handles.begin(),_handles.end(),hThread) == _handles.end())
                    {
                        return -1;
                    }

                    return pthread_join(hThread, NULL);
                }

            }

            int WaitMultipleThread()
            {
                if (_handles.empty())		
                  return -1;	
                int nErrorcode;
                for (uint i = 0; i < _handles.size(); i++)
                {
                    nErrorcode = pthread_join(_handles[i], NULL); 
                    if (nErrorcode != 0)
                      return nErrorcode;	
                }	
                return 0;
            }

        private:
            vector<pthread_t> _handles; 

        private:
            ThreadManager(const ThreadManager&){;}// copy forbidden
            void operator = (const ThreadManager &){}// copy forbidden			
    };
}



#endif
