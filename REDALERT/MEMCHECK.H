//
// Copyright 2020 Electronic Arts Inc.
//
// TiberianDawn.DLL and RedAlert.dll and corresponding source code is free 
// software: you can redistribute it and/or modify it under the terms of 
// the GNU General Public License as published by the Free Software Foundation, 
// either version 3 of the License, or (at your option) any later version.

// TiberianDawn.DLL and RedAlert.dll and corresponding source code is distributed 
// in the hope that it will be useful, but with permitted additional restrictions 
// under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT 
// distributed with this program. You should have received a copy of the 
// GNU General Public License along with permitted additional restrictions 
// with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection


/* #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#

                    MemCheck 3.0 Professional for DOS

            Copyright (c) 1990-1994, StratosWare Corporation.  
                           All rights reserved.

                             1-800-WE-DEBUG

        Note to Developers:
        --------------------
        This file should be #included AFTER any other #includes in
        each source file which is to be memory checked, and BEFORE
        any code that performs any operations on allocated pointers.
        If it isn't, MemCheck will not pick up source file and line
        information for intercepted functions. 

        The MCCONFIG.EXE utility distributed with MemCheck 3.0
        will do this safely and quickly for you.

        Most specifically, this header file MUST NOT come before
        any prototypes of routines that MemCheck intercepts, like
        malloc(), free(), strcpy(), and so on.

        The Final Cut:
        ---------------
        To ENTIRELY remove MemCheck from your code, just #define
        the constant "NOMEMCHECK", or equivalently, "NOMC".
        
        This header file will then automatically 'evaporate' all 
        MemCheck 3.0 calls.  This is MUCH PREFERABLE to placing 
        #if-#endif's around the header file's inclusion, as in

                    #ifdef DEBUG        // DON'T DO THIS!
                    #include <memcheck.h>
                    #endif

        Using the "#ifdef DEBUG" as above doesn't allow the
        MemCheck header file to evaporate the MemCheck 3.0 API
        calls you may have placed in your code, like mc_startcheck()
        and mc_endcheck().  You would then have to surround
        each MemCheck API call with additional #if-#endif's.

    Modification History

    WHO     WHEN        WHAT
    KWB     07/28/93    Gussy for beta
    KWB     09/11/93    Add new placement overload, NEW() C++ stuff
    KWB     11/08/93    Final QA/QC for initial release
    KWB     12/02/93    LINT -save added
    KWB     02/19/94    Fixed function inlining holes, added macros
                        for underscore func variants under MSC 7+
    KWB     07/08/94    Added defines for BC extender (_CC_POWERPACK_)
    KWB     07/09/94    Added special case for STACKTOP, END under PowerPack
    KWB     08/04/94    Added cdecl modifier to stklen, atopsp etc. decls
    KWB     08/11/94    Associated _CC32_ with _PROTECTED_ in ccdefs section;
                        Changed method of determining compiler model,
                        e.g. _CC_MSC6_ from if == to if >=
                        Associated DOSX286 with _PROTECTED_ for Phar Lap
                        Added MC_SET_EXCEPTF, mc_set/get_exceptf()
    KWB     08/17/94    Added new[] support filtering (_CPP_ANSI20_)
    KWB     08/18/94    Changed _MCFARCALL to _MCFARGLUE 
    KWB     09/13/94    Added erf_printf as alias for erf_stdout
    KWB     09/14/94    Added endf_summary
    KWB     09/21/94    Added MCCRITF and mc_set_critf() & related
    KWB     10/10/94    Added #if !defined(setmem) etc. for BC DPMI32
    KWB     10/11/94    Added _CC_BORLANDx_ comp def, 'x' = major ver

*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*# */


/* Avoid multiple inclusions */
#ifndef _MEMCHECK_H_
#define _MEMCHECK_H_

/* Prevent inclusion in Windows(tm) compilations */
#if defined(_Windows) || defined(WINDOWS) || defined(_WINDOWS)
#   if !defined (__DPMI16__) && !defined (__DPMI32__) && !defined (DOSX286)
#       error DOS version of MemCheck header file #included!
#   endif
#endif

/* Shorthand equivalence, V2.0 backwards compatibility... */
#if defined (NOMC) || defined (NOMEMCHK)
#   define NOMEMCHECK
#endif

/*  C++ new interception -- see note later and
    read the MemCheck 3.0 User's Manual, section
    "Integration With C++."  Uncommenting the next line
    and following simple instructions allows seamless
    transmission of the exact file and line location
    of new's in your source code.
*/
/*  #define NEW_OVERLOADED  */


/* *** Backwards compatibility with V2.0 *** */

#define mc_isactive         mc_is_active    /* standardize naming... */
#define mc_getmode          mc_get_mode
#define mc_errorstatus      mc_error_flags
#define mc_verify_memory    mc_check_buffers

#define MC_USEDISK          MC_USING_DISK
#define MC_USEMEM           MC_USING_MEMORY


/* *** Backwards compatibility with V2.1 *** */

#define MCLINENO            MCSL            /* "MemCheck Source Line" */
#define MFUNC               ERF             /* error reporting function */

#define mc_set_msgfunc      mc_set_erf      /* "Message funcs" are now    */
#define mc_get_msgfunc      mc_get_erf      /* universally referred to as */
#define mc_error_status     mc_error_flags  /* "error reporting functions"*/

/*  Maintain source code compatibility with version 2.1.
    Buffer registration is simplified to
    just calling "mc_register", regardless of model.  
    Same with buffer checking, e.g. "mc_check_far" 
    and "mc_check_near" are rolled into "mc_check". 
*/
#define mc_register_near(p,s)   mc_register((void _MCFAR *)(p),s)
#define mc_register_far(p,s)    mc_register((void _MCFAR *)(p),s)
#define mc_unregister_near(p)   mc_unregister((void _MCFAR *)(p))
#define mc_unregister_far(p)    mc_unregister((void _MCFAR *)(p))
#define mc_check_near(p)        mc_check((void _MCFAR *)(p))
#define mc_check_far(p)         mc_check((void _MCFAR *)(p))

/*  Error Number Definitions
    Returned by mc_endcheck() and mc_error_flags().
    In MemCheck 3.0, there's now a global error number much
    like the "errno" variable in standard C.   
*/
#define MCE_NO_ERROR            0       /* it's debugging time & all well */
#define MCE_NULL_SOURCE         1       /* null source ptr on copy */
#define MCE_NULL_DEST           2       /* null dest ptr on copy */
#define MCE_UNDERWRITE          3       /* allocated buf underwritten (front) */
#define MCE_OVERWRITE           4       /* allocated buf overwritten (end) */
#define MCE_LEAK                5       /* forgot to free alloc'd memory */
#define MCE_LEAKAGE             MCE_LEAK
#define MCE_UNFREED_MEMORY      MCE_LEAK
#define MCE_NULL_PTR_ASSIGN     6       /* assigned data through null ptr */
#define MCE_BAD_STACK_PTR       7       /* bad stack pointer */
#define MCE_STACK_OVERWRITE     8       /* copy trashes stack frame */
#define MCE_INTERNAL            9       /* internal error msg */
#define MCE_OVERLAPPING_COPY    10      /* source overlaps dest on copy */
#define MCE_INVALID_PTR         11      /* bad ptr on free, realloc */
#define MCE_DEST_OVERWRITE      12      /* copy too big for dest buffer */
#define MCE_OUT_OF_MEMORY       13      /* out of memory */
#define MCE_OOM                 MCE_OUT_OF_MEMORY   
#define MCE_GPF_PTR             14      /* ptr caused GPF */


/*  MemCheck Error Flags 

    The MemCheck error flag is just an unsigned long 
    (specifically, a MCEFLAGS typedef) with "sticky" bits 
    corresponding to the above MCE_... error numbers.  
    You can use the error flags macro MC_EFLAG(e) to 
    test the global MC_Errno double flag word.
*/
/* Returns a long word with the e-th bit set */
#define MC_EFLAG(e) ( (e) ? ((MCEFLAGS)1 << (e-1)) : (MCEFLAGS)0)

#define EFLAG_NULL_PTR          MC_EFLAG(MCE_NULL_DEST)
#define EFLAG_BAD_PTR           MC_EFLAG(MCE_UNALLOCED_PTR)
#define EFLAG_FRONT_MAGIC       MC_EFLAG(MCE_UNDERWRITE)
#define EFLAG_BACK_MAGIC        MC_EFLAG(MCE_OVERWRITE)
#define EFLAG_PTRS_NOT_FREED    MC_EFLAG(MCE_LEAK)
#define EFLAG_TRACK_FILE        0   /*obsolete in 3.0+*/
#define EFLAG_NULL_ASSIGN       MCE_FLAG(MCE_NULL_PTR_ASSIGN)

/* *** End Compatibility Section *** */


/* *** MemCheck Compiler Constant Definitions *** */

/* 
    Compiler            Defines
    --------            -------
    Microsoft           _CC_MSC_, _CC_MSC_COMPATIBLE_
        V8.x            _CC_MSC8_
        V7.x            _CC_MSC7_
        V6.x            _CC_MSC6_
        V5.x            _CC_MSC5_
    Borland*            _CC_BORLAND_, _CC_BCC_
        V3.x            _CC_BORLAND3_
        V4.x            _CC_BORLAND4_
        PowerPack/16    _CC_POWERPACK_, _CC_POWERPACK16_
        PowerPack/32    _CC_POWERPACK_, _CC_POWERPACK32_, _CC32_
    WATCOM              _CC_WATCOM_, _CC_MSC_COMPATIBLE_
        WAT/386         _CC_WATCOM32_, _CC32_

    * Borland has no good way of determining compiler
      version.  __BORLANDC__ returns some truly funky
      hex constant that "will increase in future versions."

    Define              Meaning
    ------              --------
    _CC32_ *            32-bit compile
    _PROTECTED_         16- or 32-bit protected mode compile
    LCODE               Defined if large code model
    LDATA               Defined if large data model
    STACKTOP            Highest stack address (top)
    STACKEND            Lowest stack address 
    STACKLEN            Stack length (top - end)
    _CPP_ANSI20_        Compiler supports C++ 2.0, e.g. new[]

    * If _CC32_ is defined, _PROTECTED_ is also defined
*/
        

#ifndef _CCDEFS_H_
#define _CCDEFS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*  NOTE: Microsoft C 5.x users have to "#define _MSC_VER 500"
    at the top of this file.
*/
#if defined(_MSC_VER)
/*
     _MSC_VER            Microsoft C version; currently defined as 700.
     M_I86   _M_I86      Member of the I86 processor family.
     M_I86mM _M_I86mM    Memory model type:
                         <m>= T            Tiny
                              S            Small (default)
                              C            Compact model
                              M            Medium model
                              L            Large model
                              H            Huge model
                         Identifiers defined by /AT, /AS, /AC, /AM,
                         /AL, and /AH, respectively.
     _MSDOS              MS-DOS operating system.
     _QC                 Microsoft QuickC Compiler.
     _WINDLL             Windows protected-mode dynamic-link library
                         is selected with /GD.
     _WINDOWS            Windows protected-mode is selected with /GA,
                         /Gn, /GW, /Mq, or /GD.
*/
#   define _CC_MSC_
#   define _CC_MSC_COMPATIBLE_

#   if (_MSC_VER >= 800)
#       define _CC_MSC8_
#   elif (_MSC_VER >= 700)
#       define _CC_MSC7_
#   elif (_MSC_VER >= 600)
#       define _CC_MSC6_
#   elif (_MSC_VER >= 500)  /* see note above */
#       define _CC_MSC5_
#   else
#       error MemCheck.h: unrecognized version of Microsoft compiler!
#   endif

#elif defined(__BORLANDC__)
#   define _CC_BORLAND_ /* Borland product */
#   define _CC_BCC_     /* Borland C compiler */

    /* Major compiler rev */
#   if (__BORLANDC__ >= 0x0450)
#       define _CC_BORLAND4_
#   elif (__BORLANDC__ >= 0x400)
#       define _CC_BORLAND3_
#   else
        /* not needed */
#   endif

    /* Borland 4.0 PowerPack BCC.EXE defines (-WX):
            __DPMI16__ _Windows

       With -WXD, -WXDE:
            __DLL__ __DPMI16__ _Windows

       Borland 4.0 PowerPack BCC21.EXE defines (-WX):
            __CONSOLE__ __DPMI32__ __FLAT__ __WIN32__ _Windows

       With -WXD:
            __DLL__ __CONSOLE__ __DPMI32__ __FLAT__ __WIN32__ _Windows
    */
#   if defined(__DPMI16__)
#       define _CC_POWERPACK_
#       define _CC_POWERPACK16_
#       define _PROTECTED_
#   endif
#   if defined(__DPMI32__)
#       define _CC_POWERPACK_
#       define _CC_POWERPACK32_
#       define _CC32_       /* flat model */
#   endif

