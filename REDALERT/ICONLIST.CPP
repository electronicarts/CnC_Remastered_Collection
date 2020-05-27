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

//	Iconlist.cpp - created by ajw 07/07/98

//	IconListClass is ListClass plus the option to include an icon on each line entry,
//	the option to have the class maintain its own copies of strings passed to it
//	for display, and the option to limit the maximum number of these strings that are
//	kept (entries are removed from the top when this maximum is reached).
//	Also added: multiple item selection capability. Note that the old selection code
//	runs as normal, but it simply not used when it comes time to display.
//	Also added: if mem. allocation is being done by this, the ability to break new items
//	into multiple lines of text is enabled.
//	Also added: extra data can be invisibly stored with each item, if memory allocation is
//	being done by this.
//	Extra data included 3 item preceding icons, 1 fixed position icon, an extra string,
//	an extra void pointer, and a color remapping value.

#include "iconlist.h"
#include "dibapi.h"

int Format_Window_String_New( const char* string, int maxlinelen, int& width, int& height, char* szReturn, int iExtraChars );
void CC_Draw_DIB( const char* pDIB, int xDest, int yDest, int iWidth, WindowNumberType window );

//***********************************************************************************************
IconListClass::IconListClass( int id, int x, int y, int w, int h, TextPrintType flags, void const * up, void const * down, 
								bool bResponsibleForStringAlloc, int iSelectionType, int iMaxItemsSaved ) :
	ListClass( id, x, y, w, h, flags, up, down )
{
	//	If bResponsibleForStringAlloc, COPIES of strings are stored in the list. Deletion is
	//	handled by this class. Icons are different - the caller is responsible for what's on
	//	the other end of the pointer.
	bDoAlloc = bResponsibleForStringAlloc;
	//	iSelectionType = 0 for no selection shown, 1 for normal ListClass selection, 2 for n multiple selections
	if( iSelectionType < 0 || iSelectionType > 2 )		iSelectionType = 1;
	iSelectType = iSelectionType;
	//	If iMaxItemsSaved is 0, there is no limit to the number of text lines. The list can grow forever.
	//	Otherwise items are deleted from the head of the list when the maximum is passed.
	//	iMaxItemsSaved only applies when bResponsibleForStringAlloc.
	iMaxItems = iMaxItemsSaved;
}

//***********************************************************************************************
IconListClass::~IconListClass( void )
{
	//	Delete the IconList_ItemExtras structs created to hold extra info on each item.
	for( int i = 0; i < ExtrasList.Count(); i++ )
		delete (IconList_ItemExtras*)ExtrasList[ i ];

	if( bDoAlloc )
	{
		//	Delete all alloc'ed strings.
		for( int i = 0; i < List.Count(); i++ )
			delete [] (char*)List[i];
	}
}

/***********************************************************************************************
 * IconListClass::Add_Item -- Adds an item to the list box.                                        *
 *                                                                                             *
 *    This will add the specified string to the list box. The string is added to the end       *
 *    of the list.                                                                             *
 *                                                                                             *
 * INPUT:      text  -- Pointer to the string to add to the list box.                          *
 *             pIcon -- Pointer to the shape to add.
 *             IconKind -- Indicates what type of image pIcon points to.
 *             szExtraDataString -- Extra string data that gets copied and stored along with item.
 *             szExtraDataPtr -- Extra data that gets stored along with item.
 *             pColorRemap -- Points to a color remapping used when drawing the item.
 *
 * OUTPUT:     Returns new item index.                                                         *
 * WARNINGS:   none                                                                            *
 * HISTORY:    07/07/1998 ajw : Created.                                                       *
 *=============================================================================================*/
int IconListClass::Add_Item(char const * text)
{
	return Add_Item( text, NULL, NULL, ICON_SHAPE );
}

