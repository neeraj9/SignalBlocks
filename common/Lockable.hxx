#ifndef iblocks_common_Lockable_hxx
#define iblocks_common_Lockable_hxx

/** Infrastructure common to VOCAL.<br><br>
 */
namespace BoostPort
{

/**
   @brief Abstract base-class for Mutexes.
*/
class Lockable
{
   protected:
      Lockable() {};
	
   public:
      virtual ~Lockable() {};
      virtual void lock() = 0;
      virtual void unlock() = 0;
      virtual void readlock() { lock(); }
      virtual void writelock() { lock() ; }
};

}

#endif // iblocks_common_Lockable_hxx
