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

#ifdef WOLAPI_INTEGRATION

/***************************************************************************
 * WOLEditClass -- Derived from EditClass, includes changes I wanted for
 *                 wolapi integration stuff.
 *
 * HISTORY:    07/17/1998 ajw : Created.
 *=========================================================================*/

#include "Function.h"

class WOLEditClass : public EditClass
{
public:
	WOLEditClass(int id, char * text, int max_len, TextPrintType flags, int x, int y, int w, int h, EditStyle style) :
		EditClass( id, text, max_len, flags, x, y, w, h, style )	{}
		
	virtual int Action (unsigned flags, KeyNumType &key);		//	Override of base

protected:
	virtual void Draw_Text( char const * text );				//	Override of base

};

#endif