int IconListClass::Add_Item( const char* text, const char* szHelp, 
								void* pIcon0, ICONKIND IconKind0, const char* szExtraDataString /* = NULL */, 
								void* pvExtraDataPtr /* = NULL */, RemapControlType* pColorRemap /* = NULL */,
								void* pIcon1 /* = NULL */, ICONKIND IconKind1 /* = ICON_SHAPE */, 
								void* pIcon2 /* = NULL */, ICONKIND IconKind2 /* = ICON_SHAPE */,
								void* pFixedIcon /* = NULL */, ICONKIND FixedIconKind /* = ICON_SHAPE */, int iXFixedIcon /* = 0 */, int iYFixedIcon /* = 0 */, int iFixedIconWidth /* = -1 */ )
{
	if( text )
	{
		if( bDoAlloc )
		{
			int iRetVal;

			char* szText = new char[ strlen( text ) + 51 ];			//	50 extra chars added for line breaks later.
			strcpy( szText, text );

			int iWidthMax, iHeight;
			//	Stupid usage of globals for font stuff... <grumble>
			if( TextFlags == TPF_TYPE )
			{
				void* pFontBefore = Set_Font( TypeFontPtr );
				DWORD FontXSpacingBefore = FontXSpacing;
				FontXSpacing = -2;

				int iWidthToClipAt = IsScrollActive ? Width : Width - UpGadget.Width;
				//	This call will place '\r's in the string where line breaks should occur.
				Format_Window_String_New( text, iWidthToClipAt, iWidthMax, iHeight, szText, 50 );

				Set_Font( pFontBefore );
				FontXSpacing = FontXSpacingBefore;		//	Just in case it matters... Doubt it.
			}
			else
			{
				//	Currently never called. Test well if you use IconList with a font other than TPF_TYPE,
				//	as the character spacing globals get set weirdly, I've found.
				int iWidthToClipAt = IsScrollActive ? Width : Width - UpGadget.Width;
				//	This call will place '\r's in the string where line breaks should occur.
				Format_Window_String_New( text, iWidthToClipAt, iWidthMax, iHeight, szText, 50 );
			}

			//	Each break character causes a line to be added to list.
			char szBreakchars[] = "\r\n\v\f";
			char* szToken;
			char* szNextChar = szText;
			szToken = strtok( szText, szBreakchars );
			while( szToken )
			{
				while( szNextChar < szToken )
				{
					//	We expected szToken to begin at szNextChar. Since it doesn't, extra break
					//	characters must have been removed by strtok as they were adjacent. We want 
					//	a line break for every break character, so add lines for each space that
					//	szNextChar is off by.
					szNextChar++;
					Add_Item_Detail( " ", szHelp, pIcon0, IconKind0, szExtraDataString, pvExtraDataPtr, pColorRemap, pIcon1, IconKind1, pIcon2, IconKind2, pFixedIcon, FixedIconKind, iXFixedIcon, iYFixedIcon, iFixedIconWidth );
				}
				iRetVal = Add_Item_Detail( szToken, szHelp, pIcon0, IconKind0, szExtraDataString, pvExtraDataPtr, pColorRemap, pIcon1, IconKind1, pIcon2, IconKind2, pFixedIcon, FixedIconKind, iXFixedIcon, iYFixedIcon, iFixedIconWidth );

				//	Expect next token two chars after the end of this one.
				szNextChar = szToken + strlen( szToken ) + 1;

				//	Get next token.
				szToken = strtok( NULL, szBreakchars );
			}
			delete [] szText;
			return iRetVal;				//	Last value returned by ListClass::Add_Item
		}
		else
		{
			//	Add one item to list.
			IconList_ItemExtras* pItemExtra = new IconList_ItemExtras;
			pItemExtra->bMultiSelected = false;
			pItemExtra->pIcon[0] = pIcon0;
			pItemExtra->IconKind[0] = IconKind0;
			pItemExtra->pIcon[1] = pIcon1;
			pItemExtra->IconKind[1] = IconKind1;
			pItemExtra->pIcon[2] = pIcon2;
			pItemExtra->IconKind[2] = IconKind2;
			pItemExtra->FixedIcon.pIcon = pFixedIcon;
			pItemExtra->FixedIcon.IconKind = FixedIconKind;
			pItemExtra->FixedIcon.xOffset = iXFixedIcon;
			pItemExtra->FixedIcon.yOffset = iYFixedIcon;
			pItemExtra->FixedIcon.iWidth = iFixedIconWidth;
			pItemExtra->pvExtraData = pvExtraDataPtr;
			pItemExtra->pColorRemap = pColorRemap;
			if( szHelp )
			{
				//	Copy help into new help string.
				pItemExtra->szHelp = new char[ strlen( szHelp ) + 1 ];
				strcpy( pItemExtra->szHelp, szHelp );
			}
			if( szExtraDataString )
			{
				//	Copy special data string into new extradata string.
				pItemExtra->szExtraData = new char[ strlen( szExtraDataString ) + 1 ];
				strcpy( pItemExtra->szExtraData, szExtraDataString );
			}
			ExtrasList.Add( pItemExtra );

			return ListClass::Add_Item( text );
		}
	}
	else
	{
		//	(no text for new item)
		if( pIcon0 || pIcon1 || pIcon2 )
		{
			//	Note: Cannot add an entry without text unless string allocation is being handled by me.
			//	Otherwise, because we want the icon to show up, create a blank entry for the ListClass.
			if( bDoAlloc )
			{
				IconList_ItemExtras* pItemExtra = new IconList_ItemExtras;
				pItemExtra->bMultiSelected = false;
				pItemExtra->pIcon[0] = pIcon0;
				pItemExtra->IconKind[0] = IconKind0;
				pItemExtra->pIcon[1] = pIcon1;
				pItemExtra->IconKind[1] = IconKind1;
				pItemExtra->pIcon[2] = pIcon2;
				pItemExtra->IconKind[2] = IconKind2;
				pItemExtra->FixedIcon.pIcon = pFixedIcon;
				pItemExtra->FixedIcon.IconKind = FixedIconKind;
				pItemExtra->FixedIcon.xOffset = iXFixedIcon;
				pItemExtra->FixedIcon.yOffset = iYFixedIcon;
				pItemExtra->FixedIcon.iWidth = iFixedIconWidth;
				pItemExtra->pvExtraData = pvExtraDataPtr;
				pItemExtra->pColorRemap = pColorRemap;
				if( szHelp )
				{
					//	Copy help into new help string.
					pItemExtra->szHelp = new char[ strlen( szHelp ) + 1 ];
					strcpy( pItemExtra->szHelp, szHelp );
				}
				if( szExtraDataString )
				{
					//	Copy special data string into new extradata string.
					pItemExtra->szExtraData = new char[ strlen( szExtraDataString ) + 1 ];
					strcpy( pItemExtra->szExtraData, szExtraDataString );
				}
				ExtrasList.Add( pItemExtra );

				if( iMaxItems && List.Count() == iMaxItems )
				{
					//	Delete head of list.
					Remove_Item( 0 );
				}
				//	Create new string, essentially blank.
				char* szText = new char[2];
				strcpy( szText, " " );
				return ListClass::Add_Item( szText );
			}
			else
				//	Cannot add entry, as text is blank and ListClass::Add_Item will do nothing.
				//	The Icon we want will not show up.
				return List.Count() - 1;
		}
		else
			return ListClass::Add_Item( text );
	}
}

