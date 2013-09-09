/*
 * kmp_os.h -- KPTS runtime header file.
 * $Revision: 42489 $
 * $Date: 2013-07-08 11:00:09 -0500 (Mon, 08 Jul 2013) $
 */

/* <copyright>
    Copyright (c) 1997-2013 Intel Corporation.  All Rights Reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

      * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
      * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
      * Neither the name of Intel Corporation nor the names of its
        contributors may be used to endorse or promote products derived
        from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

</copyright> */

#ifndef KMP_OS_H
#define KMP_OS_H

#include <stdlib.h>

#define KMP_FTN_PLAIN   1
#define KMP_FTN_APPEND  2
#define KMP_FTN_UPPER   3
/*
#define KMP_FTN_PREPEND 4
#define KMP_FTN_UAPPEND 5
*/

#define KMP_PTR_SKIP    (sizeof(void*))

/* -------------------------- Compiler variations ------------------------ */

#define KMP_OFF				0
#define KMP_ON				1

#define KMP_MEM_CONS_VOLATILE		0
#define KMP_MEM_CONS_FENCE		1

#ifndef KMP_MEM_CONS_MODEL
# define KMP_MEM_CONS_MODEL	 KMP_MEM_CONS_VOLATILE
#endif

/* ---------------------- Operating system recognition ------------------- */

#define KMP_OS_LINUX    0
#define KMP_OS_DARWIN   0
#define KMP_OS_WINDOWS    0
#define KMP_OS_CNK      0
#define KMP_OS_UNIX     0  /* disjunction of KMP_OS_LINUX with KMP_OS_DARWIN */

#define KMP_ARCH_X86        0
#define KMP_ARCH_X86_64	    0
#define KMP_ARCH_PPC64      0

#ifdef _WIN32
# undef KMP_OS_WINDOWS
# define KMP_OS_WINDOWS 1
#endif

#if ( defined __APPLE__ && defined __MACH__ )
# undef KMP_OS_DARWIN
# define KMP_OS_DARWIN 1
#endif

#if ( defined __linux )
# undef KMP_OS_LINUX
# define KMP_OS_LINUX 1
#endif

#if ( defined __bgq__ )
# undef KMP_OS_CNK
# define KMP_OS_CNK 1
#endif

#if (1 != KMP_OS_LINUX + KMP_OS_DARWIN + KMP_OS_WINDOWS)
# error Unknown OS
#endif

#if KMP_OS_LINUX || KMP_OS_DARWIN
# undef KMP_OS_UNIX
# define KMP_OS_UNIX 1
#endif

#if KMP_OS_WINDOWS
# if defined _M_AMD64
#  undef KMP_ARCH_X86_64
#  define KMP_ARCH_X86_64 1
# else
#  undef KMP_ARCH_X86
#   define KMP_ARCH_X86 1
# endif
#endif

#if KMP_OS_UNIX
# if defined __x86_64
#  undef KMP_ARCH_X86_64
#  define KMP_ARCH_X86_64 1
# elif defined __powerpc64__
#  undef KMP_ARCH_PPC64
#  define KMP_ARCH_PPC64 1
# else
#  undef KMP_ARCH_X86
#  define KMP_ARCH_X86 1
# endif
#endif

#if (1 != KMP_ARCH_X86 + KMP_ARCH_X86_64 + KMP_ARCH_PPC64)
# error Unknown or unsupported architecture
#endif

#if KMP_OS_WINDOWS
# if defined  KMP_WIN_CDECL ||  !defined GUIDEDLL_EXPORTS
#   define USE_FTN_CDECL      KMP_FTN_UPPER
# endif

