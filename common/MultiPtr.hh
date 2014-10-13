// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
// copyright (c) 2010-12 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE.txt for license information
#ifndef sigblocks_common_MultiPtr_hxx
#define sigblocks_common_MultiPtr_hxx

namespace sigblocks
{
  /**
   *  A wrapper class to provide MultiPtr with reference semantics.
   *  For example, an MultiPtr can be assigned (or constructed from)
   *  the result of a function which returns an MultiPtr by value.
   *
   *  All the MultiPtr_ref stuff should happen behind the scenes.
   */
  template<typename _Tp1>
    struct MultiPtr_ref
    {
      _Tp1* _M_ptr;
      
      explicit
      MultiPtr_ref(_Tp1* __p): _M_ptr(__p) { }
    };


  /**
   *  @brief  A simple smart pointer providing strict ownership semantics.
   *
   *  The Standard says:
   *  <pre>
   *  An @c MultiPtr owns the object it holds a pointer to.  Copying
   *  an @c MultiPtr copies the pointer and transfers ownership to the
   *  destination.  If more than one @c MultiPtr owns the same object
   *  at the same time the behavior of the program is undefined.
   *
   *  The uses of @c MultiPtr include providing temporary
   *  exception-safety for dynamically allocated memory, passing
   *  ownership of dynamically allocated memory to a function, and
   *  returning dynamically allocated memory from a function.  @c
   *  MultiPtr does not meet the CopyConstructible and Assignable
   *  requirements for Standard Library <a
   *  href="tables.html#65">container</a> elements and thus
   *  instantiating a Standard Library container with an @c MultiPtr
   *  results in undefined behavior.
   *  </pre>
   *  Quoted from [20.4.5]/3.
   *
   *  Good examples of what can and cannot be done with MultiPtr can
   *  be found in the libstdc++ testsuite.
   *
   *  @if maint
   *  _GLIBCXX_RESOLVE_LIB_DEFECTS
   *  127.  MultiPtr<> conversion issues
   *  These resolutions have all been incorporated.
   *  @endif
   */
  template<typename _Tp>
    class MultiPtr
    {
    private:
      _Tp* _M_ptr;
      
    public:
      /// The pointed-to type.
      typedef _Tp element_type;
      
      /**
       *  @brief  An %MultiPtr is usually constructed from a raw pointer.
       *  @param  p  A pointer (defaults to NULL).
       *
       *  This object now @e owns the object pointed to by @a p.
       */
      explicit
      MultiPtr(element_type* __p = 0) throw() : _M_ptr(__p) { }

      /**
       *  @brief  An %MultiPtr can be constructed from another %MultiPtr.
       *  @param  a  Another %MultiPtr of the same type.
       *
       *  This object now @e owns the object previously owned by @a a,
       *  which has given up ownsership.
       */
      MultiPtr(MultiPtr& __a) throw() : _M_ptr(__a.release()) { }

      /**
       *  @brief  An %MultiPtr can be constructed from another %MultiPtr.
       *  @param  a  Another %MultiPtr of a different but related type.
       *
       *  A pointer-to-Tp1 must be convertible to a
       *  pointer-to-Tp/element_type.
       *
       *  This object now @e owns the object previously owned by @a a,
       *  which has given up ownsership.
       */
      template<typename _Tp1>
        MultiPtr(MultiPtr<_Tp1>& __a) throw() : _M_ptr(__a.release()) { }

      /**
       *  @brief  %MultiPtr assignment operator.
       *  @param  a  Another %MultiPtr of the same type.
       *
       *  This object now @e owns the object previously owned by @a a,
       *  which has given up ownsership.  The object that this one @e
       *  used to own and track has been deleted.
       */
      MultiPtr&
      operator=(MultiPtr& __a) throw()
      {
	reset(__a.release());
	return *this;
      }

      /**
       *  @brief  %MultiPtr assignment operator.
       *  @param  a  Another %MultiPtr of a different but related type.
       *
       *  A pointer-to-Tp1 must be convertible to a pointer-to-Tp/element_type.
       *
       *  This object now @e owns the object previously owned by @a a,
       *  which has given up ownsership.  The object that this one @e
       *  used to own and track has been deleted.
       */
      template<typename _Tp1>
        MultiPtr&
        operator=(MultiPtr<_Tp1>& __a) throw()
        {
	  reset(__a.release());
	  return *this;
	}