//***********************************************************************************************
int IconListClass::Add_Item_Detail( const char* szToken, const char* szHelp,
										void* pIcon0, ICONKIND IconKind0, const char* szExtraDataString, 
										void* pvExtraData, RemapControlType* pColorRemap,
										void* pIcon1, ICONKIND IconKind1, 
										void* pIcon2, ICONKIND IconKind2,
										void* pFixedIcon, ICONKIND FixedIconKind, int iXFixedIcon, int iYFixedIcon, int iFixedIconWidth )
{
	//	Broken out of above function as it is repeated.

	//	Add one item to list.
	//	Too many entries?
	if( iMaxItems && List.Count() == iMaxItems )
	{
		//	Delete head of list.
		Remove_Item( 0 );
	}
	//	Create icon entry.
	IconList_ItemExtras* pItemExtra = new IconList_ItemExtras;
	pItemExtra->bMultiSelected = false;
	pItemExtra->pIcon[0] = pIcon0;				//	ajw - Question: repeat the icon for each entry? make it optional?
	pItemExtra->IconKind[0] = IconKind0;
	pItemExtra->pIcon[1] = pIcon1;
	pItemExtra->IconKind[1] = IconKind1;
	pItemExtra->pIcon[2] = pIcon2;
	pItemExtra->IconKind[2] = IconKind2;
	pItemExtra->FixedIcon.pIcon = pFixedIcon;
	pItemExtra->FixedIcon.IconKind = FixedIconKind;
	pItemExtra->FixedIcon.xOffset = iXFixedIcon;
	pItemExtra->FixedIcon.yOffset = iYFixedIcon;
	pItemExtra->FixedIcon.iWidth = iFixedIconWidth;
	pItemExtra->pvExtraData = pvExtraData;
	pItemExtra->pColorRemap = pColorRemap;
	if( szHelp )
	{
		//	Copy help into new help string.
		pItemExtra->szHelp = new char[ strlen( szHelp ) + 1 ];
		strcpy( pItemExtra->szHelp, szHelp );
	}
	if( szExtraDataString )
	{
		//	Copy special data string into new extradata string.
		pItemExtra->szExtraData = new char[ strlen( szExtraDataString ) + 1 ];
		strcpy( pItemExtra->szExtraData, szExtraDataString );
	}
	ExtrasList.Add( pItemExtra );
	//	Create text entry.
	//	Copy text to new string.
	char* szTextBit = new char[ strlen( szToken ) + 1 ];
	strcpy( szTextBit, szToken );
	return ListClass::Add_Item( szTextBit );
}