# define KMP_FTN            KMP_FTN_PLAIN
# define USE_FTN_EXTRA      KMP_FTN_PLAIN
# if KMP_ARCH_X86
#  if defined KMP_WIN_STDCALL || !defined GUIDEDLL_EXPORTS
#   define USE_FTN_STDCALL   KMP_FTN_UPPER
#  endif
# endif
  typedef char              kmp_int8;
  typedef unsigned char     kmp_uint8;
  typedef short             kmp_int16;
  typedef unsigned short    kmp_uint16;
  typedef int               kmp_int32;
  typedef unsigned int      kmp_uint32;
# define KMP_INT32_SPEC     "d"
# define KMP_UINT32_SPEC    "u"
# ifndef KMP_STRUCT64
   typedef __int64 		kmp_int64;
   typedef unsigned __int64 	kmp_uint64;
   #define KMP_INT64_SPEC 	"I64d"
   #define KMP_UINT64_SPEC	"I64u"
# else
   struct kmp_struct64 {
    kmp_int32   a,b;
   };
   typedef struct kmp_struct64 kmp_int64;
   typedef struct kmp_struct64 kmp_uint64;
   /* Not sure what to use for KMP_[U]INT64_SPEC here */
# endif
# if KMP_ARCH_X86_64
#  define KMP_INTPTR 1
   typedef __int64         	kmp_intptr_t;
   typedef unsigned __int64	kmp_uintptr_t;
#  define KMP_INTPTR_SPEC  	"I64d"
#  define KMP_UINTPTR_SPEC 	"I64u"
# endif
#endif /* KMP_OS_WINDOWS */

#if KMP_OS_UNIX
# define KMP_FTN        KMP_FTN_PLAIN
# define USE_FTN_CDECL  KMP_FTN_PLAIN
# define USE_FTN_EXTRA  KMP_FTN_APPEND
  typedef char               kmp_int8;
  typedef unsigned char      kmp_uint8;
  typedef short              kmp_int16;
  typedef unsigned short     kmp_uint16;
  typedef int                kmp_int32;
  typedef unsigned int       kmp_uint32;
  typedef long long          kmp_int64;
  typedef unsigned long long kmp_uint64;
# define KMP_INT32_SPEC      "d"
# define KMP_UINT32_SPEC     "u"
# define KMP_INT64_SPEC	     "lld"
# define KMP_UINT64_SPEC     "llu"
#endif /* KMP_OS_UNIX */

#if KMP_ARCH_X86
# define KMP_SIZE_T_SPEC KMP_UINT32_SPEC
#elif KMP_ARCH_X86_64 || KMP_ARCH_PPC64
# define KMP_SIZE_T_SPEC KMP_UINT64_SPEC
#else
# error "Can't determine size_t printf format specifier."
#endif

#if KMP_ARCH_X86
# define KMP_SIZE_T_MAX (0xFFFFFFFF)
#else
# define KMP_SIZE_T_MAX (0xFFFFFFFFFFFFFFFF)
#endif

typedef size_t  kmp_size_t;
typedef float   kmp_real32;
typedef double  kmp_real64;

#ifndef KMP_INTPTR
# define KMP_INTPTR 1
  typedef long             kmp_intptr_t;
  typedef unsigned long    kmp_uintptr_t;
# define KMP_INTPTR_SPEC   "ld"
# define KMP_UINTPTR_SPEC  "lu"
#endif

#ifdef KMP_I8
  typedef kmp_int64      kmp_int;
  typedef kmp_uint64     kmp_uint;
# define  KMP_INT_SPEC	 KMP_INT64_SPEC
# define  KMP_UINT_SPEC	 KMP_UINT64_SPEC
# define  KMP_INT_MAX    ((kmp_int64)0x7FFFFFFFFFFFFFFFLL)
# define  KMP_INT_MIN    ((kmp_int64)0x8000000000000000LL)
#else
  typedef kmp_int32      kmp_int;
  typedef kmp_uint32     kmp_uint;
# define  KMP_INT_SPEC	 KMP_INT32_SPEC
# define  KMP_UINT_SPEC	 KMP_UINT32_SPEC
# define  KMP_INT_MAX    ((kmp_int32)0x7FFFFFFF)
# define  KMP_INT_MIN    ((kmp_int64)0x80000000)
#endif /* KMP_I8 */

