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

//	Bigcheck.h
//	ajw 9/14/98

#ifdef WOLAPI_INTEGRATION

#ifndef BIGCHECKBOX_H
#define BIGCHECKBOX_H

#include	"toggle.h"

#define BIGCHECK_OFFSETX	20
#define BIGCHECK_OFFSETY	0

//***********************************************************************************************
class BigCheckBoxClass : public ToggleClass
{
public:
	BigCheckBoxClass( unsigned id, int x, int y, int w, int h, const char* szCaptionIn, TextPrintType TextFlags, 
							bool bInitiallyChecked = false ) :
		ToggleClass( id, x, y, w, h ),
		TextFlags( TextFlags )
	{
		szCaption = new char[ strlen( szCaptionIn ) + 1 ];
		strcpy( szCaption, szCaptionIn );
		if( bInitiallyChecked )
			Turn_On();
		IsToggleType = 1;
	}
	virtual ~BigCheckBoxClass()
	{
		delete [] szCaption;
	}

	virtual int Draw_Me(int forced=false);
	virtual int Action(unsigned flags, KeyNumType & key);

	bool	Toggle()
	{
		if( IsOn )
		{
			Turn_Off();
			return false;
		}
		Turn_On();
		return true;
	}

protected:
	TextPrintType	TextFlags;
	char*			szCaption;

};

#endif

#endif