//***********************************************************************************************
int IconListClass::Add_Item( int text )
{
	return Add_Item( Text_String(text), NULL, NULL, ICON_SHAPE );
}

//***********************************************************************************************
int IconListClass::Add_Item( int text, const char* szHelp,
								void* pIcon0, ICONKIND IconKind0, const char* szExtraDataString /* = NULL */, 
								void* pvExtraDataPtr /* = NULL */, RemapControlType* pColorRemap /* = NULL */,
								void* pIcon1 /* = NULL */, ICONKIND IconKind1 /* = ICON_SHAPE */, 
								void* pIcon2 /* = NULL */, ICONKIND IconKind2 /* = ICON_SHAPE */,
								void* pFixedIcon /* = NULL */, ICONKIND FixedIconKind /* = ICON_SHAPE */, int iXFixedIcon /* = 0 */, int iYFixedIcon /* = 0 */, int iFixedIconWidth /* = -1 */ )
{
	return Add_Item( Text_String(text), szHelp, pIcon0, IconKind0, szExtraDataString, pvExtraDataPtr, pColorRemap, 
						pIcon1, IconKind1, pIcon2, IconKind2, pFixedIcon, FixedIconKind, iXFixedIcon, iYFixedIcon, iFixedIconWidth );
}

//***********************************************************************************************
void IconListClass::Remove_Item( char const * text )
{
	if( text )
		Remove_Item( List.ID(text) );
}

//***********************************************************************************************
void IconListClass::Remove_Item( int index )
{
	if( (unsigned)index < List.Count() )
	{
		delete (IconList_ItemExtras*)ExtrasList[ index ];
		ExtrasList.Delete( index );
		if( bDoAlloc )
			//	Delete alloc'ed string.
			delete [] (char*)List[index];
		ListClass::Remove_Item( index );
		
		//	I should probably put this in ListClass:Remove_Item(), as it seems clearly to be
		//	missing, but I want to only affect my own new code, to not introduce possible bugs.
		//	Shift the selected index if appropriate...
		if( SelectedIndex >= index )
		{
			SelectedIndex--;
			if( SelectedIndex < 0 )
				SelectedIndex = 0;
		}
	}
}

/***********************************************************************************************
 * IconListClass::Draw_Entry -- Calls ListClass::Draw_Entry, then adds icon.                   *
 *                                                                                             *
 *    This routine is called by the Draw_Me function when it desired to redraw a particular    *
 *    text line in the list box.                                                               *
 *                                                                                             *
 * INPUT:   index    -- The index of the list entry to draw. This index is based on the        *
 *                      total list and NOT the current visible view page.                      *
 *                                                                                             *
 *          x,y      -- Pixel coordinates for the upper left corner of the text entry.         *
 *                                                                                             *
 *          width    -- The maximum width that the text may draw over. It is expected that     *
 *                      this drawing routine entirely fills this length.                       *
 *                                                                                             *
 *          selected -- bool; Is this a selected (highlighted) listbox entry?                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 * WARNINGS:   none                                                                            *
 * HISTORY:                                                                                    *
 *   07/07/1998  ajw: Created.                                                                 *
 *=============================================================================================*/

#define PREICONGAP		1
#define ICONTEXTGAP		2