/* Turbo C++ */
#elif defined(MCTCP)    /* homebrew */
#   define _CC_BORLAND_ /* Borland product */
#   define _CC_TCP_     /* Turbo C++ */

#elif defined(__TURBOC__)
/*
        __TURBOC__      Gives the current Turbo C version
                        number, a hexadecimal number.  Version
                        1.0 id 0x1000; version 1.2 is 0x0102, etc.
        __TINY__, __SMALL__, __MEDIUM__, 
        __COMPACT__, __LARGE__, __HUGE__
                        Model defintions 
        __MSDOS__       Signals that we're not yet in the
                        twenty-first century
*/
#   define _CC_BORLAND_ /* Borland C product */
#   define _CC_TCC_     /* Turbo C/C++ compiler */


#elif defined(_INTELC32_)
/*
        _INTELC32_      has the value 1.
        _ARCHITECTURE_  is 386 if the nomod486 pragma is ineffect,
                        486 otherwise.
*/
#   define _CC_INTEL_       /* Intel Code Builder */
#   define _CC_MSC_COMPATIBLE_
#   define _CC32_           /* flat model */

#elif defined(__WATCOMC__) 
/*  
        __WATCOMC__     Used to determine if the WATCOM C
                        or C/386 compiler is compiling
        __386__         identifies the target machine as an
                        Intel 80386 under the WATCOM C/386 compiler
        MSDOS           Signals that we're not yet in the
                        twenty-first century
        __FLAT__, __SMALL__, __MEDIUM__, 
        __COMPACT__, __LARGE__  
                        Model defintions (flat is default)
                        Also id's MSC-compatible model defines

        8.5 and later:
        __WINDOWS__     Identifies 16-bit Windows ("zw" or "zW" opts)
        __WINDOWS_386__ 32-bit Microsoft Windows "zW" opt 
        __NETWARE_386__ Novell Netware 386, defined by the 
                        Novell/Watcom C 
        __OS2__         IBM OS/2-hosted version of Watcom 
*/
#   define _CC_WATCOM_      /* Watcom C product */
#   define _CC_MSC_COMPATIBLE_
#   ifdef __386__
#       define _CC32_       /* flat model */
#       define _CC_WATCOM32_
#   endif


#elif defined(__STDC__)     /* Standard ANSI C */
#   define _CC_ANSI_
#   define _CC32_           /* no segmentation via far/near */
#   define far
#   define near
#   define huge
#   define cdecl

/* Avoids parameter mismatches from _far, etc. */
#   define _far
#   define _near
#   define _huge
#   define _cdecl

#else       /* UNSUPPORTED / UNRECOGNIZED COMPILER */

#error MemCheck 3.0: unrecognized compiler
/*
    If you're using Microsoft C5.1, you must
    define the constant _MSC_VER, e.g.

    #define _MSC_VER    500

    Place this statement at the top of this
    header file or in your project header file
    BEFORE the MemCheck header file is included.

    Microsoft C 5.0 is NOT supported (it's non-ANSI).
*/
    
#endif  /* compiler constant definitions */

/* END of _CC..._ name setups; ripple & alias */

/* Sheer utility & avoidance of !_CC32_ */
#ifndef _CC32_
#   define _CC16_   
#endif

/* 32-bit compilers are always protected mode */
#ifdef _CC32_
#ifndef _PROTECTED_
#   define _PROTECTED_
#endif
#endif  /* CC32 */

/* Phar Lap support */
#ifdef DOSX286
#ifndef _PROTECTED_
#   define _PROTECTED_
#endif
#endif  /* DOSX286 */

/* C++ 2.0 compliance: _CPP_ANSI20_ */
#if defined(_CC_BORLAND_)
    /* Only BC++ 4.x and later, but Borland has
       seriously mangled version info (__BORLANDC__) */
#   if defined (__BCPLUSPLUS__)
#       if (__BCPLUSPLUS__ > 0x0310)    /* after 3.1 */
#           define _CPP_ANSI20_
#       endif
#   elif defined(_CC_POWERPACK_)
#           define _CPP_ANSI20_
#   endif
#elif defined (_CC_MSC_)
    /* Current release through 8.x doesn't support new[] */
#elif defined (_CC_WATCOM_)
#   if (__WATCOMC__ >= 1000)    /* 10.x */
#           define _CPP_ANSI20_
#   endif
#endif


/*******************************************************************/
/*******************************************************************/

/*  *** Code and Data Size Constants *** 
    LCODE not used by this header file.
    LDATA *is* used, however.

    May be commented out if these constants are already defined.
*/

/*  #ifndef LCODE   */
#if defined(M_I86MM) || defined(M_I86LM) || defined(M_I86HM)
#   define LCODE    1
#elif defined(__MEDIUM__) || defined(__LARGE__) || defined(__HUGE__)
#   define LCODE    1
#endif
/*  #endif  */

#if defined(__COMPACT__) || defined(__LARGE__) || defined(__HUGE__)
#   define LDATA    1
#elif defined(M_I86CM) || defined(M_I86LM) || defined(M_I86HM)
#   define LDATA    1
#endif

/* Macro "aliases" */

#ifndef _LCODE
#   ifdef LCODE
#       define _LCODE   LCODE
#   endif
#endif

#ifndef _LDATA
#   ifdef LDATA
#       define _LDATA   LDATA
#   endif
#endif

/*******************************************************************/
/*******************************************************************/

/* *** Physical Stack Address *** */

#if defined(_CC_BORLAND_)   /* -------------------------- */

    /*
    BORLAND RTL Notes:
    ;----------------------------------------------------------------------
    ; In large data models, the stack is in its own segment.  The stack
    ; starts at SS:__stklen and grows down to SS:0.
    ;
    ; In small data models, the stack is in the DGROUP, and grows down
    ; to meet the heap.  The end of the heap is marked by ___brklvl.
    (KWB: Note that the brklvl is adjusted upwards until it meets
    _stklen...)
    ;----------------------------------------------------------------------
    */

#   define STACKSLOP    256
    extern unsigned cdecl _stklen;

#   if defined(_CC_POWERPACK_)
#       define  STACKTOP    (mc_stacktop())
#       define  STACKEND    (mc_stackend())
#   else /* not P-Pack */
#   ifdef LDATA
            /*  Compact, Large, Huge Models ...

                The stack starts at SS:_stklen and
                grows downward to SS:0
            */
#           define  STACKTOP    ((unsigned) _stklen)
#           define  STACKEND    ((unsigned) 0 + STACKSLOP)

#   else
        /*  Small, Medium Models ...

            The stack starts at SS:0xFFFE and grows
            downwards _stklen bytes.
        */
#       define  STACKTOP    ((unsigned) 0xFFFE)
#       define  STACKEND    (STACKTOP - _stklen + STACKSLOP)
#   endif
#   endif   /* not PowerPack */

#elif defined (_CC_MSC_)  /* ------------------------------- */

    extern char cdecl end;              /* end of stack */
    extern unsigned cdecl _atopsp;      /* top of stack */

#   define  STACKTOP    _atopsp
#   define  STACKSLOP   256

#   ifdef LDATA
        /* If in far data, stack could be in its own
            seg. tho not usually. see /FARSTACK */
#       define  STACKEND    ((unsigned) ((unsigned long)&end) + STACKSLOP)
#   else
        /*  If near data, must be in DS; use near ptr */
#       define  STACKEND    ((unsigned)&end + STACKSLOP)
#   endif

#elif defined (_CC_WATCOM_)  /* ------------------------------- */

    extern unsigned _STACKLOW;      /* end of stack */
    extern unsigned _STACKTOP;      /* top of stack */

#   define  STACKTOP    (_STACKTOP)
#   define  STACKSLOP   256

#   ifdef LDATA
#       define  STACKEND    ((unsigned) (_STACKLOW + STACKSLOP))
#   else
#       define  STACKEND    ((unsigned) (_STACKLOW + STACKSLOP))
#   endif

#else                   /* Unknown compiler ---------------- */

#error Unknown compiler at __FILE__(__LINE__)

#endif      /* defining stack top, end */

/*******************************************************************/
/*******************************************************************/

#ifdef __cplusplus
}
#endif

#endif  /* _ccdefs already #included */

/* End CCDEFS */


#if !defined (NULL)     /* pull in stdio.h if not already */
#   include <stdio.h>
#endif

/* Backup... sometimes NULL defined in other headers */
#if !defined (_IOFBF)   /* pull in stdio.h if not already */
#   include <stdio.h>
#endif


/* *** MemCheck Constants *** */

/*  Standard from MemCheck 3.0 onwards.
    Access major version and revision
    via mc_version() and mc_revision() macros.
*/
#define _MC_VERSION     0x0300      /* welcome 3.0 the powerful */

#define mc_version()    ((int)((_MC_VERSION & 0xFF00) >> 8))
#define mc_revision()   ((int)(_MC_VERSION & 0x00FF))

#if defined (_CC32_)    /* 32-bit Intel target */
#define PRT_FP  "0x%p"
#else
#define PRT_FP  "%Fp"
#endif

/* *** MCID Macro *** */

/*  Allows later flexibility in assigning mapping...
    Also makes MCIDs formulaic 
*/
#define _MCID(f)    (MCID)(__paste(MCID_,f))

/*
    MemCheck Function ID's (MCID's)

    These are the indices used to retrieve information
    about specific runtime library calls.
*/
/* --- MEMCHECK INTERNAL FUNCTIONS --- */

#define _MCID_FIRST_INTERNAL    0       /* index of first internal func */

#define MCID_mc_startcheck      0
#define MCID_mc_endcheck        1
#define MCID_mc_check_buffers   2
#define MCID_mc_check           3
#define MCID_mc_register        4
#define MCID_mc_unregister      5
#define MCID_mc_set_alignsize   6
#define MCID_mc_set_checkbytes  7
#define MCID_mc_nullcheck       8
#define MCID_mc_breakpoint      9
#define MCID_mc_debug           10
#define MCID_mc_set_location    11
#define MCID_mc_stack_trace     12
#define MCID_mc_report          13

#define _MCID_LAST_INTERNAL         14  /* Set = to last internal ID */


/* ***************   STANDARD C ROUTINES     ******************* */

#define _MCID_FIRST_ANSI        (_MCID_LAST_INTERNAL+1)

#define MCID_calloc             (_MCID_FIRST_ANSI + 0)
#define MCID_free               (_MCID_FIRST_ANSI + 1)
#define MCID_malloc             (_MCID_FIRST_ANSI + 2)
#define MCID_memcpy             (_MCID_FIRST_ANSI + 3)
#define MCID_memmove            (_MCID_FIRST_ANSI + 4)
#define MCID_memset             (_MCID_FIRST_ANSI + 5)
#define MCID_realloc            (_MCID_FIRST_ANSI + 6)
#define MCID_sprintf            (_MCID_FIRST_ANSI + 7)
#define MCID_strcat             (_MCID_FIRST_ANSI + 8)
#define MCID_strcpy             (_MCID_FIRST_ANSI + 9)
#define MCID_strncat            (_MCID_FIRST_ANSI + 10)
#define MCID_strncpy            (_MCID_FIRST_ANSI + 11)
#define MCID_vsprintf           (_MCID_FIRST_ANSI + 12)

#define _MCID_LAST_ANSI     MCID_vsprintf       /* define to last ANSI */


/* ***************   MICROSOFT C     ******************* */

#if defined(_CC_MSC_COMPATIBLE_)

#define _MCID_FIRST_MSC     (_MCID_LAST_ANSI + 1)

#   define MCID__expand         (_MCID_FIRST_MSC + 0)
#   define MCID__ffree          (_MCID_FIRST_MSC + 1)
#   define MCID__fcalloc        (_MCID_FIRST_MSC + 2)
#   define MCID__fmalloc        (_MCID_FIRST_MSC + 3)
#   define MCID__fmsize         (_MCID_FIRST_MSC + 4)
#   define MCID__frealloc       (_MCID_FIRST_MSC + 5)
#   define MCID__fexpand        (_MCID_FIRST_MSC + 6)
#   define MCID__msize          (_MCID_FIRST_MSC + 7)

#   define MCID__fmemmove       (_MCID_FIRST_MSC + 8)
#   define MCID__fmemcpy        (_MCID_FIRST_MSC + 9)
#   define MCID__fmemset        (_MCID_FIRST_MSC + 10)
#   define MCID__fmemccpy       (_MCID_FIRST_MSC + 11)
#   define MCID__fstrcat        (_MCID_FIRST_MSC + 12)
#   define MCID__fstrncat       (_MCID_FIRST_MSC + 13)
#   define MCID__fstrcpy        (_MCID_FIRST_MSC + 14)
#   define MCID__fstrncpy       (_MCID_FIRST_MSC + 15)
#   define MCID__fstrdup        (_MCID_FIRST_MSC + 16)
#   define MCID__fstrset        (_MCID_FIRST_MSC + 17)
#   define MCID__fstrnset       (_MCID_FIRST_MSC + 18)