#ifdef __cplusplus
    //-------------------------------------------------------------------------
    // template for debug prints specification ( d, u, lld, llu ), and to obtain
    // signed/unsigned flavors of a type
    template< typename T >
    struct traits_t {
        typedef T           signed_t;
        typedef T           unsigned_t;
        typedef T           floating_t;
        static char const * spec;
    };
    // int
    template<>
    struct traits_t< signed int > {
        typedef signed int    signed_t;
        typedef unsigned int  unsigned_t;
        typedef double        floating_t;
        static char const *   spec;
    };
    // unsigned int
    template<>
    struct traits_t< unsigned int > {
        typedef signed int    signed_t;
        typedef unsigned int  unsigned_t;
        typedef double        floating_t;
        static char const *   spec;
    };
    // long long
    template<>
    struct traits_t< signed long long > {
        typedef signed long long    signed_t;
        typedef unsigned long long  unsigned_t;
        typedef long double         floating_t;
        static char const *         spec;
    };
    // unsigned long long
    template<>
    struct traits_t< unsigned long long > {
        typedef signed long long    signed_t;
        typedef unsigned long long  unsigned_t;
        typedef long double         floating_t;
        static char const *         spec;
    };
    //-------------------------------------------------------------------------
#endif // __cplusplus

#if KMP_OS_WINDOWS
# define KMP_STDCALL      __stdcall
#endif

#ifndef KMP_STDCALL
# define KMP_STDCALL    /* nothing */
#endif

#define KMP_EXPORT	extern	/* export declaration in guide libraries */

#if __GNUC__ == 4
    #define __forceinline __inline
#endif

#define PAGE_SIZE                       (0x4000)
#define PAGE_ALIGNED(_addr)     ( ! ((size_t) _addr & \
                                     (size_t)(PAGE_SIZE - 1)))
#define ALIGN_TO_PAGE(x)   (void *)(((size_t)(x)) & ~((size_t)(PAGE_SIZE - 1)))

/* ---------------------- Support for cache alignment, padding, etc. -----------------*/

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/* Define the default size of the cache line */
#ifndef CACHE_LINE
    #define CACHE_LINE                  128         /* cache line size in bytes */
#else
    #if ( CACHE_LINE < 64 ) && ! defined( KMP_OS_DARWIN )
        // 2006-02-13: This produces too many warnings on OS X*. Disable it for a while...
        #warning CACHE_LINE is too small.
    #endif
#endif /* CACHE_LINE */

/* SGI's cache padding improvements using align decl specs (Ver 19) */
#if !defined KMP_PERF_V19
# define KMP_PERF_V19			KMP_ON
#endif

/* SGI's improvements for inline argv (Ver 106) */
#if !defined KMP_PERF_V106
# define KMP_PERF_V106			KMP_ON
#endif

#define KMP_CACHE_PREFETCH(ADDR) 	/* nothing */

/* Temporary note: if performance testing of this passes, we can remove
   all references to KMP_DO_ALIGN and replace with KMP_ALIGN.  */
#if KMP_OS_UNIX && defined(__GNUC__)
# define KMP_DO_ALIGN(bytes)  __attribute__((aligned(bytes)))
# define KMP_ALIGN_CACHE      __attribute__((aligned(CACHE_LINE)))
# define KMP_ALIGN(bytes)     __attribute__((aligned(bytes)))
#else
# define KMP_DO_ALIGN(bytes)  __declspec( align(bytes) )
# define KMP_ALIGN_CACHE      __declspec( align(CACHE_LINE) )
# define KMP_ALIGN(bytes)     __declspec( align(bytes) )
#endif

#if defined(__MIC__) || defined(__MIC2__)
    #define KMP_MIC  1