void IconListClass::Draw_Entry( int index, int x, int y, int width, int selected )
{
	IconList_ItemExtras* pExtras = (IconList_ItemExtras*)ExtrasList[ index ];

	int xText = x;
	//	ajw If I end up needing to use SHAPEs for icons, figure out shape width here and offset x.
	bool bIconsPresent = false;
	for( int iIcon = 0; iIcon != 3; iIcon++ )
		if( pExtras->pIcon[ iIcon ] && pExtras->IconKind[ iIcon ] == ICON_DIB )
		{
			//	Push text over to accommodate icon.
			int iWidthIcon = PREICONGAP + DIBWidth( (char*)pExtras->pIcon[ iIcon ] );
			xText += iWidthIcon;
			width -= iWidthIcon;
			bIconsPresent = true;
		}
	if( bIconsPresent )
	{
		xText += ICONTEXTGAP;
		width -= ICONTEXTGAP;
	}

	RemapControlType* pRemap = pExtras->pColorRemap;
	if( !pRemap )
	{
		//	Tabs hack. If there are icons, and a tab, push back the FIRST tab appropriately.
		//	(Ignore others. This is a hack because having more than one tab will now break this.)
		//	See local version of this same hack, below.
		int TempTabs;
		const int* TabsSave;
		if( Tabs )
		{
			TempTabs = *Tabs - ( xText - x );
			TabsSave = Tabs;
			Tabs = &TempTabs;
		}
		switch( iSelectType )
		{
		case 0:
			//	Don't draw any items selected (even if they are, really, in ListClass).
			ListClass::Draw_Entry( index, xText, y, width, false );
			break;
		case 1:
			ListClass::Draw_Entry( index, xText, y, width, selected );
			break;
		case 2:
			//	Ignore 'selected' parameter. We use our own records.
			ListClass::Draw_Entry( index, xText, y, width, pExtras->bMultiSelected );
			break;
		}
		//	Restore Tabs.
		if( Tabs )
			Tabs = TabsSave;
	}
	else
	{
		//	Use different color remapping.
		//	This is largely copied straight from ListClass::Draw_Entry()...

		TextPrintType flags = TextFlags;

		bool bShowSelected;

		switch( iSelectType )
		{
		case 0:
			bShowSelected = false;
			break;
		case 1:
			bShowSelected = selected;
			break;
		case 2:
			bShowSelected = pExtras->bMultiSelected;
			break;
		}

		if( bShowSelected )
		{
			flags = flags | TPF_BRIGHT_COLOR;
			LogicPage->Fill_Rect( xText, y, xText + width - 1, y + LineHeight - 1, pRemap->Shadow );
		}
		else
		{
			if (!(flags & TPF_USE_GRAD_PAL))
			{
				flags = flags | TPF_MEDIUM_COLOR;
			}
		}
		//	Tabs hack. If there are icons, and a tab, push back the FIRST tab appropriately.
		//	(Ignore others. This is a hack because having more than one tab will now break this.)
		if( Tabs )
		{
			int tab = *Tabs - ( xText - x );
			Conquer_Clip_Text_Print( List[index], xText, y, pRemap, TBLACK, flags, width, &tab );
		}
		else
			Conquer_Clip_Text_Print( List[index], xText, y, pRemap, TBLACK, flags, width, NULL );
	}

	//	Draw fixed position icon.
	if( pExtras->FixedIcon.pIcon )
	{
		if( pExtras->FixedIcon.IconKind == ICON_SHAPE )
			CC_Draw_Shape( pExtras->FixedIcon.pIcon, 0, x + pExtras->FixedIcon.xOffset, y + pExtras->FixedIcon.yOffset, WINDOW_MAIN, SHAPE_NORMAL );
			//	Put similar code in here for shapes if used...
		else
			CC_Draw_DIB( (char*)pExtras->FixedIcon.pIcon, x + pExtras->FixedIcon.xOffset, y + pExtras->FixedIcon.yOffset, pExtras->FixedIcon.iWidth, WINDOW_MAIN );
	}

	//	Draw variable position left-of-text icons.
	for( iIcon = 0; iIcon != 3; iIcon++ )
	{
		if( pExtras->pIcon[ iIcon ] )
		{
			x += PREICONGAP;
			if( pExtras->IconKind[ iIcon ] == ICON_SHAPE )
				CC_Draw_Shape( pExtras->pIcon[ iIcon ], 0, x, y, WINDOW_MAIN, SHAPE_NORMAL );
				//	Put similar code in here for shapes if used...
			else
			{
				CC_Draw_DIB( (char*)pExtras->pIcon[ iIcon ], x, y, 9999, WINDOW_MAIN );
				x += DIBWidth( (char*)pExtras->pIcon[ iIcon ] );
			}
		}
	}
}

