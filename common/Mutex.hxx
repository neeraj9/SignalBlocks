#ifndef sigblocks_common_Mutex_hxx
#define sigblocks_common_Mutex_hxx

#include "compat.hxx"
#include "Lockable.hxx"

namespace BoostPort
{
class Condition;

/**
   @brief A semaphore that can be locked by only one thread at a time.  

   Used to serialize access to some resource (such as a data member).  Here's
   an example:

   @code
     void
     Log::setLevel(Level level)
     {
        Lock lock(_mutex);
        _level = level; 
     }
   @endcode

   As indicated in the example, you will probably never use any member function 
   in this class.  The 'Lock' class provides this functionality.

   @see Lock
   @see Condition

*/
class Mutex : public Lockable
{
      friend class Condition;

   public:
      Mutex();
      virtual ~Mutex();
      virtual void lock();
      virtual void unlock();

   private:
      // !kh!
      //  no value sematics, therefore private and not implemented.
      Mutex (const Mutex&);
      Mutex& operator= (const Mutex&);

   private:
#ifdef WIN32
	  CRITICAL_SECTION mId;
#else
      mutable  pthread_mutex_t mId;
      pthread_mutex_t* getId() const;
#endif
};

}

#endif // sigblocks_common_Mutex_hxx
