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

//	ToolTip.cpp
#if (0)//PG
#include "function.h"
#include "ToolTip.h"
#include "IconList.h"

//#include "WolDebug.h"

bool SaveSurfaceRect( int xRect, int yRect, int wRect, int hRect, char* pBits, WindowNumberType window );
bool RestoreSurfaceRect( int xRect, int yRect, int wRect, int hRect, const char* pBits, WindowNumberType window );

//***********************************************************************************************
ToolTipClass::ToolTipClass( GadgetClass* pGadget, const char* szText, int xShow, int yShow, 
								bool bRightAlign /* = false */, bool bIconList /*= false */ )
	: pGadget( pGadget ), xShow( xShow ), yShow( yShow ), next( NULL ), bShowing( false ), bIconList( bIconList ),
		bRightAlign( bRightAlign )

{
	if( szText )
	{
		if( strlen( szText ) > TOOLTIPTEXT_MAX_LEN )
			strcpy( szTip, "Tooltip too long!" );
		else
			strcpy( szTip, szText );
	}
	else
		*szTip = 0;

	Set_Font( TypeFontPtr );
	Fancy_Text_Print( TXT_NONE, 0, 0, TBLACK, TBLACK, TPF_TYPE );	//	Required before String_Pixel_Width() call, for god's sake.
	wShow = String_Pixel_Width( szTip ) + 2;
	hShow = 11;

	if( !bIconList )
	{
		pSaveRect = new char[ wShow * hShow ];		//	Else it is reallocated on every draw.
		if( bRightAlign )
			this->xShow -= wShow;
	}
	else
		pSaveRect = NULL;

	//	bIconList is true if tooltips appear for individual line items in an iconlist.
	//	szText in this case is ignored.
	//	yShow is the y position of the top row's tooltip - other rows will be offset from here.
}

//***********************************************************************************************
ToolTipClass* ToolTipClass::GetToolTipHit()
{
	//	Returns 'this' if the mouse is over gadget bound to tooltip.
	//	Otherwise calls the same function in the next tooltip in the list of which *this is a part.
	if( bGadgetHit() )
		return this;
	else if( next )
		return next->GetToolTipHit();
	else
		return NULL;
}

//***********************************************************************************************
bool ToolTipClass::bGadgetHit()
{
	//	Returns true if the mouse is currently over the gadget to which *this is bound.
	int x = Get_Mouse_X();
	int y = Get_Mouse_Y();
	return ( x > pGadget->X && x < pGadget->X + pGadget->Width && y > pGadget->Y && y < pGadget->Y + pGadget->Height );
}

//***********************************************************************************************
void ToolTipClass::Move( int xShow, int yShow )
{
	bool bRestoreShow = false;
	if( bShowing )
	{
		bRestoreShow = true;
		Unshow();
	}
	this->xShow = xShow;
	if( !bIconList )
	{
		if( bRightAlign )
			this->xShow -= wShow;
	}
	this->yShow = yShow;
	if( bRestoreShow )
		Show();
}

//***********************************************************************************************
void ToolTipClass::Show()
{
	if( !bShowing )
	{
		Set_Font( TypeFontPtr );
		int xShowUse = xShow, yShowUse, wShowUse;
		const char* szTipUse;
		if( !bIconList )
		{
			yShowUse = yShow;
			wShowUse = wShow;
			szTipUse = szTip;
		}
		else
		{
			IconListClass* pIconList = (IconListClass*)pGadget;
			iLastIconListIndex = pIconList->IndexUnderMouse();
			if( iLastIconListIndex < 0 )
			{
				//	Nothing to show.
				bShowing = true;
				return;
			}
			yShowUse = pIconList->OffsetToIndex( iLastIconListIndex, yShow );
			szTipUse = pIconList->Get_Item_Help( iLastIconListIndex );
			if( !szTipUse || *szTipUse == 0 )
			{
				//	Nothing to show.
				bShowing = true;
				bLastShowNoText = true;
				return;
			}
			Fancy_Text_Print( TXT_NONE, 0, 0, TBLACK, TBLACK, TPF_TYPE );	//	Required before String_Pixel_Width() call, for god's sake.
			wShowUse = String_Pixel_Width( szTipUse ) + 2;
			if( bRightAlign )
				xShowUse -= wShowUse;
			delete [] pSaveRect;
			pSaveRect = new char[ wShowUse * hShow ];
			bLastShowNoText = false;
			xLastShow = xShowUse;
			yLastShow = yShowUse;
			wLastShow = wShowUse;
		}

		//	Save rect about to be corrupted.
		Hide_Mouse();
		SaveSurfaceRect( xShowUse, yShowUse, wShowUse, hShow, pSaveRect, WINDOW_MAIN );
		//	Draw text.
		//Simple_Text_Print( szTipUse, xShowUse, yShowUse, GadgetClass::Get_Color_Scheme(), ColorRemaps[ PCOLOR_BROWN ].Color, TPF_TYPE ); //TPF_DROPSHADOW );
		Simple_Text_Print( szTipUse, xShowUse, yShowUse, GadgetClass::Get_Color_Scheme(), BLACK, TPF_TYPE ); //TPF_DROPSHADOW );
		//	Draw bounding rect.
//		LogicPage->Draw_Rect( xShowUse, yShowUse, xShowUse + wShowUse - 1, yShowUse + hShow - 1, ColorRemaps[ PCOLOR_GOLD ].Color );
		Draw_Box( xShowUse, yShowUse, wShowUse, hShow, BOXSTYLE_BOX, false );
		Show_Mouse();
		bShowing = true;
	}
}