//***********************************************************************************************
int IconListClass::Action(unsigned flags, KeyNumType & key)
{
	//	Overriding of function is for the sake of MultiSelecting only.
	if( iSelectType == 2 )
	{
		if( !( flags & LEFTRELEASE ) )
		{
			if( !( flags & KEYBOARD ) )
			{
				int index = Get_Mouse_Y() - (Y+1);
				index = index / LineHeight;
				int iSelected = CurrentTopIndex + index;
				iSelected = min( iSelected, List.Count() - 1 );
				if( iSelected >= 0 )
					((IconList_ItemExtras*)ExtrasList[ iSelected ])->bMultiSelected = 
						!((IconList_ItemExtras*)ExtrasList[ iSelected ])->bMultiSelected;
			}
		}
	}
	return ListClass::Action( flags, key );
}

//***********************************************************************************************
// * IconListClass::Show_Last_Item -- Scrolls listbox down to ensure that last entry is visible.
//	ajw 07/09/98
void IconListClass::Show_Last_Item()
{
	int iItemLast = List.Count() - 1;
	if( iItemLast - LineCount + 1 != CurrentTopIndex )
	{
		Flag_To_Redraw();
		Set_View_Index( iItemLast - LineCount + 1 );
	}
}

//***********************************************************************************************
bool IconListClass::bItemIsMultiSelected( int index ) const
{
	if( index < ExtrasList.Count() && index > -1 )
		return ( (IconList_ItemExtras*)ExtrasList[ index ] )->bMultiSelected;
	else
		return false;
}

//***********************************************************************************************
void IconListClass::MultiSelect( int index, bool bSelect )
{
	if( index < ExtrasList.Count() && index > -1 )
		( (IconList_ItemExtras*)ExtrasList[ index ] )->bMultiSelected = bSelect;
}

//***********************************************************************************************
const char* IconListClass::Get_Item_ExtraDataString( int index ) const
{
	//	Returns const pointer to the hidden "extra data" string that can be associated with each item.
	//	This is NULL if no extra data was assigned.
	if( index < ExtrasList.Count() && index > -1 )
	{
		return ( (IconList_ItemExtras*)ExtrasList[ index ] )->szExtraData;
	}
	return NULL;
}

//***********************************************************************************************
void IconListClass::Set_Item_ExtraDataString( int index, const char* szNewString )
{
	if( index < ExtrasList.Count() && index > -1 )
	{
		IconList_ItemExtras* pItemExtra = (IconList_ItemExtras*)ExtrasList[ index ];
		if( pItemExtra->szExtraData )
		{
			//	Delete the existing string.
			delete [] pItemExtra->szExtraData;
		}
		if( szNewString )
		{
			//	Copy special data string into new extradata string.
			pItemExtra->szExtraData = new char[ strlen( szNewString ) + 1 ];
			strcpy( pItemExtra->szExtraData, szNewString );
		}
		else
			pItemExtra->szExtraData = NULL;
	}
}
				
//***********************************************************************************************
void* IconListClass::Get_Item_ExtraDataPtr( int index ) const
{
	//	Returns the hidden "extra data" void pointer that can be associated with each item.
	//	This is NULL if no value was assigned.
	if( index < ExtrasList.Count() && index > -1 )
		return ( (IconList_ItemExtras*)ExtrasList[ index ] )->pvExtraData;
	else
		return NULL;
}

//***********************************************************************************************
void IconListClass::Set_Item_ExtraDataPtr( int index, void* pNewValue )
{
	//	Sets the hidden "extra data" void pointer that can be associated with each item.
	if( index < ExtrasList.Count() && index > -1 )
		( (IconList_ItemExtras*)ExtrasList[ index ] )->pvExtraData = pNewValue;
}

//***********************************************************************************************
const IconList_ItemExtras* IconListClass::Get_ItemExtras( int index ) const
{
	if( index < ExtrasList.Count() && index > -1 )
		return (IconList_ItemExtras*)ExtrasList[ index ];
	else
		return NULL;
}
		
//***********************************************************************************************
const char* IconListClass::Get_Item_Help( int index ) const
{
	//	Returns pointer to the string allocated for tooltip help.
	if( index < ExtrasList.Count() && index > -1 )
		return ( (IconList_ItemExtras*)ExtrasList[ index ] )->szHelp;
	else
		return NULL;
}