      /**
       *  When the %MultiPtr goes out of scope, the object it owns is
       *  deleted.  If it no longer owns anything (i.e., @c get() is
       *  @c NULL), then this has no effect.
       *
       *  @if maint
       *  The C++ standard says there is supposed to be an empty throw
       *  specification here, but omitting it is standard conforming.  Its
       *  presence can be detected only if _Tp::~_Tp() throws, but this is
       *  prohibited.  [17.4.3.6]/2
       *  @endif
       */
      ~MultiPtr() { delete [] _M_ptr; }
      
      /**
       *  @brief  Smart pointer dereferencing.
       *
       *  If this %MultiPtr no longer owns anything, then this
       *  operation will crash.  (For a smart pointer, "no longer owns
       *  anything" is the same as being a null pointer, and you know
       *  what happens when you dereference one of those...)
       */
      element_type&
      operator*() const throw() 
      {
	_GLIBCXX_DEBUG_ASSERT(_M_ptr != 0);
	return *_M_ptr; 
      }
      
      /**
       *  @brief  Smart pointer dereferencing.
       *
       *  This returns the pointer itself, which the language then will
       *  automatically cause to be dereferenced.
       */
      element_type*
      operator->() const throw() 
      {
	_GLIBCXX_DEBUG_ASSERT(_M_ptr != 0);
	return _M_ptr; 
      }
      
      /**
       *  @brief  Bypassing the smart pointer.
       *  @return  The raw pointer being managed.
       *
       *  You can get a copy of the pointer that this object owns, for
       *  situations such as passing to a function which only accepts
       *  a raw pointer.
       *
       *  @note  This %MultiPtr still owns the memory.
       */
      element_type*
      get() const throw() { return _M_ptr; }
      
      /**
       *  @brief  Bypassing the smart pointer.
       *  @return  The raw pointer being managed.
       *
       *  You can get a copy of the pointer that this object owns, for
       *  situations such as passing to a function which only accepts
       *  a raw pointer.
       *
       *  @note  This %MultiPtr no longer owns the memory.  When this object
       *  goes out of scope, nothing will happen.
       */
      element_type*
      release() throw()
      {
	element_type* __tmp = _M_ptr;
	_M_ptr = 0;
	return __tmp;
      }
      
      /**
       *  @brief  Forcibly deletes the managed object.
       *  @param  p  A pointer (defaults to NULL).
       *
       *  This object now @e owns the object pointed to by @a p.  The
       *  previous object has been deleted.
       */
      void
      reset(element_type* __p = 0) throw()
      {
	if (__p != _M_ptr)
	  {
	    delete []_M_ptr;
	    _M_ptr = __p;
	  }
      }
      
      /** 
       *  @brief  Automatic conversions
       *
       *  These operations convert an %MultiPtr into and from an MultiPtr_ref
       *  automatically as needed.  This allows constructs such as
       *  @code
       *    MultiPtr<Derived>  func_returning_MultiPtr(.....);
       *    ...
       *    MultiPtr<Base> ptr = func_returning_MultiPtr(.....);
       *  @endcode
       */
      MultiPtr(MultiPtr_ref<element_type> __ref) throw()
      : _M_ptr(__ref._M_ptr) { }
      
      MultiPtr&
      operator=(MultiPtr_ref<element_type> __ref) throw()
      {
	if (__ref._M_ptr != this->get())
	  {
	    delete [] _M_ptr;
	    _M_ptr = __ref._M_ptr;
	  }
	return *this;
      }
      
      template<typename _Tp1>
        operator MultiPtr_ref<_Tp1>() throw()
        { return MultiPtr_ref<_Tp1>(this->release()); }

      template<typename _Tp1>
        operator MultiPtr<_Tp1>() throw()
        { return MultiPtr<_Tp1>(this->release()); }
  };
}

#endif // sigblocks_common_MultiPtr_hxx
