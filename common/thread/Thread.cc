// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "Thread.hh"

using namespace sigblocks;

Thread::Thread()
  : mThread(pthread_self()),
    mJoinable(false)
{
}

Thread::Thread(const functor0<void>& func)
  : mJoinable(true)
{
  int ret = pthread_create(&mThread, 0, &mThreadProxy, &param);
  if (ret != 0)
  {
    throw ThreadResourceError();
  }
}

Thread::~Thread()
{
  pthread_detach(mThread);
}