//***********************************************************************************************
void IconListClass::Clear()
{
	//	Removes all items from list.

	//	Delete the IconList_ItemExtras structs created to hold extra info on each item.
	for( int i = 0; i < ExtrasList.Count(); i++ )
		delete (IconList_ItemExtras*)ExtrasList[ i ];
	ExtrasList.Clear();

	if( bDoAlloc )
	{
		//	Delete all alloc'ed strings.
		for( int i = 0; i < List.Count(); i++ )
			delete [] (char*)List[i];
	}

	List.Clear();
	Remove_Scroll_Bar();
	CurrentTopIndex = 0;
}

//***********************************************************************************************
RemapControlType* IconListClass::Get_Item_Color( int index )
{
	if( index < ExtrasList.Count() && index > -1 )
		return ( (IconList_ItemExtras*)ExtrasList[ index ] )->pColorRemap;
	else
		return NULL;
}

//***********************************************************************************************
void IconListClass::Set_Item_Color( int index, RemapControlType* pColorRemap )
{
	if( index < ExtrasList.Count() && index > -1 )
		( (IconList_ItemExtras*)ExtrasList[ index ] )->pColorRemap = pColorRemap;
}

//***********************************************************************************************
int IconListClass::Find( const char* szItemToFind )
{
	//	Returns -1 if szItemToFind is not found as the text BEGINNING one of the list entries, else index of item.
	//	Compare is case-sensitive.
	for( int i = 0; i < List.Count(); i++ )
	{
		if( strncmp( List[ i ], szItemToFind, strlen( szItemToFind ) ) == 0 )
			return i;
	}
	return -1;
}

//***********************************************************************************************
int IconListClass::FindColor( RemapControlType* pColorRemap )
{
	//	Returns -1 if no items of specified color are found, else first index. Assumes colorptr == colorptr is a valid equality test.
	for( int i = 0; i < List.Count(); i++ )
	{
		if( Get_Item_Color( i ) == pColorRemap )
			return i;
	}
	return -1;
}

//***********************************************************************************************
bool IconListClass::Set_Item( unsigned int index, const char* szText )
{
	//	Resets the text string allocated for an item.
	if( !bDoAlloc || index >= List.Count() )
		return false;

	//	Delete alloc'ed string.
	delete [] (char*)List[ index ];

	//	Copy text to new string.
	char* szTextNew = new char[ strlen( szText ) + 1 ];
	strcpy( szTextNew, szText );

	//	Reassign List's ptr.
	List[ index ] = szTextNew;

	return true;
}

//***********************************************************************************************
bool IconListClass::Set_Icon( unsigned int index, unsigned int iIconNumber, void* pIcon, ICONKIND IconKind )
{
	if( index >= List.Count() )
		return false;

	//	Sets one of the left-aligned icons.
	( (IconList_ItemExtras*)ExtrasList[ index ] )->pIcon[ iIconNumber ] = pIcon;
	( (IconList_ItemExtras*)ExtrasList[ index ] )->IconKind[ iIconNumber ] = IconKind;
	return true;
}

//***********************************************************************************************
int IconListClass::GetRealWidth()		//	sigh
{
	if( IsScrollActive )
		return Width + ScrollGadget.Width;
	return Width;
}

//***********************************************************************************************
void IconListClass::Resize( int x, int y, int w, int h )
{
	Remove_Scroll_Bar();	//	If there is one.

	X = x;
	Y = y;
	Width = w;
	Height = h;

	Set_Position( x, y );

	LineCount = (h-1) / LineHeight;


	if (List.Count() > LineCount) {
		Add_Scroll_Bar();
	}

	Flag_To_Redraw();
}

//***********************************************************************************************
int IconListClass::IndexUnderMouse()
{
	//	Returns index of line that mouse is currently over, or -1 for mouse not hitting valid index.
	//	Assumes that x position of mouse is already known to be over the iconlist.
	int index = Get_Mouse_Y() - (Y+1);
	index = index / LineHeight + CurrentTopIndex;
	if( index > List.Count() - 1 || index < 0 )
		return -1;
	return index;
}

//***********************************************************************************************
int IconListClass::OffsetToIndex( int iIndex, int y )
{
	//	Finds the current offset of item iIndex from the current top view index, in pixels, and add it to y.
	return y + ( iIndex - CurrentTopIndex ) * LineHeight;
}