// Intel(R) Composer XE (13.0) defines both __MIC__ and __MIC2__ !
# if __MIC2__ || __KNC__
    #define KMP_MIC1 0
    #define KMP_MIC2 1
# else
    #define KMP_MIC1 1
    #define KMP_MIC2 0
# endif
#else
    #define KMP_MIC  0
    #define KMP_MIC1 0
    #define KMP_MIC2 0
#endif

/* General purpose fence types for memory operations */
enum kmp_mem_fence_type {
    kmp_no_fence,         /* No memory fence */
    kmp_acquire_fence,    /* Acquire (read) memory fence */
    kmp_release_fence,    /* Release (write) memory fence */
    kmp_full_fence        /* Full (read+write) memory fence */
};


#if KMP_MIC || KMP_ARCH_PPC64
/* cast ADDR to correct type so that proper intrinsic will be used */
# define KMP_TEST_THEN_INC32( ADDR )            __sync_fetch_and_add( (kmp_int32*)(ADDR), 1 )
//__atomic_fetch_add_explicit_4((ADDR),1,0)
# define KMP_TEST_THEN_INC_ACQ32( ADDR )	__sync_fetch_and_add( (kmp_int32*)(ADDR), 1 )
//__atomic_fetch_add_explicit_4((ADDR),1,0)
# define KMP_TEST_THEN_INC64( ADDR )		__sync_fetch_and_add( (kmp_int64*)(ADDR), 1LL )
//__atomic_fetch_add_explicit_8((ADDR),1LL,0)
# define KMP_TEST_THEN_INC_ACQ64( ADDR )	__sync_fetch_and_add( (kmp_int64*)(ADDR), 1LL )
//__atomic_fetch_add_explicit_8((ADDR),1LL,0)
# define KMP_TEST_THEN_ADD4_32( ADDR ) 		__sync_fetch_and_add( (kmp_int32*)(ADDR), 4 )
//__atomic_fetch_add_explicit_4((ADDR),4,0)
# define KMP_TEST_THEN_ADD4_ACQ32( ADDR ) 	__sync_fetch_and_add( (kmp_int32*)(ADDR), 4 )
//__atomic_fetch_add_explicit_4((ADDR),4,0)
# define KMP_TEST_THEN_ADD4_64( ADDR ) 		__sync_fetch_and_add( (kmp_int64*)(ADDR), 4LL )
//__atomic_fetch_add_explicit_8((ADDR),4LL,0)
# define KMP_TEST_THEN_ADD4_ACQ64( ADDR ) 	__sync_fetch_and_add( (kmp_int64*)(ADDR), 4LL )
//__atomic_fetch_add_explicit_8((ADDR),4LL,0)
# define KMP_TEST_THEN_DEC32( ADDR )		__sync_fetch_and_sub( (kmp_int32*)(ADDR), 1 )
//__atomic_fetch_sub_explicit_4((ADDR),1,0)
# define KMP_TEST_THEN_DEC_ACQ32( ADDR )	__sync_fetch_and_sub( (kmp_int32*)(ADDR), 1 )
//__atomic_fetch_sub_explicit_4((ADDR),1,0)
# define KMP_TEST_THEN_DEC64( ADDR )		__sync_fetch_and_sub( (kmp_int64*)(ADDR), 1LL )
//__atomic_fetch_sub_explicit_8((ADDR),1LL,0)
# define KMP_TEST_THEN_DEC_ACQ64( ADDR )	__sync_fetch_and_sub( (kmp_int64*)(ADDR), 1LL )
//__atomic_fetch_sub_explicit_8((ADDR),1LL,0)
# define KMP_TEST_THEN_ADD32( ADDR, INCR )	__sync_fetch_and_add( (kmp_int32*)(ADDR), (INCR) )
//__atomic_fetch_add_explicit_4((ADDR),(INCR),0)
# define KMP_TEST_THEN_ADD64( ADDR, INCR )	__sync_fetch_and_add( (kmp_int64*)(ADDR), (INCR) )
//__atomic_fetch_add_explicit_8((ADDR),(INCR),0)
# define KMP_COMPARE_AND_STORE_ACQ32(p,cv,sv)   __sync_bool_compare_and_swap((volatile kmp_uint32*)(p),(kmp_uint32)(cv),(kmp_uint32)(sv))
# define KMP_COMPARE_AND_STORE_REL32(p,cv,sv)   __sync_bool_compare_and_swap((volatile kmp_uint32*)(p),(kmp_uint32)(cv),(kmp_uint32)(sv))
# define KMP_COMPARE_AND_STORE_ACQ64(p,cv,sv)   __sync_bool_compare_and_swap((volatile kmp_uint64*)(p),(kmp_uint64)(cv),(kmp_uint64)(sv))
# define KMP_COMPARE_AND_STORE_REL64(p,cv,sv)   __sync_bool_compare_and_swap((volatile kmp_uint64*)(p),(kmp_uint64)(cv),(kmp_uint64)(sv))
# define KMP_COMPARE_AND_STORE_PTR(p,cv,sv)     __sync_bool_compare_and_swap((volatile kmp_uint64*)(p),(kmp_uint64)(cv),(kmp_uint64)(sv))
// cannot use low-level CAS intrinsic because it has two pointer parameters (we often use 0 or 1)
#if KMP_ARCH_PPC64
# define KMP_COMPARE_AND_STORE_ACQ16(p,cv,sv)   __sync_bool_compare_and_swap((volatile kmp_uint16*)(p),(kmp_uint16)(cv),(kmp_uint16)(sv))
# define KMP_COMPARE_AND_STORE_REL16(p,cv,sv)   __sync_bool_compare_and_swap((volatile kmp_uint16*)(p),(kmp_uint16)(cv),(kmp_uint16)(sv))
# define KMP_COMPARE_AND_STORE_ACQ8(p,cv,sv)   __sync_bool_compare_and_swap((volatile kmp_uint8*)(p),(kmp_uint8)(cv),(kmp_uint8)(sv))
# define KMP_COMPARE_AND_STORE_REL8(p,cv,sv)   __sync_bool_compare_and_swap((volatile kmp_uint8*)(p),(kmp_uint8)(cv),(kmp_uint8)(sv))
#endif
#endif

