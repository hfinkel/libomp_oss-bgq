/*
 * kmp_ftn_cdecl.c -- Fortran __cdecl linkage support for OpenMP.
 * $Revision: 42061 $
 * $Date: 2013-02-28 16:36:24 -0600 (Thu, 28 Feb 2013) $
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

#include "kmp.h"

// Note: This string is not printed when KMP_VERSION=1.
char const __kmp_version_ftncdecl[] = KMP_VERSION_PREFIX "Fortran __cdecl OMP support: "
#ifdef USE_FTN_CDECL
    "yes";
#else
    "no";
#endif

#ifdef USE_FTN_CDECL

#define FTN_STDCALL 	/* no stdcall */
#define KMP_FTN_ENTRIES	USE_FTN_CDECL

#include "kmp_ftn_os.h"
#include "kmp_ftn_entry.h"

#endif /* USE_FTN_CDECL */

