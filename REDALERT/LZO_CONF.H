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

/* lzo_conf.h -- main internal configuration file for the the LZO library

   This file is part of the LZO real-time data compression library.

   Copyright (C) 1996 Markus Franz Xaver Johannes Oberhumer

   The LZO library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   The LZO library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the LZO library; see the file COPYING.LIB.
   If not, write to the Free Software Foundation, Inc.,
   675 Mass Ave, Cambridge, MA 02139, USA.

   Markus F.X.J. Oberhumer
   markus.oberhumer@jk.uni-linz.ac.at
 */


/* WARNING: this file should *not* be used by applications. It is
   part of the implementation of the library and is subject
   to change.
 */


#ifndef __LZO_CONF_H
#define __LZO_CONF_H

#ifndef __LZOCONF_H
#  include <lzoconf.h>
#endif


/***********************************************************************
// compiler specific defines
************************************************************************/

/* need Borland C 4.0 or above because of huge-pointer bugs */
#if defined(__LZO_MSDOS16) && defined(__TURBOC__)
#  if (__TURBOC__ < 0x452)
#    error You need a newer compiler version
#  endif
#endif

#if defined(__LZO_MSDOS) || defined(__i386__) || defined(__386__)
#  if !defined(__LZO_i386)
#    define __LZO_i386
#  endif
#endif


/***********************************************************************
//
************************************************************************/

#include <stddef.h>			/* ptrdiff_t, size_t */
#include <string.h>			/* memcpy, memmove, memcmp, memset */

#if 0 && !defined(assert)
#  error <assert.h> not included
#endif

#if defined(__BOUNDS_CHECKING_ON)
#  include <unchecked.h>
#else
#  define BOUNDS_CHECKING_OFF_DURING(stmt)		stmt
#  define BOUNDS_CHECKING_OFF_IN_EXPR(expr)		(expr)
#endif

/* ptrdiff_t */
#if (UINT_MAX >= 0xffffffffL)
   typedef ptrdiff_t        lzo_ptrdiff_t;
#else
   typedef long             lzo_ptrdiff_t;
#endif


#ifdef __cplusplus
#  define LZO_UNUSED(parm)
#else
#  define LZO_UNUSED(parm)	parm
#endif


#if !defined(__inline__) && !defined(__GNUC__)
#  if defined(__cplusplus)
#    define __inline__		inline
#  else
#    define __inline__		/* nothing */
#  endif
#endif


/***********************************************************************
// compiler and architecture specific stuff
************************************************************************/

/* Some defines that indicate if memory can be accessed at unaligned
 * addresses. You should also test that this is actually faster if
 * it is allowed by your system.
 */

#if 1 && defined(__LZO_i386)
#  if !defined(LZO_UNALIGNED_OK_2)
#    define LZO_UNALIGNED_OK_2
#  endif
#  if !defined(LZO_UNALIGNED_OK_4)
#    define LZO_UNALIGNED_OK_4
#  endif
#endif


#if defined(LZO_UNALIGNED_OK_2) || defined(LZO_UNALIGNED_OK_4)
#  if !defined(LZO_UNALIGNED_OK)
#    define LZO_UNALIGNED_OK
#  endif
#endif


/* Definitions for byte order, according to significance of bytes, from low
 * addresses to high addresses. The value is what you get by putting '4'
 * in the most significant byte, '3' in the second most significant byte,
 * '2' in the second least significant byte, and '1' in the least
 * significant byte.
 */

#define	LZO_LITTLE_ENDIAN		1234
#define	LZO_BIG_ENDIAN			4321
#define	LZO_PDP_ENDIAN			3412

/* The byte order is only needed if we use LZO_UNALIGNED_OK */
#if !defined(LZO_BYTE_ORDER)
#  if defined(__LZO_i386)
#    define LZO_BYTE_ORDER		LZO_LITTLE_ENDIAN
#  elif defined(__mc68000__)
#    define LZO_BYTE_ORDER		LZO_BIG_ENDIAN
#  elif defined(__BYTE_ORDER)
#    define LZO_BYTE_ORDER		__BYTE_ORDER
#  endif
#endif

#if defined(LZO_UNALIGNED_OK)
#  if !defined(LZO_BYTE_ORDER)
#    error LZO_BYTE_ORDER is not defined
#  elif (LZO_BYTE_ORDER != LZO_LITTLE_ENDIAN) && \
        (LZO_BYTE_ORDER != LZO_BIG_ENDIAN)
