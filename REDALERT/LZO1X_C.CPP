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

/* lzo1x_c.c -- standalone LZO1X-1 compressor

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


#include "lzo1x.h"
#ifndef NDEBUG
#define NDEBUG
#endif
#include <assert.h>
#include "lzo_conf.h"

#if !defined(LZO1X) && !defined(LZO1Y)
#  define LZO1X
#endif


/***********************************************************************
//
************************************************************************/

#define M1_MAX_OFFSET	0x0400
#if defined(LZO1X)
#define M2_MAX_OFFSET	0x0800
#elif defined(LZO1Y)
#define M2_MAX_OFFSET	0x0400
#endif
#define M3_MAX_OFFSET	0x4000
#define M4_MAX_OFFSET	0xbfff

#define MX_MAX_OFFSET	(M1_MAX_OFFSET + M2_MAX_OFFSET)

#define M1_MARKER		0
#define M2_MARKER		64
#define M3_MARKER		32
#define M4_MARKER		16


#define _DV2(p,shift1,shift2) \
		(((( (lzo_uint)(p[2]) << shift1) ^ p[1]) << shift2) ^ p[0])
#define DVAL_NEXT(dv,p) \
		dv ^= p[-1]; dv = (((dv) >> 5) ^ ((lzo_uint)(p[2]) << (2*5)))
#define _DV(p,shift) 		_DV2(p,shift,shift)
#define DVAL_FIRST(dv,p)	dv = _DV((p),5)
#define _DINDEX(dv,p)		((40799u * (dv)) >> 5)
#define DINDEX(dv,p)		(((_DINDEX(dv,p)) & 0x3fff) << 0)
#define UPDATE_D(dict,cycle,dv,p)		dict[ DINDEX(dv,p) ] = (p)
#define UPDATE_I(dict,cycle,index,p)	dict[index] = (p)


/***********************************************************************
// compress a block of data.
************************************************************************/

