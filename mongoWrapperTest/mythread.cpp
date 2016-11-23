/*
   thread.cpp

   Definition of a Java style thread class in C++.

   ------------------------------------------

   Copyright © 2013  [Vic Hargrave - http://vichargrave.com]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#ifndef __thread_h__
#define __thread_h__
#include <pthread.h>

class Mythread
{
    private:
        pthread_t  m_tid;
        int        m_running;
        int        m_detached;

    public:    
        virtual void* run() = 0;
        Mythread() : m_tid(0), m_running(0), m_detached(0) {}

        ~Mythread()
        {
            if (m_running == 1 && m_detached == 0) {
                pthread_detach(m_tid);
            }
            if (m_running == 1) {
                pthread_cancel(m_tid);
            }
        }

        int start()
        {
            int result = pthread_create(&m_tid, NULL, runThread, this);
            if (result == 0) {
                m_running = 1;
            }
            return result;
        }

        int join()
        {
            int result = -1;
            if (m_running == 1) {
                result = pthread_join(m_tid, NULL);
                if (result == 0) {
                    m_detached = 0;
                }
            }
            return result;
        }

        int detach()
        {
            int result = -1;
            if (m_running == 1 && m_detached == 0) {
                result = pthread_detach(m_tid);
                if (result == 0) {
                    m_detached = 1;
                }
            }
            return result;
        }

        pthread_t self() {
            return m_tid;
        }
        static void* runThread(void* arg)
        {
            return ((Mythread*)arg)->run();
        }

};





#endif