#   define MCID__nfree          (_MCID_FIRST_MSC + 19)
#   define MCID__nmalloc        (_MCID_FIRST_MSC + 20)
#   define MCID__ncalloc        (_MCID_FIRST_MSC + 21)
#   define MCID__nrealloc       (_MCID_FIRST_MSC + 22)
#   define MCID__nexpand        (_MCID_FIRST_MSC + 23)
#   define MCID__nmsize         (_MCID_FIRST_MSC + 24)
#   define MCID__nstrdup        (_MCID_FIRST_MSC + 25)

#   define MCID__dos_setvect    (_MCID_FIRST_MSC + 26)
#   define MCID__getdcwd        (_MCID_FIRST_MSC + 27)

/*  Here starts the Great ANSI Divide.
    MSC6 and earlier have no underscores;
    MSC7 and later *have* underscores to emphasize
        departure from ANSI...
*/
#if defined(_CC_MSC_) && !defined (MSC6)    /* not MSC6-compatible */

#   define MCID__getcwd         (_MCID_FIRST_MSC + 28)
#   define MCID__cgets          (_MCID_FIRST_MSC + 29)
#   define MCID__halloc         (_MCID_FIRST_MSC + 30)
#   define MCID__hfree          (_MCID_FIRST_MSC + 31)
#   define MCID__memccpy        (_MCID_FIRST_MSC + 32)
#   define MCID__strdup         (_MCID_FIRST_MSC + 33)
#   define MCID__strnset        (_MCID_FIRST_MSC + 34)
#   define MCID__strset         (_MCID_FIRST_MSC + 35)
#   define MCID__swab           (_MCID_FIRST_MSC + 36)
#   define MCID__tempnam        (_MCID_FIRST_MSC + 37)

#else   /*** MSC6 and before; WATCOM ***/

/* No leading underscores */

#   define MCID_getcwd          (_MCID_FIRST_MSC + 28)
#   define MCID_cgets           (_MCID_FIRST_MSC + 29)
#   define MCID_halloc          (_MCID_FIRST_MSC + 30)
#   define MCID_hfree           (_MCID_FIRST_MSC + 31)
#   define MCID_memccpy         (_MCID_FIRST_MSC + 32)
#   define MCID_strdup          (_MCID_FIRST_MSC + 33)
#   define MCID_strnset         (_MCID_FIRST_MSC + 34)
#   define MCID_strset          (_MCID_FIRST_MSC + 35)
#   define MCID_swab            (_MCID_FIRST_MSC + 36)
#   define MCID_tempnam         (_MCID_FIRST_MSC + 37)

#endif  /* MSC6 ANSI calls */

#   define MCID_new             (_MCID_FIRST_MSC + 38)
#   define MCID_delete          (_MCID_FIRST_MSC + 39)
#   define MCID__fullpath       (_MCID_FIRST_MSC + 40)

#endif  /* Microsoft C-compatible calls */


/* ***************   BORLAND C     ******************* */

#if defined (_CC_BORLAND_)

#define _MCID_FIRST_BC      (_MCID_LAST_ANSI + 1)

#   define MCID__fmemmove       (_MCID_FIRST_BC + 0)
#   define MCID__fmemcpy        (_MCID_FIRST_BC + 1)
#   define MCID__fmemset        (_MCID_FIRST_BC + 2)
#   define MCID__fmemccpy       (_MCID_FIRST_BC + 3)
#   define MCID__fstrcat        (_MCID_FIRST_BC + 4)
#   define MCID__fstrncat       (_MCID_FIRST_BC + 5)
#   define MCID__fstrcpy        (_MCID_FIRST_BC + 6)
#   define MCID__fstrncpy       (_MCID_FIRST_BC + 7)
#   define MCID__fstrdup        (_MCID_FIRST_BC + 8)
#   define MCID__fstrset        (_MCID_FIRST_BC + 9)
#   define MCID__fstrnset       (_MCID_FIRST_BC + 10)

#   define MCID__dos_setvect    (_MCID_FIRST_BC + 11)
#   define MCID__getdcwd        (_MCID_FIRST_BC + 12)

#   define MCID_getcwd          (_MCID_FIRST_BC + 13)
#   define MCID_cgets           (_MCID_FIRST_BC + 14)
#   define MCID_memccpy         (_MCID_FIRST_BC + 15)
#   define MCID_strdup          (_MCID_FIRST_BC + 16)
#   define MCID_strnset         (_MCID_FIRST_BC + 17)
#   define MCID_strset          (_MCID_FIRST_BC + 18)
#   define MCID_swab            (_MCID_FIRST_BC + 19)
#   define MCID_tempnam         (_MCID_FIRST_BC + 20)

#   define MCID_farmalloc       (_MCID_FIRST_BC + 21)
#   define MCID_farrealloc      (_MCID_FIRST_BC + 22)
#   define MCID_farfree         (_MCID_FIRST_BC + 23)
#   define MCID_farcalloc       (_MCID_FIRST_BC + 24)
#   define MCID_movmem          (_MCID_FIRST_BC + 25)
#   define MCID_setmem          (_MCID_FIRST_BC + 26)
#   define MCID_setvect         (_MCID_FIRST_BC + 27)
#   define MCID_stpcpy          (_MCID_FIRST_BC + 28)
#   define MCID__fmovmem        (_MCID_FIRST_BC + 29)
#   define MCID__fsetmem        (_MCID_FIRST_BC + 30)
#   define MCID_new             (_MCID_FIRST_BC + 31)
#   define MCID_delete          (_MCID_FIRST_BC + 32)
#   define MCID__fullpath       (_MCID_FIRST_BC + 33)

#endif


/* 
    'TOUCH' macro so high warning levels don't generate
    'unreferenced variable' warnings, especially when
    making Production libraries... All MemCheck code
    compiles without a whymper.
*/
#if defined (_CC_WATCOM_)
#   define TOUCH(var)   var = var
#elif defined (_CC_BORLAND4_)
#   define TOUCH(var)   var = var
#else
#   define TOUCH(var)   if (var)
#endif


/* Default log name used by stock erf_logfile() and variants... */
#define MEMCHECK_LOG    "MEMCHECK.LOG"

#define MAX_MEMORY  1000    /* 1000K is more than ever possible */

/* User-Modifiable Defaults */

#define D_CheckByteCt   sizeof(int)     /* word size is default */
#define D_AlignSize     sizeof(int)     /* align returned memory ptrs */

/*  Number of bytes to copy from null segment (to determine null
    pointer assignments) 
*/
#define D_NULLCHECK_BYTES_FAR       16      /* at 0000:0000 (far NULL) */
#define D_NULLCHECK_BYTES_NEAR      16      /* at   DS:0000 (near NULL) */
#define MAX_NULLCHECK_BYTES_FAR     1024    /* extent of irupt vect tbl */
#define MAX_NULLCHECK_BYTES_NEAR    66      /* reserved in DS */

/* Unroll the double-negative */
/*
    Debugging code specific to MemCheck can be 
    conditionally compiled by placing it within
    #if-#endif sections:  (NOTE that this is NOT
    required when just using API functions)

        #ifdef MEMCHECK

        void _MCCALLBACK trackf_special (int op, MEMRECP memrecp)
        {
           (... your custom callback code ...)
        }

        #endif

    instead of the more arcane

        #ifndef NOMEMCHECK
        :
        #endif

    (Both approaches work equally well, however...)
*/
#ifndef NOMEMCHECK      /* MemCheck active */
#define MEMCHECK    
#endif


/* *** Calling Conventions *** */

#if !defined (_CC_ANSI_)
#define     _MCAPI          pascal      /* MemCheck API functions   */
#define     _FASTAPI        pascal      /* speed-critical functions */
#define     _MCCDECL        cdecl       /* MemCheck varargs API     */
#define     _MCCALLBACK     cdecl       /* callback functions       */
#define     _MCVAR          cdecl       /* MemCheck global variable */
#else
#define     _MCAPI          /* MemCheck API functions   */
#define     _FASTAPI        /* speed-critical functions */
#define     _MCCDECL        /* MemCheck varargs API     */
#define     _MCCALLBACK     /* callback functions       */
#define     _MCVAR          /* MemCheck global variable */
#endif

#if !defined(_CC_WATCOM_)
#   define  _RTL            _MCCDECL    /* RTL calling conv */
#else
#   define  _RTL                        /* RTL calling conv */

/*  WATCOM C++ does not currently (2/17/94)
    accept "cdecl" as a modifier on variables...  
*/
#   undef _MCVAR
#   define _MCVAR
#endif  /* _CC_WATCOM_ */

/*   32-bit compiler-independent stuff */
#if !defined(_CC32_) 
#define _MCFAR      far
#define _MCFARCALL  far
#define _MCNEAR     near
#define _MCNEARCALL near
#define _MCHUGE     huge
#else
#define _MCFAR  
#define _MCFARCALL
#define _MCNEAR
#define _MCNEARCALL
#define _MCHUGE 
#endif  /* _CC32_ */

/*
    MSC declares the following routines as "far"... 
    So does Borland.  WATCOM does not; define glue.

        _fstrset        _fstrnset       _fstrcpy 
        _fstrncpy       _fstrcat        _fstrncat
        _fmemset        _fmemmove       _fmemccpy
*/
#if !defined(_CC_WATCOM_)
#   define _MCFARGLUE   far
#else
#   define _MCFARGLUE   
#endif


/*  Microsoft C7 and later will not have
    have a malloc_mc, only _fmalloc_mc; likewise
    with free_mc.
    The RTLMALLOC and RTLFREE macros are used
    to refer to a generically present back-end malloc
    and free.
*/
#if defined (_CC_MSC_)
#   if defined (LDATA)
#       define RTLMALLOC    RTL(_fmalloc)
#       define RTLFREE      RTL(_ffree)
#   else
#       define RTLMALLOC    RTL(_nmalloc)
#       define RTLFREE      RTL(_nfree)
#   endif
#else   /* non-MSC */
#   define RTLMALLOC    RTL(malloc)
#   define RTLFREE      RTL(free)
#endif


/*  WATCOM defines its atexit funcs as a "register",
    which causes a param type mismatch.  
    _ATEXITFUNC calling convention smooths this out.
*/
#if defined (_CC_WATCOM_)
#   define _ATEXITFUNC  
#else
#   define _ATEXITFUNC      _MCCDECL
#endif


/*  MemCheck Tracking Mode 
    
    Returned by mc_get_mode().
    Indicates whether information on each allocation
    is being stored in memory or on disk.
*/
#define MC_USING_MEMORY     1
#define MC_USING_DISK       2


/* Min, max orders for each node in the B-tree */

#define BT_ORDER_MIN        5
#define BT_ORDER_MAX        255     /* maximum tree order */
#define BT_ORDER_DEFAULT    19      /* default tree order */

/*
    Returned by mc_get_speed().
    Pass as params to mc_set_speed().
*/
#define MC_RUN_NORMAL       1
#define MC_RUN_FAST         2

/*  For mc_report():
    "Flags" field of the MEMREC structure
    is set to REPORT_START or REPORT_END
    to indicate begin and end of report.

    NOTE: If REPORT_START or REPORT_END conflicts
        with defines in your project, just comment
        them out and use the MC_... variants instead.
*/
#define REPORT_START        (MRFLAGS)0xFE
#define REPORT_END          (MRFLAGS)0xFD

#define MC_REPORT_START     (MRFLAGS)0xFE   /* alternates in case of conflict */
#define MC_REPORT_END       (MRFLAGS)0xFD


/*
    Maximum number of breakpoints that
    can be set via mc_breakpoint().
*/
#define MC_MAX_BREAKS       50


/*  "Optype" parameter on Tracking function callback. */
#define TRACKF_ADD      1       /* record being added to tree */
#define TRACKF_DEL      2       /* record being deleted from tree */

/*  Used for the mcflags field of MEMREC to indicate
    whether file & line are exact or approximate
*/
#define MRFLAG_EXACT_LOC        ( (MRFLAGS) 0x01)

/*
    Set if the values for a MEMREC are already converted
    to "user" values.
*/
#define MRFLAG_USER_SPECS       ( (MRFLAGS) 0x02)
#define MRFLAG_NO_CHECKBYTES    ( (MRFLAGS) 0x04)

/* Alternate name */
#define mc_message      mc_debug 

