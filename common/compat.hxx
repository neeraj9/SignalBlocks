#ifndef sigblocks_common_compat_hxx
#define sigblocks_common_compat_hxx

/**
   @file
   This file is used to handle compatibility fixes/tweaks so we can 
   function on multiple platforms.
*/

#if defined(__INTEL_COMPILER ) && defined( __OPTIMIZE__ )
#  undef __OPTIMIZE__ // wierd intel bug with ntohs and htons macros
#endif

//#if defined(HAVE_SYS_INT_TYPES_H)
//#include <sys/int_types.h>
//#endif

#include <cstring>

#ifndef WIN32
#  include <netdb.h>
#  include <sys/types.h>
#  include <sys/time.h>
#  include <sys/socket.h>
#  include <sys/select.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <unistd.h>
#  include <pthread.h>
#  include <limits.h>
#endif

#ifdef WIN32
// !cj! TODO would be nice to remove this 
#  ifndef __GNUC__
#    pragma warning(disable : 4996)
#  endif
#define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  include <winsock2.h>
#undef WIN32_LEAN_AND_MEAN
#  include <errno.h>
#  include <io.h>
#ifdef UNDER_CE
#include "wince/WceCompat.hxx"
#endif // UNDER_CE
#endif

#if defined(__APPLE__) 
#  if !defined(MAC_OS_X_VERSION_MIN_REQUIRED) || MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_2
      // you don't include the SDK or you're running 10.3 or above
      // note: this will fail on 10.2 if you don't include the SDK
#     include <arpa/nameser_compat.h>
#  else
      // you include the SDK and you're running Mac OS 10.2 or below
      typedef int socklen_t;
#  endif
#  ifdef __MWERKS__ /* this is a <limits.h> bug filed with Apple, Radar# 3657629. */
#    ifndef __SCHAR_MAX__ 
#      define __SCHAR_MAX__ 127
#    endif
#  endif
#endif

#if defined(__SUNPRO_CC)
#  if defined(_TIME_T)
    using std::time_t;
#  endif
#  include <time.h>
#  include <memory.h>
#  include <string.h>
#endif

#if !defined(T_NAPTR)
#  define T_NAPTR 35
#endif

#if !defined(T_SRV)
#  define T_SRV 33
#endif

#if !defined(T_AAAA)
#  define T_AAAA 28
#endif

#if !defined(T_A)
#  define T_A 1
#endif

namespace BoostPort
{

#if defined(WIN32) || defined(__QNX__)
#ifndef strcasecmp
#  define strcasecmp(a,b)    stricmp(a,b)
#endif
#ifndef strncasecmp
#  define strncasecmp(a,b,c) strnicmp(a,b,c)
#endif
#endif

#if defined(__QNX__) || defined(__sun) || defined(WIN32)
  typedef unsigned int u_int32_t;
#endif

template<typename _Tp>
inline const _Tp&
BoostPortMin(const _Tp& __a, const _Tp& __b)
{
   if (__b < __a) return __b; return __a;
}

template<typename _Tp>
inline const _Tp&
BoostPortMax(const _Tp& __a, const _Tp& __b) 
{
   if (__a < __b) return __b; return __a;
}

}

// Mac OS X: UInt32 definition conflicts with the Mac OS SDK.
// If you've included the SDK then TARGET_OS_MAC will be defined.
#ifndef TARGET_OS_MAC
typedef unsigned char  UInt8;
typedef unsigned short UInt16;
typedef unsigned int   UInt32;
#endif

#if defined( WIN32 )
  typedef unsigned __int64 UInt64;
#else
  typedef unsigned long long UInt64;
#endif
//typedef struct { unsigned char octet[16]; }  UInt128;

//template "levels; ie REASONABLE and COMPLETE
//reasonable allows most things such as partial template specialization,
//etc...like most compilers and VC++2003+.
//COMPLETE would allow template metaprogramming, template< template< > > tricks,
//etc...REASONABLE should always be defined when COMPLETE is defined.

//#if !defined(__SUNPRO_CC) && !defined(__INTEL_COMPILER)
#if !defined(__INTEL_COMPILER)
#  define REASONABLE_TEMPLATES
#endif

// .bwc. This is the only place we check for USE_IPV6 in a header file. This 
// code has no effect if USE_IPV6 is not set, so this should only kick in when
// we're building the BoostPort libs. If someone sets USE_IPV6 while building
// against the BoostPort libs, no BoostPort header file will care, but if the platform
// doesn't support IPv6, we will undef USE_IPV6.
#ifdef USE_IPV6
#ifndef IPPROTO_IPV6
#ifdef _MSC_VER
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)"): "
#pragma message (__LOC__ " IPv6 support requested, but IPPROTO_V6 undefined; this platform does not appear to support IPv6 ")
#else
#warning IPv6 support requested, but IPPROTO_IPV6 undefined; this platform does not appear to support IPv6
#endif
#undef USE_IPV6
#endif
#endif

#endif // sigblocks_common_compat_hxx