#if ! defined KMP_TEST_THEN_INC32
# define KMP_TEST_THEN_INC32( ADDR )		( __kmp_test_then_add32( (ADDR), 1 ) )
#endif
#if ! defined KMP_TEST_THEN_INC_ACQ32
# define KMP_TEST_THEN_INC_ACQ32( ADDR )	( __kmp_test_then_add32( (ADDR), 1 ) )
#endif

#if ! defined KMP_TEST_THEN_INC64
# define KMP_TEST_THEN_INC64( ADDR )		( __kmp_test_then_add64( (ADDR), 1LL ) )
#endif
#if ! defined KMP_TEST_THEN_INC_ACQ64
# define KMP_TEST_THEN_INC_ACQ64( ADDR )	( __kmp_test_then_add64( (ADDR), 1LL ) )
#endif

#if ! defined KMP_TEST_THEN_ADD4_32
# define KMP_TEST_THEN_ADD4_32( ADDR ) 		( __kmp_test_then_add32( (ADDR), 4 ) )
#endif
#if ! defined KMP_TEST_THEN_ADD4_ACQ32
# define KMP_TEST_THEN_ADD4_ACQ32( ADDR ) 	( __kmp_test_then_add32( (ADDR), 4 ) )
#endif

#if ! defined KMP_TEST_THEN_ADD4_64
# define KMP_TEST_THEN_ADD4_64( ADDR ) 		( __kmp_test_then_add64( (ADDR), 4LL ) )
#endif
#if ! defined KMP_TEST_THEN_ADD4_ACQ64
# define KMP_TEST_THEN_ADD4_ACQ64( ADDR ) 	( __kmp_test_then_add64( (ADDR), 4LL ) )
#endif