/*
    Parameter to mc_check_transfer() that
    specifies that the given data transfer function cannot
    have overlapping source & destination.
    (MCID's are unsigned bytes.)
*/
#define MCF_NO_OVERLAP      ((unsigned)0x8000)
#define NO_OVERLAP(mcid)    ((mcid) | MCF_NO_OVERLAP)

/*  Parameter to mc_check_transfer indicating that
    the found memory record is not needed */
#define NO_MEMREC           ((MEMRECP)NULL) 
#define NOMEMREC            NO_MEMREC

/*  Parameter to mc_check_transfer indicating that
    there is no source pointer operand associated
    with the data transfer being checked: e.g. memset.  */
#define NO_SOURCE           ((void _MCFAR *)0xFFFFFFFA)


/* *** TYPEDEFS *** */

typedef char *          MCSF;       /* MemCheck source file */
typedef unsigned int    MCSL;       /* MemCheck source line */
typedef unsigned char   MCID;       /* MemCheck function ID */

typedef unsigned long   MCEFLAGS;   /* MemCheck error flags */
typedef void _MCFAR *   MCPTR;      /* type of ptr stored in tree */
typedef unsigned char   MRFLAGS;    /* flags in MEMRECORD */
typedef unsigned long   MCFLAGS;    /* MemCheck settings flags */

/* MemCheck Rocket allocator prototypes */
typedef void _MCFAR * (_MCFAR *ROCKETALLOCF)    (size_t);
typedef void          (_MCFAR *ROCKETFREEF)     (void _MCFAR *);

#pragma pack(1)
/* 
    Memory Tracking Structure (MEMREC)

    This is the data structure for buffers being
    tracked by MemCheck.
*/
typedef struct MemRecord 
    {
        MCPTR           ptr;            /* heap/registered ptr */
        MCID            mcid;           /* MemCheck function ID */
        MRFLAGS         flags;          /* internal MC flags */
        unsigned long   allocno;        /* cardinality of allocation */
        unsigned long   size;           /* size of block */
        MCSF            file;           /* source file */
        MCSL            line;           /* source line */

    } MEMREC, _MCFAR *MEMRECP;


/* *** SETTINGS *** */
/* These are values that describe the life of a MemCheck run. */

typedef struct MCSETTINGS { 
    /*
        Bit Flag                What
        --- ---------------     -----------------------------------
        0   MCF_ACTIVE          MemCheck active or off
        1   MCF_FAST_MODE       Fast mode or normal
        2   MCF_PROTECTED_MODE  Protected mode or real
        3   MCF_FAR_NULL_CHECK  Check for far NULL ptr assigns *
        4   MCF_NEAR_NULL_CHECK Check for far NULL ptr assigns *
        5   MCF_STANDARD_STACK  Standard stack frame *
        6   MCF_AUTOINIT        Start up automatically *
        7   MCF_CLEAR_ON_FREE   Clear buffers when freed 
        8   MCF_DISK_ROCKET     Use DiskRocket options
        9   MCF_IDX_IN_MEMORY   Use memory only for Rocket indexes *
                                (only if DiskRocket linked)
        10  MCF_SOURCE_ONLY     Intercept in source code only 

        11 - 31 Reserved
    */
    MCFLAGS         Flags;              /* Main settings flags */

    unsigned short  MaxMem;             /* Max mem for tree usage, in K */
    unsigned short  NearNullBytes;      /* bytes to check in near null */
    unsigned short  FarNullBytes;       /*  "    "    "    " far   "   */
    unsigned char   CheckByteCt;        /* check byte count */
    unsigned char   AlignSize;          /* alignment boundary size */
    char            TrackingDir[36];    /* Rocket stores temp files here */

    } MCSETTINGS, *MCSETTINGSP;


/* Random 32-bit .CFG file sentinel */
#define MC_CFG_FILE_SENTINEL        ( (unsigned long) 0x10F23BC4 )

typedef struct MCCfgInfo {

    unsigned long   sentinel;           /* always MC_CFG_FILE_SENTINEL */
    MCSETTINGS      MemCheckSettings;   /* saved by user */

    } MCCFGINFO, *MCCFGINFOP;


#ifndef _CC32_

/* 16-bit exception stack */
typedef struct {

    unsigned    xRetIP;
    unsigned    xRetCS;
    unsigned    xErr;
    unsigned    xIP;
    unsigned    xCS;
    unsigned    xFlags;
    unsigned    xSP;
    unsigned    xSS;

    } MCEXCEPTINFO;

#else

/* 32-bit exception stack */
typedef struct {

    unsigned long   xRetEIP;
    unsigned short  xRsvd1;
    unsigned short  xRetCS;
    unsigned long   xErr;
    unsigned long   xEIP;
    unsigned short  xRsvd2;
    unsigned short  xCS;
    unsigned long   xFlags;
    unsigned long   xESP;
    unsigned short  xRsvd3;
    unsigned short  xSS;

    } MCEXCEPTINFO;

#endif  /* _CC32_ */

/* Values for MCCRITSECT.action */
#define MCCS_ENTER_SECTION  0
#define MCCS_LEAVE_SECTION  1

#define MCCS_ACTION(pMCCS)  (pMCCS->nAction)
#define MCCS_ENTER(pMCCS)   ((*(pMCCS->pLocked))++) /* inc flag */
#define MCCS_LEAVE(pMCCS)   ((*(pMCCS->pLocked))--) /* dec flag */

/* 
    Critical section object - ptr to this passed to crit sect callback 
    WARNING:  access these fields ONLY via the MCCS_...() macros.
        To do otherwise subjects you to changes in implementation
        of the underlying coordination of critical section locking.
*/
typedef struct {
    int             nAction;    /* MCCS_ENTER/LEAVE_SECTION */
    int *           pLocked;    /* # times entered          */
    unsigned long   ulRsvd;     /* internal use             */
    } MCCRITSECT;

#pragma pack()


#define MC_FEXIT    ( (MCID) 0xFF )


/* Error Reporting Function typedef */
#ifndef _ERF_DEFINED
#define _ERF_DEFINED
typedef void (_MCCALLBACK *ERF) (char *);
#endif


/* *** Callback Functions *** */

/* Interception callback (on every interception) */
typedef void (_MCCALLBACK * GLOBALF) (void);

/* Called whenever nodes added to or deleted from MC database */
typedef void (_MCCALLBACK *TRACKF) (int, MEMRECP);

/* User-definable check function to add to transfer checking */
typedef void (_MCCALLBACK * CHECKF) (
                int ,           /* 0 or MCE_... error val for this xfer op */
                void _MCFAR *,  /* user ptr dest */
                long            /* bytes to copy to dest */
            );

/* Funcs called at startup or shutdown */
typedef void (_MCCALLBACK *STARTF) (void);
typedef void (_MCCALLBACK *ENDF)   (void);

/* Report function type passed to mc_report() */
typedef void (_MCCALLBACK *REPORTF) (MEMRECP);

/* Additional heap pointer verification (troubleshoot only) */
typedef int (_MCCALLBACK *VERIFYF) (void _MCFAR *);

typedef void (*MCVOIDFP) (void);

/* Exception handler */
typedef void (_MCCALLBACK _MCFAR *MCEXCEPTF) (void);

/* Multitasking enter/exit critical section callback.
   Specify as param to mc_set_critf() at beginning of program;
   callback function will be called with MCCS_ENTER_SECTION
   on entry to MemCheck, or MCCS_LEAVE_SECTION on exit.

   NOT TO BE CONFUSED WITH A MEMCHECK "GLOBAL" FUNCTION.
   Global functions (GLOBALF's) are used to perform any
   actions on the interception of a runtime function;
   CRITF's must be used ONLY to serialize access to MemCheck.
*/
typedef void (_MCCALLBACK * MCCRITF) (MCCRITSECT *);


/*  Stack Frame Handler 

    You can define your own function to 
    record, analyze, or inspect each stack frame
    when mc_stack_trace() is called.

    You *cannot* modify ANY of the values passed
    in, as the "const" typing indicates.  If you need to
    modify a value, make a copy.  See the MemCheck 3.0
    documentation for more details on stack frame handlers.
*/

typedef void (_MCFAR _MCCDECL *_SSFRAMEHANDLER ) (
            short    const ,        /* AX: near/far/error flag   */
            unsigned short const ,  /* CX: near (default) rtn CS */
            unsigned short const ,  /* ES: far rtn CS            */
            unsigned const ,        /* DI: rtn offset from stack */
            short const             /* DX: frame count           */
    );

/*  Values for "flag" constant parameter to a
    stack frame handler. 
*/
#define TRACE_BAD_FRAME     0x00        /* couldn't recognize frame     */
#define TRACE_FAR_CALL      0x01        /* frame represents a far call  */
#define TRACE_NEAR_CALL     0x02        /*   "        "     " near  "   */
#define TRACE_BAD_CHAIN     0x03        /* frame BP chewed up           */
#define TRACE_BEGIN         0x80        /* signals begin walk           */
#define TRACE_END           0x81        /* signals end of walk          */


/* MC Settings Structure, "flags" member: */
#define MCF_ACTIVE          (MCFLAGS)(0x01)
#define MCF_FAST_MODE       (MCFLAGS)(0x02)
#define MCF_PROTECTED_MODE  (MCFLAGS)(0x04)
#define MCF_FAR_NULL_CHECK  (MCFLAGS)(0x08)
#define MCF_NEAR_NULL_CHECK (MCFLAGS)(0x10)
#define MCF_STANDARD_STACK  (MCFLAGS)(0x20)
#define MCF_AUTOINIT        (MCFLAGS)(0x40)
#define MCF_CLEAR_ON_FREE   (MCFLAGS)(0x80)
#define MCF_DISK_ROCKET     (MCFLAGS)(0x100)
#define MCF_IDX_IN_MEMORY   (MCFLAGS)(0x200)
#define MCF_SOURCE_ONLY     (MCFLAGS)(0x400)


/* *** Conditional Compilation *** */

/* --------------------------------------------------------------
    If MEMCHECK is not being `compiled out' (via definition
    of the constant NOMEMCHECK), include this section...
-------------------------------------------------------------- */

#if !defined(MEMCHECK)

/* Include Section for `MemCheck Not Active' */

/*  *****************************
     MemCheck Not Active Section
    *****************************

    This section completely removes or
    "evaporates" all MemCheck function references
    from your projects when you compile with
    NOMEMCHECK #defined.

    There's no need to remove any MemCheck
    headers or statements from your code
    to produce a full production version
    of your application.

                    o
                   ooo
                 ooooooo
                ooooooooo
               ooooooooooo
                   ooo
                   ooo
                   ooo
                   ooo
                                    */

#ifndef MEMCHECK_MODULE

/*  Evaporate all MemCheck 3.0 API
    statements to do nothing, safely... */

#   define  mc_alloc_count()                0L
#   define  mc_blocks_allocated()           0L
#   define  mc_blocks_freed()               0L
#   define  mc_breakpoint(fi)               0
#   define  mc_bytes_allocated()            0L
#   define  mc_bytes_freed()                0L
#   define  mc_check(p)                     0
#   define  mc_check_buffers()              0
#   define  mc_cur_file()                   "No file"
#   define  mc_cur_line()                   0
#   define  mc_debug(s)
#   define  mc_debugf(_args)
#   define  mc_debug_on()
#   define  mc_debug_off()
#   define  mc_endcheck()                   (MCEFLAGS)0
#   define  mc_errno()                      MCE_NO_ERROR
#   define  mc_error_flags()                (MCEFLAGS)0
#   define  mc_error_text(e)                "MemCheck not active"
#   define  mc_except_text(e)               "MemCheck not active"
#   define  mc_file()                       "No file"
#   define  mc_find_buffer(p,mr)            0
#   define  mc_func()                       (MCID)0
#   define  mc_func_name(mcid)              ("")
#   define  mc_get_erf()                    (ERF)NULL
#   define  mc_get_mode()                   0
#   define  mc_get_speed()                  0
#   define  mc_in_source()                  0
#   define  mc_is_active()                  0
#   define  mc_line()                       0
#   define  mc_load_debugger()
#   define  mc_location_text()              "MemCheck not active"
#   define  mc_memory_leaked()              0L
#   define  mc_memrec()                     (MEMRECP)NULL
#   define  mc_memrec_text()                "MemCheck not active"
#   define  mc_msg_continued()              0
#   define  mc_nullcheck()                  0
#   define  mc_null_snapshot() 
#   define  mc_register(p,s)    
#   define  mc_report(_f)
#   define  mc_set_alignsize(_s)
#   define  mc_set_breakfile(_f)
#   define  mc_set_checkbytes(_cb)
#   define  mc_set_checkf(_f)               (CHECKF)NULL
#   define  mc_set_critf(_f)                
#   define  mc_set_endf(erf)                (ENDF)NULL
#   define  mc_set_erf(erf)                 (ERF)NULL
#   define  mc_set_globalf(_f)              (GLOBALF)NULL
#   define  mc_set_globalexitf(_f)          (GLOBALF)NULL
#   define  mc_set_speed(_s)
#   define  mc_set_location()
#   define  mc_set_trackf(_f)               (TRACKF)NULL
#   define  mc_set_tracefile(_f)
#   define  mc_set_tree_order(_q)
#   define  mc_set_track_dir(_dir)          
#   define  mc_source_ptr()                 (MCPTR)NULL
#   define  mc_stack_trace(_memo)           0
#   define  mc_startcheck(_erf)
#   define  mc_unregister(p)                
#   define  mc_set_exceptf(f)   
#   define  mc_get_exceptf()                ((MCEXCEPTF)NULL)

