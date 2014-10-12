// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_common_thread_Thread_hh
#define iblocks_common_thread_Thread_hh

#include <pthread.h>

namespace iblocks
{
  class Thread
  {
  public:
    Thread();
    explicit Thread(const functor0<void>& func);

    void Join();
    void Yield();

  private: // no copy allowed
    Thread(const Thread&);
    Thread& operator=(const Thread&);

  private:
    pthread_t mThread;
    const bool mJoinable;
  };
}
#endif // iblocks_common_thread_Thread_hh
