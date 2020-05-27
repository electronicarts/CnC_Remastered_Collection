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

// list of 256 sines

#define MAKE_PTR(vp,x,y) (char *) (vp->Get_Offset() + ((y) * (vp->Get_Width() + vp->Get_XAdd())) + (x))
#define MAKE_PTR2(vp,x,y) (char *) (vp->Get_Offset() + (((y)<<1) * (vp->Get_Width() + vp->Get_XAdd())) + ((x)<<1))

#ifndef WIN32
class BitmapClass
{
	public:
		BitmapClass(int w, int h, unsigned char * data) :
			Width(w), Height(h), Data(data) {};

		int Width;
		int Height;
		unsigned char * Data;
};

class TPoint2D
{
	public:
		TPoint2D(int xx, int yy) : x(xx), y(yy) {};
		TPoint2D(void) : x(0), y(0) {};

		int x;
		int y;
};
#endif


long Cos256[] = {
     128,     127,     127,     127,     127,     127,     126,     126,
     125,     124,     124,     123,     122,     121,     120,     119,
     118,     116,     115,     114,     112,     111,     109,     107,
     106,     104,     102,     100,      98,      96,      94,      92,
      90,      87,      85,      83,      80,      78,      75,      73,
      70,      68,      65,      62,      59,      57,      54,      51,
      48,      45,      42,      39,      36,      33,      30,      27,
      24,      21,      18,      14,      11,       8,       5,       2,
       0,      -3,      -7,     -10,     -13,     -16,     -19,     -22,
     -25,     -28,     -31,     -35,     -38,     -41,     -44,     -46,
     -49,     -52,     -55,     -58,     -61,     -63,     -66,     -69,
     -72,     -74,     -77,     -79,     -82,     -84,     -86,     -89,
     -91,     -93,     -95,     -97,     -99,    -101,    -103,    -105,
    -107,    -108,    -110,    -112,    -113,    -114,    -116,    -117,
    -118,    -119,    -120,    -121,    -122,    -123,    -124,    -125,
    -125,    -126,    -126,    -127,    -127,    -127,    -127,    -127,
    -127,    -127,    -127,    -127,    -127,    -126,    -126,    -125,
    -125,    -124,    -123,    -122,    -121,    -120,    -119,    -118,
    -117,    -116,    -114,    -113,    -112,    -110,    -108,    -107,
    -105,    -103,    -101,     -99,     -97,     -95,     -93,     -91,
     -89,     -86,     -84,     -82,     -79,     -77,     -74,     -72,
     -69,     -66,     -64,     -61,     -58,     -55,     -52,     -49,
     -46,     -44,     -41,     -38,     -35,     -31,     -28,     -25,
     -22,     -19,     -16,     -13,     -10,      -7,      -3,       0,
       2,       5,       8,      11,      14,      18,      21,      24,
      27,      30,      33,      36,      39,      42,      45,      48,
      51,      54,      57,      59,      62,      65,      68,      70,
      73,      75,      78,      80,      83,      85,      87,      90,
      92,      94,      96,      98,     100,     102,     104,     106,
     107,     109,     111,     112,     114,     115,     116,     118,
     119,     120,     121,     122,     123,     124,     124,     125,
     126,     126,     127,     127,     127,     127,     127,     127,
};
long Sin256[] = {
       0,       3,       6,       9,      12,      15,      18,      21,
      25,      28,      31,      34,      37,      40,      43,      46,
      49,      52,      54,      57,      60,      63,      66,      68,
      71,      73,      76,      79,      81,      83,      86,      88,
      90,      92,      95,      97,      99,     101,     103,     104,
     106,     108,     110,     111,     113,     114,     115,     117,
     118,     119,     120,     121,     122,     123,     124,     125,
     125,     126,     126,     127,     127,     127,     127,     127,
     127,     127,     127,     127,     127,     126,     126,     125,
     125,     124,     123,     123,     122,     121,     120,     119,
     117,     116,     115,     113,     112,     110,     109,     107,
     105,     104,     102,     100,      98,      96,      94,      91,
      89,      87,      85,      82,      80,      77,      75,      72,
      70,      67,      64,      61,      59,      56,      53,      50,
      47,      44,      41,      38,      35,      32,      29,      26,
      23,      20,      17,      14,      11,       7,       4,       1,
      -1,      -4,      -7,     -11,     -14,     -17,     -20,     -23,
     -26,     -29,     -32,     -35,     -38,     -41,     -44,     -47,
     -50,     -53,     -56,     -59,     -61,     -64,     -67,     -70,
     -72,     -75,     -77,     -80,     -82,     -85,     -87,     -89,
     -91,     -94,     -96,     -98,    -100,    -102,    -104,    -105,
    -107,    -109,    -110,    -112,    -113,    -115,    -116,    -117,
    -119,    -120,    -121,    -122,    -123,    -123,    -124,    -125,
    -125,    -126,    -126,    -127,    -127,    -127,    -127,    -127,
    -127,    -127,    -127,    -127,    -127,    -126,    -126,    -125,
    -125,    -124,    -123,    -122,    -121,    -120,    -119,    -118,
    -117,    -115,    -114,    -113,    -111,    -110,    -108,    -106,
    -104,    -103,    -101,     -99,     -97,     -95,     -92,     -90,
     -88,     -86,     -83,     -81,     -79,     -76,     -73,     -71,
     -68,     -66,     -63,     -60,     -57,     -54,     -52,     -49,
     -46,     -43,     -40,     -37,     -34,     -31,     -28,     -25,
     -21,     -18,     -15,     -12,      -9,      -6,      -3,       0,
};