/* *** Stock error reporting functions *** */
#   define  erf_default(_msg)
#   define  erf_standard(_msg)
#   define  erf_logfile(_msg)
#   define  erf_log_only(_msg)
#   define  erf_trace(_msg)

/* Internal Helpers */
#   define  _direct_output(_s)              
#   define  _mcsl(_f,_l)
#   define  _mcsl_delete(_f,_l)
#   define  _mcsl_new(_f,_l)
#   define  _mcslx(_f,_l,_s)
#   define  _mc_set_delflag()
#   define  _mc_set_location(_f,_l)         
#   define  _mc_set_newflag()

/* Link-time compileouts */
#   define  MC_SET_AUTOINIT(_toggle)
#   define  MC_SET_CHECK_FREQ(_freq)    
#   define  MC_SET_CHECKF(_f)   
#   define  MC_SET_CRITF(_f)    
#   define  MC_SET_ENDF(_f) 
#   define  MC_SET_ENV_VAR(_envvarname)
#   define  MC_SET_ERF(_f)  
#   define  MC_SET_GLOBALF(_f)  
#   define  MC_SET_GLOBALEXITF(_f)  
#   define  MC_SET_LOGFILE(_logfilename)    
#   define  MC_SET_PANIC_BUFFERS(_q)
#   define  MC_SET_SSFH(_f) 
#   define  MC_SET_STARTF(_f)   
#   define  MC_SET_TRACKF(_f)   
#   define  MC_SET_VERIFYF(_f)  

/* Back-end direct */
#define RTL(_f)     _f

/* *** C++ *** */
#ifdef __cplusplus

#define NEW(_object)        new _object
#define DELETE(_object)     delete _object
#define DELETE_ARR(_arr)    delete[] _arr

#define cpp_malloc(_s)      malloc(_s)
#define cpp_calloc(_n,_s)   calloc(_n,_s)
#define cpp_free(_p)        free(_p)

/* Borland C++ */
#define cpp_farmalloc(_s)   farmalloc(_s)
#define cpp_farfree(_fp)    farfree(_fp)

/* Microsoft C++-compatibles */
#define cpp__fmalloc(_s)    _fmalloc(_s)
#define cpp__ffree(_fp)     _ffree(_fp)

#endif  /* C++ */

#endif  /* not MEMCHECK_MODULE */


/* #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*# */

#else   /* MEMCHECK is defined */

/* #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*# */

#pragma message ("MemCheck V3.0")

/*
    *************************
     MemCheck Active Section
    *************************

    The rest of this header file deals with
    MemCheck's being compiled into an application.

                                                   */