#if ! defined KMP_TEST_THEN_DEC32
# define KMP_TEST_THEN_DEC32( ADDR )		( __kmp_test_then_add32( (ADDR), -1 ) )
#endif
#if !defined KMP_TEST_THEN_DEC_ACQ32
# define KMP_TEST_THEN_DEC_ACQ32( ADDR )	( __kmp_test_then_add32( (ADDR), -1 ) )
#endif

#if !defined KMP_TEST_THEN_DEC64
# define KMP_TEST_THEN_DEC64( ADDR )		( __kmp_test_then_add64( (ADDR), -1LL ) )
#endif
#if !defined KMP_TEST_THEN_DEC_ACQ64
# define KMP_TEST_THEN_DEC_ACQ64( ADDR )	( __kmp_test_then_add64( (ADDR), -1LL ) )
#endif

#if !defined KMP_TEST_THEN_ADD32
# define KMP_TEST_THEN_ADD32( ADDR, INCR )	( __kmp_test_then_add32( (ADDR), (INCR) ) )
#endif

#if !defined KMP_TEST_THEN_ADD64
# define KMP_TEST_THEN_ADD64( ADDR, INCR )	( __kmp_test_then_add64( (ADDR), (INCR) ) )
#endif

/* ------------- relaxed consistency memory model stuff ------------------ */

#if KMP_OS_WINDOWS
#if USE_ITT_BUILD
#   ifdef USE_ITT
#     define KMP_MB()     /* _asm{ nop } */
#     define KMP_IMB()    /* _asm{ nop } */
#   else
#    ifdef __ABSOFT_WIN
#      define KMP_MB()     asm ("nop")
#      define KMP_IMB()    asm ("nop")
#    else
#      define KMP_MB()     /* _asm{ nop } */
#      define KMP_IMB()    /* _asm{ nop } */
#     endif
#   endif /* USE_ITT */
#else
#    ifdef __ABSOFT_WIN
#      define KMP_MB()     asm ("nop")
#      define KMP_IMB()    asm ("nop")
#    else
#      define KMP_MB()     /* _asm{ nop } */
#      define KMP_IMB()    /* _asm{ nop } */
#     endif
#endif /* USE_ITT_BUILD */
#endif /* KMP_OS_WINDOWS */

#if KMP_ARCH_PPC64
# define KMP_MB()       __sync_synchronize()
#endif

#ifndef KMP_MB
# define KMP_MB()       /* nothing to do */
#endif

#ifndef KMP_IMB
# define KMP_IMB()      /* nothing to do */
#endif

#ifndef KMP_ST4_REL
# define KMP_ST4_REL(A,D) 	( *(A) = (D) )
#endif

#ifndef KMP_ST8_REL
# define KMP_ST8_REL(A,D) 	( *(A) = (D) )
#endif

#ifndef KMP_LD4_ACQ
# define KMP_LD4_ACQ(A)		( *(A) )
#endif

#ifndef KMP_LD8_ACQ
# define KMP_LD8_ACQ(A)		( *(A) )
#endif

/* ------------------------------------------------------------------------ */
//
// FIXME - maybe this should this be
//
// #define TCR_4(a)    (*(volatile kmp_int32 *)(&a))
// #define TCW_4(a,b)  (a) = (*(volatile kmp_int32 *)&(b))
//
// #define TCR_8(a)    (*(volatile kmp_int64 *)(a))
// #define TCW_8(a,b)  (a) = (*(volatile kmp_int64 *)(&b))
//
// I'm fairly certain this is the correct thing to do, but I'm afraid
// of performance regressions.
//