//***********************************************************************************************
//***********************************************************************************************
//	* Format_Window_String_New
//	Functions like Format_Window_String except it fixes an infinite loop bug that occurred when strings
//	lacked suitable break points, eliminates the '@' as an escape character, and operates differently
//	in that it leaves the original string along, writing results instead to a second string parameter,
//	that is iExtraChars longer than the original string. This is all a big hack so that I can insert 
//	extra break characters when a break in a long single word has to be made.
//	Hey - it's better than an infinite loop that forces you to reset your machine, as in the original code...

int Format_Window_String_New( const char* string, int maxlinelen, int& width, int& height, char* szReturn, int iExtraChars )
{
	int	linelen;
	int	lines = 0;
	width	= 0;
	height = 0;

	// In no string was passed in, then there are no lines.
	if (!string) return(0);

	// While there are more letters left divide the line up.
	while (*string) {
		linelen = 0;
		height += FontHeight + FontYSpacing;
		lines++;

		// While the current line is less then the max length...
		*szReturn = *string;
		linelen += Char_Pixel_Width( *string );
		while ( linelen < maxlinelen && *string != '\r' && *string != '\0' )
		{
			*++szReturn = *++string;
			linelen += Char_Pixel_Width( *string );
		}

		// if the line is too long...
		if (linelen >= maxlinelen) 
		{
			/*
			**	Back up to an appropriate location to break.
			*/
			const char* stringOverEnd = string;
			while( linelen > 0 && *string != ' ' && *string != '\r' && *string != '\0' )
			{
				linelen -= Char_Pixel_Width(*string--);
			}
			if( linelen <= 0 )
			{
				//	We could not find a nice break point.
				//	Go back one char from over-the-end point and add in a break there.
				string = stringOverEnd - 1;
				if( iExtraChars > 0 )
					iExtraChars--;			//	One less to make use of later.
				else
					//	We've used up all our extras characters.
					//	Put in a break below by wiping out a valid char here.
					szReturn--;
			}
			else
			{
				//	Back up szReturn to same location.
				szReturn -= ( stringOverEnd - string );
			}
		}

		/*
		**	Record the largest width of the worst case string.
		*/
		if (linelen > width) {
			width = linelen;
		}

		/*
		**	Force a break at the end of the line.
		*/
		if (*string) {
		 	*szReturn++ = '\r';
			string++;
		}
	}
	return(lines);
}

//***********************************************************************************************
void CC_Draw_DIB( const char* pDIB, int xDest, int yDest, int iWidth, WindowNumberType window )
{
	//	A very basic DIB drawing routine. No clipping. No edge of window overrun checking.
	//	If iWidth is too large, default width of dib is used.
	//	If iWidth is negative, dib isn't drawn.
	if( pDIB && iWidth >= 0 )
	{

		int iWidthDIB = DIBWidth( pDIB );
		int iHeight = DIBHeight( pDIB );
		const char* pBits = FindDIBBits( pDIB );

		int iSrcPitch = ( iWidthDIB + 3 ) & ~3;

		if( iWidth > iWidthDIB )
			iWidth = iWidthDIB;

		GraphicViewPortClass draw_window(	LogicPage->Get_Graphic_Buffer(),
											WindowList[window][WINDOWX] + LogicPage->Get_XPos(),
											WindowList[window][WINDOWY] + LogicPage->Get_YPos(),
											WindowList[window][WINDOWWIDTH],
											WindowList[window][WINDOWHEIGHT] );
		if( draw_window.Lock() )
		{
			int iDestPitch = draw_window.Get_Pitch() + draw_window.Get_Width();	//	Meaning of "Pitch" in this class seems to mean the eol skip.
			char* pLineDest = (char*)draw_window.Get_Offset() + xDest + ( yDest + iHeight - 1 ) * iDestPitch;

			const char* pLineSrc = pBits;
			for( int y = 0; y != iHeight; y++ )
			{
				char* pDest = pLineDest;
				const char* pSrc = pLineSrc;
				for( int x = 0; x != iWidth; x++ )
				{
					*pDest++ = *pSrc++;
				}
				pLineDest -= iDestPitch;
				pLineSrc += iSrcPitch;
			}
			draw_window.Unlock();
		}
	}
//	else
//		debugprint( "CC_Draw_DIB bad case ------------ pDib %i, iWidth %i\n", pDIB, iWidth );
}


#endif