/* Specify that vars and funcs are straight C.. */
#ifdef __cplusplus
extern "C" {
#endif


/* *** ANSI Location Defines *** */

#define _MC_NO_FILE ((MCSF) 0)      /* just in case... */
#define _MC_NO_LINE ((MCSL) 0)


/*  Allow for the possibility of _MCSF_ being
    defined to reference a single, static module
    filename.  This prevents a multiplicity of 
    static filenames getting added to the DGROUP, e.g.

        static char *_thisfile = __FILE__;
        #define _MCSF_  ((MCSF)thisfile)
        #include <memcheck.h>

    This is only needed under MSC pre-VC++.
    Borland has "-d" Merge duplicate strings.
    VC++ has "/Gf" Elim duplicate strings.
*/
#if !defined (_MCSF_)
#   ifdef __FILE__
#      define _MCSF_        (MCSF)__FILE__
#   else
#      define _MCSF_        _MC_NO_FILE
#   endif
#endif

#ifdef __LINE__
#   define _MCSL_       (MCSL)__LINE__
#else
#   define _MCSL_       _MC_NO_LINE
#endif


/* *** Standard ANSI C Includes *** 

    For va_list function call args 
    Inclusion of this header won't change
    the behavior of host code.
*/
#if !defined (va_start)     /* avoid multiple inclusion... */
#   include <stdarg.h>
#endif


/* *** Compiler-specific includes *** */

/*lint -save -e537 Repeated include files (if necessary) */
#if defined(_CC_MSC_COMPATIBLE_)

#   if !defined (_INC_MALLOC)   /* C7.x and later optimization */
#       include <malloc.h>
#   endif

#   if !defined (_INC_STRING)   /* C7.x and later optimization */
#       include <string.h>
#   endif   

#elif defined(_CC_BORLAND_)

#   if !defined (__ALLOC_H)
#       include <alloc.h>           
#   endif

/* String functions must be proto'd before pragmas */
#   if !defined (__STRING_H)
#       include <string.h>
#   endif

#endif  /* Compiler-specific includes */
/*lint -restore */

#if defined (_CC_POWERPACK32_)
extern void cdecl mcinitfp_startcheck (void);
extern void cdecl mcexitfp_endcheck (void);
#pragma startup mcinitfp_startcheck     16
#pragma exit    mcexitfp_endcheck       16
#endif

/***************************************/
/* *** MemCheck 3.0 API Prototypes *** */
/***************************************/

/* Internal helper macro - proto shorthand */
#define _LOCP   MCSF,MCSL

extern  unsigned long _MCAPI mc_alloc_count (void);
extern  unsigned long _MCAPI mc_blocks_allocated (void);
extern  unsigned long _MCAPI mc_blocks_freed     (void);
extern  unsigned long _MCAPI mc_bytes_allocated  (void);
extern  unsigned long _MCAPI mc_bytes_freed      (void);
extern  int         _MCAPI mc_check (void _MCFAR *);
extern  int         _MCAPI mc_check_buffers (void);
extern  MCSF        _MCAPI mc_cur_file (void);
extern  MCSL        _MCAPI mc_cur_line (void);
extern  void        _MCCDECL mc_debugv (const char *, ...);
extern  void        _MCAPI mc_debug (const char *);
extern  MCEFLAGS    _MCAPI mc_endcheck (void);
extern  MCEFLAGS    _MCAPI mc_error_flags (void);
extern  char *      _MCAPI mc_error_text (int);
extern  int         _MCAPI mc_errno (void);
extern  char *      _MCAPI mc_except_text (unsigned);
extern  MCSF        _MCAPI mc_file (void);
extern  int         _MCAPI mc_find_buffer(void _MCFAR *realptr,MEMRECP memrecp);
extern  MCID        _MCAPI mc_func (void);
extern  char *      _MCAPI mc_func_name(MCID);
extern  ERF         _MCAPI mc_get_erf (void);
extern  MCEXCEPTF   _MCAPI mc_get_exceptf (void);
extern  int         _MCAPI mc_get_mode (void);
extern  int         _MCAPI mc_get_speed (void);
extern  char *      _MCAPI mc_get_tracefile (void);
extern  int         _MCAPI mc_in_source (void);
extern  int         _MCAPI mc_is_active (void);
extern  MCSL        _MCAPI mc_line (void);
extern  char *      _MCAPI mc_location_text (void);
#define  mc_load_debugger()     _asm int 3
extern  unsigned long _MCAPI mc_memory_leaked (void);
extern  char *      _MCAPI mc_memrec_text (MEMRECP);
extern  MEMRECP     _MCAPI mc_memrec (void);
extern  int         _MCAPI mc_msg_continued (void);
extern  int         _MCAPI mc_nullcheck (void);
extern  void        _MCAPI mc_null_snapshot (void);
extern  void        _MCAPI mc_register (void _MCFAR *, unsigned long);
extern  void        _MCAPI mc_report (REPORTF);
extern  void        _MCAPI mc_set_alignsize (unsigned int);
extern  void        _MCAPI mc_set_breakfile (char *);
extern  void        _MCAPI mc_set_checkbytes (unsigned int);
extern  CHECKF      _MCAPI mc_set_checkf (CHECKF);
extern  void        _MCAPI mc_set_critf (MCCRITF);
extern  ENDF        _MCAPI mc_set_endf (ENDF);
extern  ERF         _MCAPI mc_set_erf (ERF);
extern  MCEXCEPTF   _MCAPI mc_set_exceptf (MCEXCEPTF);
extern  GLOBALF     _MCAPI mc_set_globalf (GLOBALF);
extern  GLOBALF     _MCAPI mc_set_globalexitf (GLOBALF);
#define mc_set_location() _mc_set_location(_MCSF_,_MCSL_)
extern  MCPTR       _MCAPI mc_source_ptr (void);
extern  void        _MCAPI mc_set_speed (int);
extern  void        _MCAPI mc_set_tracefile (char *);
extern  void        _MCAPI mc_set_track_dir (char *);
extern  TRACKF      _MCAPI mc_set_trackf (TRACKF);
extern  void        _MCAPI mc_set_tree_order (int);
extern  int         _MCAPI mc_stack_trace (char *);
extern  void        _MCAPI mc_startcheck (_LOCP, ERF);
extern  void        _ATEXITFUNC mc_endcheck_at_exit (void);
extern  void        _MCAPI mc_unregister (void _MCFAR *);

/* Debugging versions of the MemCheck library only */
extern  void _MCAPI mc_debug_on  (void);
extern  void _MCAPI mc_debug_off (void);
extern  int _MCCALLBACK mc_search_heap (void _MCFAR *);

/* *** INTERNAL API HELPERS *** */
extern void _MCAPI   _mc_set_location (_LOCP);
extern void _FASTAPI _mcsl  (MCSF,MCSL);            /* location run-ahead */
extern void _FASTAPI _mcslx (MCSF,MCSL,size_t);     /* location run-ahead */
extern void _FASTAPI _mcsl_new (MCSF,MCSL);         /* location run-ahead */
extern void _FASTAPI _mcsl_delete (MCSF,MCSL);      /* location run-ahead */
extern void _FASTAPI _mc_set_newflag (void);        /* new's a'comin'     */
extern void _FASTAPI _mc_set_delflag (void);        /* delete's a'comin'  */

/* Misc - uses INT 9 to output directly to screen */
#if !defined (_CC_WATCOM32_)
extern void _MCCDECL _MCFAR _direct_output (char _MCFAR *);
#else
#define _direct_output(s)   printf ("%s\n", s)  /* ALERT */
#endif

/*
    mc_breakpoint() is now a MemCheck Tracking function (TRACKF).
    Tracking functions get called every time
    MemCheck adds or deletes from its database.
*/
#define mc_breakpoint(_f)   \
            mc_set_trackf( (mc_set_breakfile (_f), trackf_breakpoint) )
#define mc_breakpoint_trace(_f) \
            mc_set_trackf( (mc_set_tracefile (_f), trackf_breakpoint_trace) )


/* *** Advanced-user API extenders *** */

/*  extern  int     _MCAPI mc_find_buffer(void _MCFAR *, MEMRECP);  */
extern  int     _MCAPI mc_check_transfer(
                            void _MCFAR *,
                            void _MCFAR *,
                            unsigned long,
                            unsigned,
                            unsigned,
                            MEMRECP);

/*  mc_get_settings

    Write your own "get settings" routine
    to override the one shipped with MemCheck.
    You can hard-wire any settings you like, e.g.
    always ON for versions of your app shipped to 
    testers/QA stations, etc.
*/
extern void _MCCALLBACK mc_get_settings (MCSETTINGS *);


/* ***  Callbacks / Functionality Extenders *** 

    Function Type           Called...
    --------------          ------------------------------
    Error reporting         To handle each MemCheck error message 
    Global Interception     On each MemCheck interception
    Checking                On every data transfer check
    Tracking                On every allocation/deallocation
    Start                   On mc_startcheck or AutoInit
    End                     At mc_endcheck or MemCheck shutdown

    Refer to your MemCheck 3.0 manual for further details.

    *** STOCK FUNCTIONS *** 
    These functions are available in the MemCheck
    libraries as "ready-made" for your programming 
    pleasure in the categories above.
*/

/* *** Stock error reporting functions *** */

extern  void    _MCCALLBACK erf_default     (char *);
extern  void    _MCCALLBACK erf_standard    (char *);
extern  void    _MCCALLBACK erf_logfile     (char *);
extern  void    _MCCALLBACK erf_log_only    (char *);
extern  void    _MCCALLBACK erf_trace       (char *);
extern  void    _MCCALLBACK erf_trace_all   (char *);
extern  void    _MCCALLBACK erf_trace_obj   (char *);
extern  void    _MCCALLBACK erf_stdout      (char *);
extern  void    _MCCALLBACK erf_stderr      (char *);
extern  void    _MCCALLBACK erf_find_leaks  (char *);

#define erf_printf  erf_stdout      /* alias*/

/* *** Stock Tracking Functions *** */

extern  void    _MCCALLBACK trackf_default          (int, MEMRECP);
extern  void    _MCCALLBACK trackf_all              (int, MEMRECP);
extern  void    _MCCALLBACK trackf_all_2            (int, MEMRECP);
extern  void    _MCCALLBACK trackf_breakpoint       (int, MEMRECP);
extern  void    _MCCALLBACK trackf_breakpoint_trace (int, MEMRECP);
extern  void    _MCCALLBACK trackf_big_alloc        (int, MEMRECP);

/* *** Stock End Functions *** */

extern  void    _MCCALLBACK endf_default    (void); /* does nothing */
extern  void    _MCCALLBACK endf_info       (void); /* write run info to log */
extern  void    _MCCALLBACK endf_alert      (void); /* warn if run errs */
extern  void    _MCCALLBACK endf_summary    (void); /* warn if run errs */

/* *** Stock Start functions *** */

extern  void    _MCCALLBACK startf_default  (void); /* does nothing */
extern  void    _MCCALLBACK startf_info     (void); /* write options to log */

/* *** Stock Check Functions *** */

extern  void    _MCCALLBACK checkf_default (int,void _MCFAR *,long);
extern  void    _MCCALLBACK checkf_dataseg (
    int,            /* 0 or MCE_... error val for this xfer op */
    void _MCFAR *,  /* user ptr dest */
    long            /* bytes to copy to dest */
    );
extern  void    _MCCALLBACK checkf_verify_heap (int,void _MCFAR *,long);
                            
/* *** Stock Global Interception Functions *** */

extern  void        _MCCALLBACK globalf_default       (void); /* does nothing */
extern  void        _MCCALLBACK globalf_supercheck    (void);
extern  void        _MCCALLBACK globalf_check_buffers (void);
extern  void        _MCCALLBACK globalf_heapcheck     (void);

/* *** Stock Report Functions *** */
extern  void        _MCCALLBACK reportf_default (MEMRECP);

/* *** Stock Exception Handlers *** */
extern void         _MCCALLBACK _MCFAR exceptf_default (void);

/* *** Stock Stack Frame Handlers *** */
extern void _MCFAR _MCCDECL ssfh_info (
    short const, unsigned short const, unsigned short const, unsigned const, short const);

extern void _MCFAR _MCCDECL ssfh_fast (
    short const, unsigned short const, unsigned short const, unsigned const, short const);
                /*  int         const   _flag,      */

extern void _MCFAR _MCCDECL ssfh_standard (
    short const, unsigned short const, unsigned short const, unsigned const, short const);

extern void _MCFAR _MCCDECL ssfh_debug (
    short const, unsigned short const, unsigned short const, unsigned const, short const);

/* */
extern unsigned int _MCAPI mc_stacktop (void);  /* high address */
extern unsigned int _MCAPI mc_stackend (void);  /* low  address */


/*  Function external variables.

    These are used effectively with MemCheck 3.0's AutoInit
    setting.  Under AutoInit, MemCheck fires itself up automatically
    on its first interception.  Under these circumstances,
    there's no chance to have changed any defaults (like the
    ERF or error reporting function).  These variables provide
    a link-level method of setting these functions:

        #include <memcheck.h>
        :
        // Sets custom erf at link-time
        MC_SET_ERF (erf_custom_startup);
        :
*/
/* *** GLOBALS *** */

extern  ERF         _MCVAR MC_ERF;          /* error reporting func ptr     */
extern  CHECKF      _MCVAR MC_CheckF;       /* transfer check func          */
extern  MCCRITF     _MCVAR MC_CritF;        /* crit section enter/exit */
extern  GLOBALF     _MCVAR MC_GlobalF;      /* global interception callback */
extern  GLOBALF     _MCVAR MC_GlobalExitF;  /* called on exit interception */
extern  TRACKF      _MCVAR MC_TrackF;       /* alloc/dealloc callback       */
extern  STARTF      _MCVAR MC_StartF;       /* startup callback             */
extern  ENDF        _MCVAR MC_EndF;         /* shutdown callback            */

extern  VERIFYF     _MCVAR MC_VerifyF;      /* troubleshooting              */

extern  char *      _MCVAR MC_LogFile;      /* log file name used           */
extern  char        _MCVAR MC_UserAutoInit;
extern  int         _MCVAR MC_CheckFreq;    /* for globalf_supercheck() et al */
extern  char *      _MCVAR MC_EnvVar;       /* Env var to detect 'active'   */
extern  unsigned short _MCVAR MC_DataSeg;   /* DS value                     */

extern  int         _MCVAR MC_MaxTraceDepth;
extern  char *      _MCVAR MCST_Desc;       /* trace descrip to mc_..trc()  */

extern  MCSETTINGS  _MCVAR MC_DefaultSettings;  /* default settings         */
extern  MCSETTINGS  _MCVAR MC_Settings;         /* real settings--
                                                   USE WITH CARE!!!         */

extern  MCVOIDFP    _MCVAR MC_PMMap1;       /* p-mode func in map seg 1     */

/* Protected mode exception handling */
extern  unsigned char   _MCVAR MC_ExceptList[]; /* exceptions to handle */
extern  MCEXCEPTINFO    _MCVAR MC_ExceptInfo;   /* in exception */
extern  MCEXCEPTF       _MCVAR MC_ExceptF;      /* installed hdler */

/* Rocket Guidance Systems */
extern ROCKETALLOCF _MCVAR  MC_RocketAllocF;
extern ROCKETFREEF  _MCVAR  MC_RocketFreeF;
extern unsigned char _MCVAR MC_PanicBufCount;   /* anti-tree failure        */

/*  This char is used to fill freed buffers
    if the "ClearOnFree" option in effect.
    Default buffer clear char is 0.
*/
extern unsigned char _MCVAR MC_FreedBufferFillChar;

/*  Link-time defaults.

    These macros are "covers" to insulate you, the developer,
    from the underlying implementation, as well as to provide
    such bennies as compiling clean out of your code when
    NOMEMCHECK or NOMC is defined.  
    
    Use instead of accessing vars directly!

    To use, place the following in ONE MODULE e.g. your main module
    (any *one* module will work fine) after the MemCheck 
    header file has been included:

        #include <memcheck.h>
        MC_SET_...(params);

    For example, to change the default log file that MemCheck
    uses at runtime to C:\MYDEV\MYPROG.LOG:

        #include <memcheck.h>
        MC_SET_LOGFILE ("C:\\MYDEV\\MPROG.LOG");

    Most of these macros have runtime function equivalents,
    such as mc_set_erf() for MC_SET_ERF(), etc.  Notable
    exceptions to this are the following values that
    must generally have link-time initializations:

        MC_SET_LOGFILE()
        MC_SET_AUTOINIT()
        MC_SET_STARTF()
*/
#define MC_SET_AUTOINIT(_toggle)        \
            char _MCVAR MC_UserAutoInit = (_toggle);
#define MC_SET_CHECKF(_f)   \
            CHECKF  _MCVAR MC_CheckF    = (_f)
#define MC_SET_CHECK_FREQ(_freq)        \
            int _MCVAR MC_CheckFreq     = (_freq)
#define MC_SET_CRITF(_f)    \
            MCCRITF _MCVAR MC_CritF     = (_f)
#define MC_SET_ENDF(_f)     \
            ENDF    _MCVAR MC_EndF      = (_f)
#define MC_SET_ENV_VAR(_envvarname)     \
            char *  _MCVAR MC_EnvVar    = (_envvarname)
#define MC_SET_ERF(_f)      \
            ERF     _MCVAR MC_ERF       = (_f)
#define MC_SET_EXCEPTF(_f)      \
            MCEXCEPTF   _MCVAR MC_ExceptF = (_f)
#define MC_SET_GLOBALF(_f)  \
            GLOBALF _MCVAR MC_GlobalF   = (_f)
#define MC_SET_GLOBALEXITF(_f)  \
            GLOBALF _MCVAR MC_GlobalExitF = (_f)
#define MC_SET_LOGFILE(_f)      \
            char *  _MCVAR MC_LogFile   = (_f)
#define MC_SET_PANIC_BUFFERS(_q)        \
            unsigned char _MCVAR MC_PanicBufCount = (_q)
#define MC_SET_SSFH(_f)     \
            _SSFRAMEHANDLER _MCVAR near MC_SFrameHandler = (_f)
#define MC_SET_STARTF(_f)       \
            STARTF  _MCVAR MC_StartF    = (_f)
#define MC_SET_TRACKF(_f)   \
            TRACKF  _MCVAR MC_TrackF    = (_f)
#define MC_SET_VERIFYF(_f)  \
            VERIFYF _MCVAR MC_VerifyF   = (_f)

/*  Use the MC_BEGIN_EXCEPTLIST, MC_HANDLE_EXCEPTION,
    and MC_END_EXCEPTLIST macros to change the exceptions 
    MemCheck handles in protected mode by default.

    Usage (exactly as typed):
            #include <memcheck.h>
            :
            MC_BEGIN_EXCEPTLIST
                MC_HANDLE_EXCEPTION (0x0)
                MC_HANDLE_EXCEPTION (0xD)
            MC_END_EXCEPTLIST

    NOTE:
    To turn off MemCheck's exception handling completely, use 

            MC_SET_EXCEPTF(NULL);

    instead of trying to define an empty EXCEPTLIST...
*/
#define MC_BEGIN_EXCEPTLIST     \
            unsigned char _MCVAR MC_ExceptList[] = {
#define MC_HANDLE_EXCEPTION(e)  \
                (unsigned char)(e),
#define MC_END_EXCEPTLIST       \
                (unsigned char)0xFF };      /* 0xFF MUST end list */

/* ------------- End MemCheck 3.0 Library Calls --------------- */

/*  Formulaic rogue varargs interceptions;
    most host-code-compatible method...
    "Are you experienced?"

    "It is better to be mugged than
     to live in fear." - Anon.
*/
#define _VA_DEF(f,r,p)          \
        typedef r (_RTL *p_##f) p;  \
        extern p_##f _MCAPI _loc_##f (_LOCP);

/* Declare sprintf helper function */
_VA_DEF(sprintf,int,(char *, const char *, ...))

                               /* * * * * * * * * * * * * * * * * * * * * * *
    *************************
          Back-End RTL
    *************************
*/

/* *** Back-end functions *** */

/*  Macro to access true back-end RTL.
    Used internally by the MemCheck API functions. 
*/
#define __paste(x,y)    x ## y
#define RTL(func)       __paste(func,_mc)

/*  Macro to encapsulate the declaration of
    the renamed (zapped) back-end rtl 
*/
#define _RTLDECL(f,rctype,params) \
        extern rctype _RTL RTL(f) params


/*  For the conversion that MSC underwent
    from C 6 to 7, where non-ANSI calls
    have underbars 
*/
#if defined (_CC_MSC_) && !defined (MSC6) 
#if (_MSC_VER >= 700)
#   define _C7A
#endif
#endif

#ifdef _C7A
#define C7ANSI(func)    _##func
#else
#define C7ANSI(func)    func
#endif

#undef _C7A


/* ---------------------------------------------- */
/* These are the renamed ("zapped") RTL functions */
/* ---------------------------------------------- */

/* *** ANSI *** */

_RTLDECL(malloc,    void *,     (size_t));
_RTLDECL(calloc,    void *,     (size_t, size_t));
_RTLDECL(realloc,   void *,     (void *, size_t));
_RTLDECL(free,      void,       (void *));
_RTLDECL(memcpy,    void *,     (void *,const void *,size_t));
_RTLDECL(memmove,   void *,     (void *,const void *,size_t));
_RTLDECL(memset,    void *,     (void *,int,size_t));
_RTLDECL(strcpy,    char *,     (char *,const char *));
_RTLDECL(strncpy,   char *,     (char *,const char *,size_t));
_RTLDECL(strcat,    char *,     (char *,const char *));
_RTLDECL(strncat,   char *,     (char *,const char *,size_t));
_RTLDECL(vsprintf,  int,        (char *,const char *,va_list));
_RTLDECL(sprintf,   int,        (char *,const char *,...));

#if !defined (_CC_ANSI_)
/* *** MSC *** */

/* WATCOM doesn't support these... */
#if !defined(_CC32_)
_RTLDECL(_fmalloc,  void far *, (size_t));
_RTLDECL(_fcalloc,  void far *, (size_t, size_t));
_RTLDECL(_ffree,    void,       (void far *));
_RTLDECL(_fmsize,   size_t,     (void far *));
#endif

_RTLDECL(_nmalloc,  void _MCNEAR *,(size_t));
_RTLDECL(_nfree,    void,       (void _MCNEAR *));

/* *** Borland *** */

#if !defined(_CC_POWERPACK32_)  
_RTLDECL(farmalloc, void _MCFAR *,  (unsigned long));
_RTLDECL(farcalloc, void _MCFAR *,  (unsigned long, unsigned long));
_RTLDECL(farfree,   void,           (void _MCFAR *));

/* *** General Porpoise *** */

_RTLDECL(_fmemset,  void far * _MCFARGLUE,  (void far *,int,size_t));
_RTLDECL(_fmemcpy,  void far * _MCFARGLUE,  (void far *,const void far *,size_t ));
_RTLDECL(_fstrcpy,  char far * _MCFARGLUE,  (char far *,const void far *));
#endif  /* not _CC_POWERPACK32_ */

#endif  /* not STDC/ANSI */

/***************************************************************** 
 * --------   Function Call Interception Definitions   --------- * 
 *****************************************************************/

#ifndef MEMCHECK_MODULE

/*
    This section targets user's code only
*/

/* Func interceptors... */
#define     _INTERCEPT(_f)      (_mcsl(_MCSF_,_MCSL_),_f)
#define     _VA_INTERCEPT(_f)   (*_loc_##_f(_MCSF_,_MCSL_))
#define     _SETLOC(_f)         (mc_set_location(),_f)

/* NOTE near _mcsl with #if (_MCC_NEAR_INTERCEPT == 0) */

/*
    MC_NO_TRANSFER_SIZE is used to eliminate errors or warnings
    like "sizeof returns 0" or "Not allowed type in sizeof <expr>".
    These occur for unsized variables declared like

        extern unsigned char gHelpString[];     

    The optimal solution is to "size" the extern, e.g.

        extern unsigned char gHelpString[80];   

    but where this may not be practical, MC_NO_TRANSFER_SIZE may
    be defined on a module-by-module OR project-wide basis.
*/
#ifdef MC_NO_XFER_SIZE      /* beta compat */
#   define  MC_NO_TRANSFER_SIZE
#endif
#ifdef NO_TRANSFER_SIZE     /* alternate */
#   define  MC_NO_TRANSFER_SIZE
#endif

#if defined (MC_NO_TRANSFER_SIZE)
#   define  _INTERCEPTX(_f,_d)  _INTERCEPT(_f)
#else   /* standard; transmit sizeof dest */
#   define  _INTERCEPTX(_f,_d)  (_mcslx(_MCSF_,_MCSL_,sizeof(_d)),_f)
#endif


/*  Intrinsic Function Disabling

    It's important to disable function inlining for
    all intercepted functions.
*/

#if defined(_CC_MSC_)

/*  Intrinsics (== in-line functions) not permissible
    since they're implemented as macros...
*/
#pragma function(strcat)
#pragma function(strcpy)
#pragma function(memcpy)
#pragma function(memset)

#pragma function(strset)

#if defined(_MSC_VER)
#if (_MSC_VER >= 700)
#pragma function(_fmemcpy)
#pragma function(_fmemset)
#pragma function(_fstrcat)
#pragma function(_fstrcpy)
#pragma function(_fstrset)
#pragma function(_strset)
#endif
#endif /* defined _MSC_VER */

#elif defined(_CC_BORLAND_)

/* Turbo C not like pragmae */
#if !defined (_CC_TCC_)

/*  Eliminate duplicate strings.
    This can save a bit of space in large
    programs particularly, since each call to 
    MemCheck references an otherwise separate
    copy of the current filename. 
*/
#pragma option -d

/*  Intrinsics (== in-line functions) not permissible
    since they're implemented as macros, for one...
*/
#pragma intrinsic -strcat
#pragma intrinsic -strncat
#pragma intrinsic -strcpy
#pragma intrinsic -strncpy
#pragma intrinsic -stpcpy
#pragma intrinsic -strset
#pragma intrinsic -strnset
#pragma intrinsic -memcpy
#pragma intrinsic -memset

#endif  /* not Turbo C */

/* end Borland compiler intrinsics */

#elif defined (_CC_WATCOM_)

/*  NOTE:  unfortunately, WATCOM C/C++ compilers
    force inlining of the strcpy() function regardless
    of whether you want it inlined or not, all the time.
    So this pragma, while it should ensure that
    strcpy() is a function call, does not...  :{

    So we take other measures below: see _mcwatcom_strcpy()
*/
#pragma function(strcpy)

#pragma function(strcat)
#pragma function(memcpy)
#pragma function(memset)

#pragma function(_fmemcpy)
#pragma function(_fmemset)
#pragma function(_fstrcat)
#pragma function(_fstrcpy)

#endif

/* End disable function inlining */


/*lint -save -e652 Define of symbol declared previously */
#if defined (MC_NO_INTERCEPT)
#define NO_INTERCEPT
#endif

#if !defined (NO_INTERCEPT)

/* *** ANSI Standard C *** */

#define calloc(n,_sz)       _INTERCEPT(calloc(n,_sz))
#define malloc(_sz)         _INTERCEPT(malloc(_sz))
#define realloc(p,s)        _INTERCEPT(realloc(p,s))
#define free(p)             _INTERCEPT(free(p))

#define memcpy(d,s,n)       _INTERCEPTX(memcpy(d,s,n),d)
#define memmove(d,s,n)      _INTERCEPTX(memmove(d,s,n),d)
#define memset(p,c,n)       _INTERCEPTX(memset(p,c,n),p)
#define strcat(s1,s2)       _INTERCEPTX(strcat(s1,s2),s1)
#if defined(_CC_WATCOM_)    
    /* WATCOM forces inlining of strcpy()... see note above */
#   define strcpy(d,s)      _INTERCEPTX(_mcwatcom_strcpy(d,s),d)
    extern char * _RTL _mcwatcom_strcpy (char *, const char *);
#else
#   define strcpy(d,s)      _INTERCEPTX(strcpy(d,s),d)
#endif
#define strncat(s1,s2,n)    _INTERCEPTX(strncat(s1,s2,n),s1)
#define strncpy(d,s,n)      _INTERCEPTX(strncpy(d,s,n),d)
#define vsprintf(s,f,a)     _INTERCEPTX(vsprintf(s,f,a),s)

/*  #define sprintf             _VA_INTERCEPT(sprintf)  */
#ifndef _lint
#define sprintf             _INTERCEPT(sprintf)
#endif  

#if defined(_CC_MSC_COMPATIBLE_)    /* *** Microsoft C *** */

#define _expand(_p,_s)      _INTERCEPT(_expand(_p,_s))
#define _fcalloc(n,_sz)     _INTERCEPT(_fcalloc(n,_sz))
#define _fexpand(_p,_s)     _INTERCEPT(_fexpand(_p,_s))
#define _ffree(p)           _INTERCEPT(_ffree(p))
#define _fmalloc(_sz)       _INTERCEPT(_fmalloc(_sz))
#define _frealloc(p,s)      _INTERCEPT(_frealloc(p,s))
#define _fmsize(p)          _INTERCEPT(_fmsize(p))
#define _msize(p)           _INTERCEPT(_msize(p))
#define _nfree(p)           _INTERCEPT(_nfree(p))
#define _nmalloc(_sz)       _INTERCEPT(_nmalloc(_sz))
#define _nrealloc(p,s)      _INTERCEPT(_nrealloc(p,s))
#define _ncalloc(n,_sz)     _INTERCEPT(_ncalloc(n,_sz))
#define _nexpand(_p,_s)     _INTERCEPT(_nexpand(_p,_s))
#define _nmsize(p)          _INTERCEPT(_nmsize(p))
#define _nstrdup(s)         _INTERCEPT(_nstrdup(s))
/*  #define halloc(n,_sz)       _INTERCEPT(halloc(n,_sz))   */
/*  #define _halloc(n,_sz)      _INTERCEPT(halloc(n,_sz))   */
/*  #define hfree(p)            _INTERCEPT(hfree(p))    */
/*  #define _hfree(p)           _INTERCEPT(hfree(p))    */

#define cgets(s)            _INTERCEPTX(cgets(s),s)
#define _cgets(s)           _INTERCEPTX(_cgets(s),s)
#define memccpy(d,s,c,n)    _INTERCEPTX(memccpy(d,s,c,n),d)
#define _memccpy(d,s,c,n)   _INTERCEPTX(_memccpy(d,s,c,n),d)
#define strdup(s)           _INTERCEPT(strdup(s))
#define _strdup(s)          _INTERCEPT(_strdup(s))
#define _strnset(s,c,n)     _INTERCEPTX(_strnset(s,c,n),s)
#define strnset(s,c,n)      _INTERCEPTX(strnset(s,c,n),s)
#define strset(s,c)         _INTERCEPTX(strset(s,c),s)
#define _strset(s,c)        _INTERCEPTX(_strset(s,c),s)
#define swab(s,d,n)         _INTERCEPTX(swab(s,d,n),d)
#define _swab(s,d,n)        _INTERCEPTX(_swab(s,d,n),d)
#define tempnam(d,pfx)      _INTERCEPT(tempnam(d,pfx))
#define _tempnam(d,pfx)     _INTERCEPT(_tempnam(d,pfx))

#define _fmemcpy(d,s,n)     _INTERCEPTX(_fmemcpy(d,s,n),d)
#define _fmemmove(d,s,n)    _INTERCEPTX(_fmemmove(d,s,n),d)
#define _fmemset(d,c,n)     _INTERCEPTX(_fmemset(d,c,n),d)
#define _fmemccpy(d,s,c,n)  _INTERCEPTX(_fmemccpy(d,s,c,n),d)
#define _fstrcat(s1,s2)     _INTERCEPTX(_fstrcat(s1,s2),s1)
#define _fstrcpy(d,s)       _INTERCEPTX(_fstrcpy(d,s),d)
#define _fstrncat(s1,s2,n)  _INTERCEPTX(_fstrncat(s1,s2,n),s1)
#define _fstrncpy(d,s,n)    _INTERCEPTX(_fstrncpy(d,s,n),d)
#define _fstrdup(s)         _INTERCEPT(_fstrdup(s))
#define _fstrnset(d,c,n)    _INTERCEPTX(_fstrnset(d,c,n),d)
#define _fstrset(d,c)       _INTERCEPTX(_fstrset(d,c),d)

#define getcwd(d,n)         _INTERCEPTX(getcwd(d,n),d)
#define _getcwd(d,n)        _INTERCEPTX(getcwd(d,n),d)
#define _getdcwd(r,d,n)     _INTERCEPTX(_getdcwd(r,d,n),d)
#define _dos_setvect(_i,_h) _INTERCEPT(_dos_setvect(_i,_h))
#define _fullpath(b,p,n)    _INTERCEPTX(_fullpath(b,p,n),b)

/* ----- END Microsoft C/C++ interceptions ----- */

#elif defined (_CC_BORLAND_)    /* *** Borland C/C++ *** */

#ifndef _CC_POWERPACK32_
#define farfree(p)          _INTERCEPT(farfree(p))
#define farmalloc(s)        _INTERCEPT(farmalloc(s))
#define farcalloc(n,s)      _INTERCEPT(farcalloc(n,s))
#define farrealloc(p,s)     _INTERCEPT(farrealloc(p,s))
#endif  /* not _CC_POWERPACK32_ */

#define cgets(s)            _INTERCEPTX(cgets(s),s)
#define memccpy(d,s,c,n)    _INTERCEPTX(memccpy(d,s,c,n),d)
#if !defined(movmem)
#define movmem(s,d,l)       _INTERCEPTX(movmem(s,d,l),d)
#endif
#if !defined(setmem)
#define setmem(d,c,v)       _INTERCEPTX(setmem(d,c,v),d)
#endif
#define setvect(i,v)        _INTERCEPT(setvect(i,v))
#define stpcpy(d,s)         _INTERCEPTX(stpcpy(d,s),d)
#define _stpcpy(d,s)        _INTERCEPTX(_stpcpy(d,s),d)
#define strdup(s)           _INTERCEPT(strdup(s))
#define strnset(s,c,n)      _INTERCEPTX(strnset(s,c,n),s)
#define strset(s,c)         _INTERCEPTX(strset(s,c),s)
#define swab(s,d,n)         _INTERCEPTX(swab(s,d,n),d)
#define tempnam(d,pfx)      _INTERCEPT(tempnam(d,pfx))

#define getcwd(d,n)         _INTERCEPTX(getcwd(d,n),d)
#define _getdcwd(r,d,n)     _INTERCEPTX(_getdcwd(r,d,n),d)
#define _dos_setvect(_i,_h) _INTERCEPT(_dos_setvect(_i,_h))

#ifndef _CC_POWERPACK32_
#define _fmemcpy(d,s,n)     _INTERCEPTX(_fmemcpy(d,s,n),d)
#define _fmemmove(d,s,n)    _INTERCEPTX(_fmemmove(d,s,n),d)
#define _fmemset(d,c,n)     _INTERCEPTX(_fmemset(d,c,n),d)
#define _fmemccpy(d,s,c,n)  _INTERCEPTX(_fmemccpy(d,s,c,n),d)
#define _fmovmem(s,d,l)     _INTERCEPTX(_fmovmem(s,d,l),s)
#define _fsetmem(d,c,v)     _INTERCEPTX(_fsetmem(d,c,v),d)
#define _fstrcat(s1,s2)     _INTERCEPTX(_fstrcat(s1,s2),s1)
#define _fstrcpy(d,s)       _INTERCEPTX(_fstrcpy(d,s),d)
#define _fstrncat(s1,s2,n)  _INTERCEPTX(_fstrncat(s1,s2,n),s1)
#define _fstrncpy(d,s,n)    _INTERCEPTX(_fstrncpy(d,s,n),d)
#define _fstrdup(s)         _INTERCEPT(_fstrdup(s))
#define _fstrnset(d,c,n)    _INTERCEPTX(_fstrnset(d,c,n),d)
#define _fstrset(d,c)       _INTERCEPTX(_fstrset(d,c),d)
#endif  /* not _CC_POWERPACK32_ */

/*
#define freemem(g)          _INTERCEPT(freemem(g))
#define vsscanf(d,f,a)      _INTERCEPTX(vsscanf(d,f,a),d)
*/

/* ----- END Borland C/C++ interceptions ----- */

#else

#error Unknown compiler in MemCheck.h

#endif  /* Compiler-specific Function Mapping Section */

/*  Location Transmitters

    You can add any non-intercepted functions to
    this bunch... Just updates MemCheck's file and line
    information via mc_set_location(), which is thousands
    of times faster than anything that does I/O.
    The only time this section could be a problem is
    if the header file is included before any other header
    files which prototype these routines.

    Borland's TD (Turbo Debugger) also has problems here (see note).
*/
#ifndef _lint   /* LINT not like */

/*  Borland's Turbo Debugger gets confoosed and executes
    a `Run' instead of a `Step' when _SETLOC macro is used...
*/
#if !defined (_CC_BORLAND_)
#if 1   /* Change this to '0' to omit this section */

#define printf              _SETLOC(printf)

#define fopen               _SETLOC(fopen)
#define fprintf             _SETLOC(fprintf)
#define fread               _SETLOC(fread)
#define fwrite              _SETLOC(fwrite)
#define fclose              _SETLOC(fclose)

#define system              _SETLOC(system)
#define exec                _SETLOC(exec)
#define spawnl              _SETLOC(spawnl)
#define spawnlp             _SETLOC(spawnlp)
#define spawnle             _SETLOC(spawnle)
#define spawnlpe            _SETLOC(spawnlpe)
#define spawnv              _SETLOC(spawnv)
#define spawnvp             _SETLOC(spawnvp)
#define spawnve             _SETLOC(spawnve)
#define spawnvpe            _SETLOC(spawnvpe)

#endif  /* end location transmission section */
#endif  /* not Borland C++ */
#endif  /* not def _lint */


/* ****  THIRD-PARTY MAPPINGS  **** */

/*  Vermont Views V3.xx

    The following code will transmit the exact file
    and line of any mem_get() and mem_free() calls to
    MemCheck, so that it can report on the location where
    these functions are called, instead of the location of
    the calloc() or free().

    If you've used MCCONFIG to configure the Vermont Views source 
    code, you *must* either NOT include the MemCheck header file 
    in the MEM_GET.C and MEM_FREE.C modules, or, if you do, then
    #define NO_INTERCEPT beforehand, e.g.

         Module MEM_GET.C ... 
        :
        #define NO_INTERCEPT
        #include <memcheck.h>
        :

    MCCONFIG may be used to configure even the shrouded
    Vermont Views source code.

    See also: TechNote "Using MemCheck 3.0 Professional
        With Vermont Views", available on the StratosWare
        BBS (313) 996-2993 as VIEWS.TXT, or by fax.
*/
#if defined (VV_SYS)    /* should do the trick */
#   define mem_get(s)   _INTERCEPT(mem_get(s))
#   define mem_free(p)  _INTERCEPT(mem_free(p))
#endif


/* ****  APPLICATION-SPECIFIC MAPPINGS  **** */

/*
    If your application uses allocation "cover" routines,
    MemCheck will by default report errors and leaks by
    the file and line of the malloc or free within the
    cover module.  To get MemCheck to report by file and
    line where the cover function is actually called, follow
    the instructtions in MemCheck TechNote "Transmitting File
    and Line to MemCheck 3.0 Professional Through Cover Functions."

    This is where you can place the cover function macros.
*/


/* end APPLICATION-SPECIFIC MAPPINGS */

#endif  /* not NO_INTERCEPT */

/* Calls that xmit source file, line number if called in source */
/* *** MemCheck API file & line transmittal *** */
#define mc_startcheck(erf)      mc_startcheck(_MCSF_,_MCSL_,erf)
#define mc_stack_trace(_memo)   _INTERCEPT(mc_stack_trace(_memo))
#define mc_debug(s)             _INTERCEPT(mc_debug(s))
#define mc_debugf(arglist)      mc_debugv arglist
#define mc_debugv               _mcsl(_MCSF_,_MCSL_),mc_debugv
#define mc_endcheck()           _INTERCEPT(mc_endcheck())
#define mc_check_buffers()      _INTERCEPT(mc_check_buffers())
#define mc_check(p)             _INTERCEPT(mc_check(p))
#define mc_register(p,s)        _INTERCEPT(mc_register(p,s))
#define mc_unregister(p)        _INTERCEPT(mc_unregister(p))
#define mc_nullcheck()          _INTERCEPT(mc_nullcheck())
#define mc_report(f)            _INTERCEPT(mc_report(f))

/*lint -restore  652 Define of symbol declared prev */

#endif  /* not MEMCHECK_MODULE, function interceptions */


/* End "C" call wrapper */
#ifdef __cplusplus
}


/*  C++ MemCheck Class

    This class can be used as an alternative to
    AutoInit, or to placing the mc_startcheck() and
    mc_endcheck() calls in your main() program.
    Just declaring an object of class 'MemCheck'
    will start MemCheck up;  usually you will place
    this 'above' any other global or statically declared
    C++ objects in your main module.

    Here are some examples of starting MemCheck up
    via object mechanics:

        MemCheck On;
        MemCheck Active;
        MemCheck Rules;

    Use your imagination!  Note that if AutoInit is ON,
    any calls to mc_startcheck() and mc_endcheck() are
    ignored.
*/
#if !defined (NO_INTERCEPT) /* must not have this def'd */

/* This class def causes a warning under MSC if not used */
#if !defined (_CC_MSC_)

class MemCheck {
public:
    MemCheck ()  { mc_startcheck (NULL); }
    ~MemCheck () { mc_endcheck ();       }
};

#endif

#endif /* NO_INTERCEPT */


/* *** For use in new and delete modules only *** */
/*
    Replace 'mallocs' with 'cpp_mallocs', etc.
    In new and delete modules, NO_INTERCEPT should be #defined, e.g.

        #define NO_INTERCEPT
        #include <memcheck.h>
        :
        void * operator new ( size_t size )
        {
            if (!size)  size = 1;
            return (cpp_malloc (size));
        }
        etc.
*/
#define cpp_malloc(_s)      (_mc_set_newflag(),     malloc(_s))
#define cpp_calloc(_n,_s)   (_mc_set_newflag(),     calloc(_n,_s))
#define cpp_free(_p)        (_mc_set_delflag(),     free(_p))

/* Borland C++ */
#define cpp_farmalloc(_s)   (_mc_set_newflag(),     farmalloc(_s))
#define cpp_farfree(_fp)    (_mc_set_delflag(),     farfree(_fp))

/* Microsoft C++-compatibles */
#define cpp__fmalloc(_s)    (_mc_set_newflag(),     _fmalloc(_s))
#define cpp__ffree(_fp)     (_mc_set_delflag(),     _ffree(_fp))


/* C++ */
#if !defined (NO_INTERCEPT)
#if !defined (NO_CPP)

/*
    This method is off by default, because it
    requires definition of a new operator like:

      void * new (size_t size, char *file, int lineno);

    Such a new operator is included in your SOURCE\CPP
    directory. To have this method used for all modules,
    #define NEW_OVERLOADED at the top of this header file
    or in your project #include file, BEFORE the MemCheck
    header file is #included.

    The substitutions for the new operator
    may not work in all situations.  To disable
    MemCheck's interception of new on a module-by-
    module basis by undefining NEW_OVERLOADED.
*/
#if defined (NEW_OVERLOADED)

/*  Method 1: Placement Operators

    Use placement operators to trap file and line location transparently
    on calls to new.  
    
    Thanks for this tip to Dan Saks,
    C and C++ writer, author, teacher, and columnist--buy his books.
    He came through when no one else had a clue!

    Please consult your manual, MemCheck technotes, 
    or StratosWare Technical Support (1-800-WE-DEBUG)
    for details on how to configure your project for
    use with an overloaded new placement operator.
*/

/* Declare overloaded new with placement operators */
void *operator new (size_t _sz, char *file, int lineno);
#if defined (_CPP_ANSI20_)
/*  Array version; only under supporting compilers 
    COMMENT LINE OUT if it causes a compile error.
*/
void *operator new[] (size_t _sz, char *file, int lineno);
#endif

#if !defined (_CC_MSC_)
#define new     new((char *)__FILE__,(int)__LINE__)
#else
#define new     new(__FILE__,__LINE__)
#endif

/*  NOTE:
    This placement operator interception syntax has been
    known to cause syntax errors (in VC++ 1.0) for no apparent reason
    on statements like

            Domain *d = new Domain ();

    Workaround is to change line (sorry!) to equivalent

            Domain *d = new Domain;
*/

/* Backwards compatibility with the V2.1 C++ macros */
#ifndef NEW
#define NEW(_object)         new _object
#endif
#ifdef DELETE
#define DELETE(_object)      delete _object
#endif
#define DELETE_ARR(_arr)     delete[] _arr

#else   /* !NEW_OVERLOADED - end of Placement Operator intercept */

/*  New and Delete Interception, Method 2: NEW() and DELETE()

    The NEW() and DELETE() macros may be used to transmit file
    and line of new and delete.   These macros, which require
    modification of source code, i.e. "NEW(object)" for "new object",
    should probably be used only if the above overloaded new does
    not work for your code base.
    
    Please consult your manual, MemCheck technotes, 
    or StratosWare Technical Support (1-800-WE-DEBUG)
    for details on how to configure your project for
    use with NEW() and DELETE().

    If calling, please have your MemCheck serial number handy.
*/
#ifndef NEW
#define NEW(_object)        (_mcsl_new(_MCSF_,_MCSL_),    new _object)
#endif
#ifndef DELETE  /* WINNT.H under BC DPMI32 defines DELETE */
#define DELETE(_object)     (_mcsl_delete(_MCSF_,_MCSL_), delete _object)
#endif
#define DELETE_ARR(_arr)    (_mcsl_delete(_MCSF_,_MCSL_), delete[] _arr)

#endif  /* !NEW_OVERLOADED */

#define delete  _mcsl_delete(_MCSF_,_MCSL_), delete


/* *** FAILURES *** */

/*  These macros failed in the purpose of
    intercepting new transparently in some
    situation or other.
*/

/* Failed on " * new expr " (TV)  */
/*  #define new (mc_set_location(),0) ? NULL : new  */

/* Failed on " x = new Object " (TV) */
/*  #define new ((mc_set_location(),0) ? NULL : new)    */
/*  #define new new (mc_set_location(),0) ? NULL :  */

#endif  /* !NO_CPP */
#endif  /* NO_INTERCEPT */
#endif  /* cplusplus */
/******** End C++ ************/


#endif      /* End of Section for MEMCHECK Defined */

/* -------------------------------------------------------------------------- */

#endif  /* _MEMCHECK_H_ */


/********************************
 * End of MemCheck 3.0 Header *
 ********************************/