#define TCR_4(a)            (a)
#define TCW_4(a,b)          (a) = (b)
#define TCR_8(a)            (a)
#define TCW_8(a,b)          (a) = (b)
#define TCR_SYNC_4(a)       (a)
#define TCW_SYNC_4(a,b)     (a) = (b)
#define TCX_SYNC_4(a,b,c)   KMP_COMPARE_AND_STORE_REL32((volatile kmp_int32 *)(volatile void *)&(a), (kmp_int32)(b), (kmp_int32)(c))
#define TCR_SYNC_8(a)       (a)
#define TCW_SYNC_8(a,b)     (a) = (b)
#define TCX_SYNC_8(a,b,c)   KMP_COMPARE_AND_STORE_REL64((volatile kmp_int64 *)(volatile void *)&(a), (kmp_int64)(b), (kmp_int64)(c))

#if KMP_ARCH_X86

    #define TCR_PTR(a)          ((void *)TCR_4(a))
    #define TCW_PTR(a,b)        TCW_4((a),(b))
    #define TCR_SYNC_PTR(a)     ((void *)TCR_SYNC_4(a))
    #define TCW_SYNC_PTR(a,b)   TCW_SYNC_4((a),(b))
    #define TCX_SYNC_PTR(a,b,c) ((void *)TCX_SYNC_4((a),(b),(c)))

#else /* 64 bit pointers */

    #define TCR_PTR(a)          ((void *)TCR_8(a))
    #define TCW_PTR(a,b)        TCW_8((a),(b))
    #define TCR_SYNC_PTR(a)     ((void *)TCR_SYNC_8(a))
    #define TCW_SYNC_PTR(a,b)   TCW_SYNC_8((a),(b))
    #define TCX_SYNC_PTR(a,b,c) ((void *)TCX_SYNC_8((a),(b),(c)))

#endif /* KMP_ARCH_X86 */

/*
 * If these FTN_{TRUE,FALSE} values change, may need to
 * change several places where they are used to check that
 * language is Fortran, not C.
 */

#ifndef FTN_TRUE
# define FTN_TRUE       TRUE
#endif

#ifndef FTN_FALSE
# define FTN_FALSE      FALSE
#endif

typedef void    (*microtask_t)( int *gtid, int *npr, ... );

#ifdef USE_VOLATILE_CAST
# define VOLATILE_CAST(x)        (volatile x)
#else
# define VOLATILE_CAST(x)        (x)
#endif

#ifdef KMP_I8
# define KMP_WAIT_YIELD           __kmp_wait_yield_8
# define KMP_EQ                   __kmp_eq_8
# define KMP_NEQ                  __kmp_neq_8
# define KMP_LT                   __kmp_lt_8
# define KMP_GE                   __kmp_ge_8
# define KMP_LE                   __kmp_le_8
#else
# define KMP_WAIT_YIELD           __kmp_wait_yield_4
# define KMP_EQ                   __kmp_eq_4
# define KMP_NEQ                  __kmp_neq_4
# define KMP_LT                   __kmp_lt_4
# define KMP_GE                   __kmp_ge_4
# define KMP_LE                   __kmp_le_4
#endif /* KMP_I8 */

/* Workaround for Intel(R) 64 code gen bug when taking address of static array (Intel(R) 64 Tracker #138) */
#if (KMP_ARCH_X86_64 || KMP_ARCH_PPC64) && KMP_OS_LINUX
# define STATIC_EFI2_WORKAROUND
#else
# define STATIC_EFI2_WORKAROUND static
#endif

// Support of BGET usage
#ifndef KMP_USE_BGET
#define KMP_USE_BGET 1
#endif


// Warning levels
enum kmp_warnings_level {
    kmp_warnings_off = 0,		/* No warnings */
    kmp_warnings_low,			/* Minimal warmings (default) */
    kmp_warnings_explicit = 6,		/* Explicitly set to ON - more warnings */
    kmp_warnings_verbose		/* reserved */
};

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* KMP_OS_H */