static int do_compress(const lzo_byte *in , lzo_uint  in_len,
	lzo_byte *out, lzo_uint *out_len,
	lzo_voidp wrkmem )
{
	register const lzo_byte *ip;
	lzo_uint dv;
	lzo_byte *op;
	const lzo_byte * const in_end = in + in_len;
	const lzo_byte * const ip_end = in + in_len - 9 - 4;
	const lzo_byte *ii;
	const lzo_bytepp const dict = (const lzo_bytepp) wrkmem;

	op = out;
	ip = in;
	ii = ip;

	DVAL_FIRST(dv,ip); UPDATE_D(dict,cycle,dv,ip); ip++;
	DVAL_NEXT(dv,ip);  UPDATE_D(dict,cycle,dv,ip); ip++;
	DVAL_NEXT(dv,ip);  UPDATE_D(dict,cycle,dv,ip); ip++;
	DVAL_NEXT(dv,ip);  UPDATE_D(dict,cycle,dv,ip); ip++;

	while (1) {
		register const lzo_byte *m_pos;
		lzo_uint m_len;
		lzo_ptrdiff_t m_off;
		lzo_uint lit;

		lzo_uint dindex = DINDEX(dv,ip);
		m_pos = dict[dindex];
		UPDATE_I(dict,cycle,dindex,ip);


		if (LZO_CHECK_MPOS_NON_DET(m_pos,m_off,in,ip,M4_MAX_OFFSET)) {
		}
#if defined(LZO_UNALIGNED_OK_2)
		else
			if (* (unsigned short *) m_pos != * (unsigned short *) ip)
#else
		else
			if (m_pos[0] != ip[0] || m_pos[1] != ip[1])
#endif
		{
		} else {
			if (m_pos[2] == ip[2]) {
				lit = ip - ii;
				m_pos += 3;
				if (m_off <= M2_MAX_OFFSET)
					goto match;

				/* better compression, but slower */
				if (lit == 3) {
					assert(op - 2 > out); op[-2] |= LZO_BYTE(3);
					*op++ = *ii++; *op++ = *ii++; *op++ = *ii++;
					goto code_match;
				}

				if (*m_pos == ip[3]) {
					goto match;
				}
			} else {
				/* still need a better way for finding M1 matches */
			}
		}


	/* a literal */
		++ip;
		if (ip >= ip_end) {
			break;
		}
		DVAL_NEXT(dv,ip);
		continue;


	/* a match */

match:

		/* store current literal run */
		if (lit > 0) {
			register lzo_uint t = lit;

			if (t <= 3) {
				assert(op - 2 > out);
				op[-2] |= LZO_BYTE(t);
			} else {
				if (t <= 18) {
					*op++ = LZO_BYTE(t - 3);
				} else {
					register lzo_uint tt = t - 18;

					*op++ = 0;
					while (tt > 255) {
						tt -= 255;
						*op++ = 0;
					}
					assert(tt > 0);
					*op++ = LZO_BYTE(tt);
				}
			}

			do {
				*op++ = *ii++;
			} while (--t > 0);
		}


		/* code the match */
code_match:
		assert(ii == ip);
		ip += 3;
		if (*m_pos++ != *ip++ || *m_pos++ != *ip++ || *m_pos++ != *ip++ ||
		    *m_pos++ != *ip++ || *m_pos++ != *ip++ || *m_pos++ != *ip++)
		{
			--ip;
			m_len = ip - ii;
			assert(m_len >= 3); assert(m_len <= 8);

			if (m_off <= M2_MAX_OFFSET) {
				m_off -= 1;
				*op++ = LZO_BYTE(((m_len - 1) << 5) | ((m_off & 7) << 2));
				*op++ = LZO_BYTE(m_off >> 3);
			} else {
				if (m_off <= M3_MAX_OFFSET) {
					m_off -= 1;
					*op++ = LZO_BYTE(M3_MARKER | (m_len - 2));
					goto m3_m4_offset;
				} else {
					m_off -= 0x4000;
					assert(m_off > 0); assert(m_off <= 0x7fff);
					*op++ = LZO_BYTE(M4_MARKER |
					                 ((m_off & 0x4000) >> 11) | (m_len - 2));
					goto m3_m4_offset;
				}
			}
		} else {
			const lzo_byte *end;
			end = in_end;
			while (ip < end && *m_pos == *ip) {
				m_pos++;
				ip++;
			}
			m_len = (ip - ii);
			assert(m_len >= 3);

			if (m_off <= M3_MAX_OFFSET) {
				m_off -= 1;
				if (m_len <= 33) {
					*op++ = LZO_BYTE(M3_MARKER | (m_len - 2));
				} else {
					m_len -= 33;
					*op++ = M3_MARKER | 0;
					goto m3_m4_len;
				}
			} else {
				m_off -= 0x4000;
				assert(m_off > 0); assert(m_off <= 0x7fff);
				if (m_len <= 9) {
					*op++ = LZO_BYTE(M4_MARKER |
					                 ((m_off & 0x4000) >> 11) | (m_len - 2));
				} else {
					m_len -= 9;
					*op++ = LZO_BYTE(M4_MARKER | ((m_off & 0x4000) >> 11));
m3_m4_len:
					while (m_len > 255) {
						m_len -= 255;
						*op++ = 0;
					}
					assert(m_len > 0);
					*op++ = LZO_BYTE(m_len);
				}
			}

m3_m4_offset:
			*op++ = LZO_BYTE((m_off & 63) << 2);
			*op++ = LZO_BYTE(m_off >> 6);
		}

		ii = ip;
		if (ip >= ip_end) {
			break;
		}
		DVAL_FIRST(dv,ip);
	}

	/* store final literal run */
	if (in_end - ii > 0) {
		register lzo_uint t = in_end - ii;

		if (op == out && t <= 238) {
			*op++ = LZO_BYTE(17 + t);
		} else {
			if (t <= 3) {
				op[-2] |= LZO_BYTE(t);
			} else {
				if (t <= 18) {
					*op++ = LZO_BYTE(t - 3);
				} else {
					register lzo_uint tt = t - 18;

					*op++ = 0;
					while (tt > 255) {
						tt -= 255;
						*op++ = 0;
					}
					assert(tt > 0);
					*op++ = LZO_BYTE(tt);
				}
			}
		}
		do {
			*op++ = *ii++;
		} while (--t > 0);
	}

	*out_len = op - out;
	return LZO_E_OK;
}


/***********************************************************************
// public entry point
************************************************************************/

int lzo1x_1_compress     ( const lzo_byte *in , lzo_uint  in_len,
                                 lzo_byte *out, lzo_uint *out_len,
                                 lzo_voidp wrkmem )
{
	lzo_byte *op = out;
	int r = LZO_E_OK;

	if (in_len <= 0)
		*out_len = 0;
	else if (in_len <= 9 + 4)
	{
		*op++ = LZO_BYTE(17 + in_len);
		do *op++ = *in++; while (--in_len > 0);
		*out_len = op - out;
	}
	else
		r = do_compress(in,in_len,out,out_len,wrkmem);

	if (r == LZO_E_OK)
	{
		op = out + *out_len;
		*op++ = M4_MARKER | 1;
		*op++ = 0;
		*op++ = 0;
		*out_len += 3;
	}

	return r;
}


/*
vi:ts=4
*/