#    error invalid LZO_BYTE_ORDER
#  endif
#endif


/***********************************************************************
// optimization
************************************************************************/

/* gcc 2.6.3 and gcc 2.7.2 have a bug */
#define LZO_OPTIMIZE_GNUC_i386_IS_BUGGY

/* Help the optimizer with register allocation.
 * Don't activate this macro for a fair comparision with other algorithms.
 */
#if 1 && defined(NDEBUG) && !defined(__BOUNDS_CHECKING_ON)
#  if defined(__GNUC__) && defined(__i386__)
#    if !defined(LZO_OPTIMIZE_GNUC_i386_IS_BUGGY)
#      define LZO_OPTIMIZE_GNUC_i386
#    endif
#  endif
#endif


/***********************************************************************
//
************************************************************************/

#define LZO_BYTE(x)			((unsigned char) (x))

#define LZO_MAX(a,b)		((a) >= (b) ? (a) : (b))
#define LZO_MIN(a,b)		((a) <= (b) ? (a) : (b))

#define lzo_sizeof(x)		((lzo_uint) (sizeof(x)))

#define LZO_HIGH(x)			((lzo_uint) (sizeof(x)/sizeof(*(x))))

/* this always fits into 16 bits */
#define LZO_SIZE(bits)		(1u << (bits))
#define LZO_MASK(bits)		(LZO_SIZE(bits) - 1)

#define LZO_LSIZE(bits)		(1ul << (bits))
#define LZO_LMASK(bits)		(LZO_LSIZE(bits) - 1)

#define LZO_USIZE(bits)		((lzo_uint) 1 << (bits))
#define LZO_UMASK(bits)		(LZO_USIZE(bits) - 1)


/***********************************************************************
// ANSI C preprocessor macros
************************************************************************/

#define _LZO_STRINGIZE(x)			#x
#define _LZO_MEXPAND(x)				_LZO_STRINGIZE(x)

/* concatenate */
#define _LZO_CONCAT2(a,b)			a ## b
#define _LZO_CONCAT3(a,b,c)			a ## b ## c
#define _LZO_CONCAT4(a,b,c,d)		a ## b ## c ## d
#define _LZO_CONCAT5(a,b,c,d,e)		a ## b ## c ## d ## e

/* expand and concatenate (by using one level of indirection) */
#define _LZO_ECONCAT2(a,b)			_LZO_CONCAT2(a,b)
#define _LZO_ECONCAT3(a,b,c)		_LZO_CONCAT3(a,b,c)
#define _LZO_ECONCAT4(a,b,c,d)		_LZO_CONCAT4(a,b,c,d)
#define _LZO_ECONCAT5(a,b,c,d,e)	_LZO_CONCAT5(a,b,c,d,e)


/***********************************************************************
//
************************************************************************/

/* Generate compressed data in a deterministic way.
 * This is fully portable, and compression can be faster as well.
 * A reason NOT to be deterministic is when the block size is
 * very small (e.g. 8kB) or the dictionary is big, because
 * then the initialization of the dictionary becomes a relevant 
 * magnitude for compression speed.
 */
#define LZO_DETERMINISTIC


/***********************************************************************
//
************************************************************************/

#if 0 
/* This line causes problems on some architectures */
#define LZO_CHECK_MPOS_DET(m_pos,m_off,in,ip,max_offset) \
	(BOUNDS_CHECKING_OFF_IN_EXPR( \
		(m_off = ip - m_pos) > max_offset ))

#else
/* This is the safe (but slower) version */
#define LZO_CHECK_MPOS_DET(m_pos,m_off,in,ip,max_offset) \
		(m_pos == NULL || (m_off = ip - m_pos) > max_offset)
#endif


/* m_pos may point anywhere...
 * This marco is probably a good candidate for architecture specific problems.
 * Try casting the pointers to lzo_ptr_t before comparing them.
 */
#define LZO_CHECK_MPOS_NON_DET(m_pos,m_off,in,ip,max_offset) \
	(BOUNDS_CHECKING_OFF_IN_EXPR( \
		(m_pos < in || (m_off = ip - m_pos) <= 0 || m_off > max_offset) ))



#endif /* already included */

/*
vi:ts=4
*/