//***********************************************************************************************
void ToolTipClass::Unshow()
{
	if( bShowing )
	{
		int xShowUse, yShowUse, wShowUse;
		if( !bIconList )
		{
			xShowUse = xShow;
			wShowUse = wShow;
			yShowUse = yShow;
		}
		else
		{
			if( iLastIconListIndex == -1 || bLastShowNoText )
			{
				//	Nothing to restore.
				bShowing = false;
				return;
			}
			//	(Can't rely on iconlist being the same as when Show() occurred.)
//			IconListClass* pIconList = (IconListClass*)pGadget;
//			yShowUse = pIconList->OffsetToIndex( iLastIconListIndex, yShow );
//			const char* szTipUsed = pIconList->Get_Item_Help( iLastIconListIndex );
//			if( !szTipUsed || *szTipUsed == 0 )
//			{
//				//	Nothing to restore.
//				bShowing = false;
//				return;
//			}
//			Fancy_Text_Print( TXT_NONE, 0, 0, TBLACK, TBLACK, TPF_TYPE );	//	Required before String_Pixel_Width() call, for god's sake.
//			wShowUse = String_Pixel_Width( szTipUsed ) + 2;
//			if( bRightAlign )
//				xShowUse -= wShowUse;
			xShowUse = xLastShow;
			yShowUse = yLastShow;
			wShowUse = wLastShow;
		}
		Hide_Mouse();
		RestoreSurfaceRect( xShowUse, yShowUse, wShowUse, hShow, pSaveRect, WINDOW_MAIN );
		Show_Mouse();
		bShowing = false;
	}
}

//***********************************************************************************************
bool ToolTipClass::bOverDifferentLine() const
{
	//	bIconList must be true if this is being used.
	//	Returns true if the iconlist line that the mouse is over is different than the last time Show() was called.
	return ( ((IconListClass*)pGadget)->IndexUnderMouse() != iLastIconListIndex );
}

//***********************************************************************************************
bool SaveSurfaceRect( int xRect, int yRect, int wRect, int hRect, char* pBits, WindowNumberType window )
{
	//	Saves a rect of the LogicPage DirectDraw surface to pBits.
//	if( wRect * hRect > iBufferSize )
//	{
//		debugprint( "SaveSurfaceRect failed.\n" );
//		return false;
//	}

	GraphicViewPortClass draw_window(	LogicPage->Get_Graphic_Buffer(),
										WindowList[window][WINDOWX] + LogicPage->Get_XPos(),
										WindowList[window][WINDOWY] + LogicPage->Get_YPos(),
										WindowList[window][WINDOWWIDTH],
										WindowList[window][WINDOWHEIGHT] );
	if( draw_window.Lock() )
	{
		int iPitchSurf = draw_window.Get_Pitch() + draw_window.Get_Width();	//	Meaning of "Pitch" in this class seems to mean the eol skip.
		const char* pLineSurf = (char*)draw_window.Get_Offset() + xRect + yRect * iPitchSurf;
		char* pLineSave = pBits;

		//	ajw - Should copy DWORDs here instead for speed.
		for( int y = 0; y != hRect; y++ )
		{
			const char* pSurf = pLineSurf;
			char* pSave = pLineSave;
			for( int x = 0; x != wRect; x++ )
				*pSave++ = *pSurf++;

			pLineSurf += iPitchSurf;
			pLineSave += wRect;
		}
		draw_window.Unlock();
		return true;
	}
	else
	{
//		debugprint( "SaveSurfaceRect Could not lock surface.\n" );
		return false;
	}
}

//***********************************************************************************************
bool RestoreSurfaceRect( int xRect, int yRect, int wRect, int hRect, const char* pBits, WindowNumberType window )
{
	//	Copies a saved rect of bits back to the LogicPage DD surface.
	GraphicViewPortClass draw_window(	LogicPage->Get_Graphic_Buffer(),
										WindowList[window][WINDOWX] + LogicPage->Get_XPos(),
										WindowList[window][WINDOWY] + LogicPage->Get_YPos(),
										WindowList[window][WINDOWWIDTH],
										WindowList[window][WINDOWHEIGHT] );
	if( draw_window.Lock() )
	{
		int iPitchSurf = draw_window.Get_Pitch() + draw_window.Get_Width();	//	Meaning of "Pitch" in this class seems to mean the eol skip.
		char* pLineSurf = (char*)draw_window.Get_Offset() + xRect + yRect * iPitchSurf;
		const char* pLineSave = pBits;

		//	ajw - Should copy DWORDs here instead for speed.
		for( int y = 0; y != hRect; y++ )
		{
			char* pSurf = pLineSurf;
			const char* pSave = pLineSave;
			for( int x = 0; x != wRect; x++ )
				*pSurf++ = *pSave++;

			pLineSurf += iPitchSurf;
			pLineSave += wRect;
		}
		draw_window.Unlock();
		return true;
	}
	else
	{
//		debugprint( "RestoreSurfaceRect Could not lock surface.\n" );
		return false;
	}
}
#endif