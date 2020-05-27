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
 * IconListClass -- Exactly like ListClass, but displays an icon as well
 *                  (actually a 'shape' image), left-aligned, covering
 *                  any text that happens to be there...
 *                  Also, I've added the option of making this class
 *                  responsible for the mem alloc. of the strings, and
 *					an automatic limiting of entries to a set maximum.
 *                                                                         *
 * HISTORY:    07/07/1998 ajw : Created, largely in hack mode.             *
 *=========================================================================*/

#ifndef ICONLIST_H
#define ICONLIST_H

#include "function.h"
#include "vector.h"

enum ICONKIND
{
	ICON_SHAPE = 0,		//	pIcon points to a shape.
	ICON_DIB			//	pIcon points to DIBitmap data.
};

struct FIXEDICON		//	For putting icons in list entries at a specific fixed offset.
{
	void*		pIcon;
	ICONKIND	IconKind;
	int			xOffset;
	int			yOffset;
	int			iWidth;
};

struct IconList_ItemExtras
{
	IconList_ItemExtras() : bMultiSelected( false ), szHelp( NULL ), szExtraData( NULL ), pvExtraData( NULL )
	{
		pIcon[0] = NULL;
		pIcon[1] = NULL;
		pIcon[2] = NULL;
	}
	virtual ~IconList_ItemExtras()
	{
		delete [] szHelp;
		delete [] szExtraData;
	}

	bool		bMultiSelected;		//	True if selected when bMultiSelect is on.
	void*		pIcon[3];			//	Icon that appears before an item.
	ICONKIND	IconKind[3];		//	Specifies what kind of image data pIcon points to.
	char*		szHelp;				//	Tooltip help string that can be associated with item. Allocated and deleted here.
	char*		szExtraData;		//	Extra string that can be associated with item. Allocated and deleted here.
	void*		pvExtraData;		//	Hidden pointer that can be associated with item.
	RemapControlType*	pColorRemap;	//	Pointer to a color remap, or null for default colored text.
	FIXEDICON	FixedIcon;
};

class IconListClass : public ListClass
{
	public:
		IconListClass( int id, int x, int y, int w, int h, TextPrintType flags, void const * up, void const * down, bool bResponsibleForStringAlloc = FALSE, int iSelectionType = 1, int iMaxItemsSaved = 0 );
//		IconListClass( const IconListClass& list );
		virtual ~IconListClass( void );

		virtual int Add_Item( char const * text );
		virtual int Add_Item( const char* text, const char* szHelp,
								void* pIcon0, ICONKIND IconKind0, const char* szExtraDataString = NULL, 
								void* pvExtraDataPtr = NULL, RemapControlType* pColorRemap = NULL,
								void* pIcon1 = NULL, ICONKIND IconKind1 = ICON_SHAPE, 
								void* pIcon2 = NULL, ICONKIND IconKind2 = ICON_SHAPE,
								void* pFixedIcon = NULL, ICONKIND FixedIconKind = ICON_SHAPE, int iXFixedIcon = 0, int iYFixedIcon = 0, int iFixedIconWidth = -1 );

		virtual int Add_Item( int text );
		virtual int Add_Item( int text, const char* szHelp,
								void* pIcon0, ICONKIND IconKind0, const char* szExtraDataString = NULL, 
								void* pvExtraDataPtr = NULL, RemapControlType* pColorRemap = NULL,
								void* pIcon1 = NULL, ICONKIND IconKind1 = ICON_SHAPE, 
								void* pIcon2 = NULL, ICONKIND IconKind2 = ICON_SHAPE,
								void* pFixedIcon = NULL, ICONKIND FixedIconKind = ICON_SHAPE, int iXFixedIcon = 0, int iYFixedIcon = 0, int iFixedIconWidth = -1 );

//		virtual int Add_Scroll_Bar(void);
//		virtual void Bump(int up);
//		virtual int Count(void) const {return List.Count();};
//		virtual int Current_Index(void) const;
//		virtual char const * Current_Item(void) const;
//		virtual int Draw_Me(int forced);
//		virtual char const * Get_Item(int index) const;
//		virtual int Step_Selected_Index(int forward);
//		virtual void Flag_To_Redraw(void);

//		virtual void Peer_To_Peer(unsigned flags, KeyNumType & key, ControlClass & whom);
		virtual void Remove_Item( char const * text );
		virtual void Remove_Item( int );
//		virtual int  Remove_Scroll_Bar(void);
//		virtual void Set_Selected_Index(int index);
//		virtual void Set_Selected_Index(char const * text);
//		virtual void Set_Tabs(int const * tabs);
//		virtual int  Set_View_Index(int index);
//		virtual void Step(int up);
//		virtual void Set_Position(int x, int y);

		/*
		** These overloaded list routines handle adding/removing the scroll bar
		** automatically when the list box is added or removed.
		*/
//		virtual LinkClass & Add(LinkClass & object);
//		virtual LinkClass & Add_Tail(LinkClass & object);
//		virtual LinkClass & Add_Head(LinkClass & object);
//		virtual GadgetClass * Remove(void);

		virtual void		Show_Last_Item();
		virtual bool		bItemIsMultiSelected( int index ) const;
		virtual void		MultiSelect( int index, bool bSelect );
		virtual const char*	Get_Item_ExtraDataString( int index ) const;
		virtual void		Set_Item_ExtraDataString( int index, const char* szNewString );
		virtual void*		Get_Item_ExtraDataPtr( int index ) const;
		virtual void		Set_Item_ExtraDataPtr( int index, void* pNewValue );
		const char*			Get_Item_Help( int index ) const;
		virtual RemapControlType*	Get_Item_Color( int index );
		virtual void				Set_Item_Color( int index, RemapControlType* pColorRemap );
		virtual const IconList_ItemExtras*	Get_ItemExtras( int index ) const;
		virtual void		Clear();
		virtual int			Get_View_Index()	{	return CurrentTopIndex;		}
		bool bScrollBeingDragged()
		{
			//	Returns true if the scroll bar of the list is being dragged by the user.
			return ( GadgetClass::StuckOn == &ScrollGadget );
		}

		virtual int			Find( const char* szItemToFind );
		virtual int			FindColor( RemapControlType* pColorRemap );

		virtual bool		Set_Item( unsigned int index, const char* szText );
		virtual bool		Set_Icon( unsigned int index, unsigned int iIconNumber, void* pIcon, ICONKIND IconKind );

		virtual int			GetRealWidth();
		virtual void		Resize( int x, int y, int w, int h );
		virtual int			IndexUnderMouse();
		virtual int			OffsetToIndex( int iIndex, int y );

		virtual int			SetSelectType( int iSelectTypeNew )
		{
			//	Provided to enable horrible hacks, mainly involved with dealing with ListClass's inability
			//	to have no item selected...
			int iSelectTypeOld = iSelectType;
			iSelectType = iSelectTypeNew;
			return iSelectTypeOld;
		}

	protected:
		virtual int  Action(unsigned flags, KeyNumType &key);
		virtual void Draw_Entry( int index, int x, int y, int width, int selected );

		virtual int  Add_Item_Detail( const char* szToken, const char* szHelp,
										void* pIcon0, ICONKIND IconKind0, const char* szExtraDataString, 
										void* pvExtraData, RemapControlType* pColorRemap,
										void* pIcon1, ICONKIND IconKind1, void* pIcon2, ICONKIND IconKind2,
										void* pFixedIcon, ICONKIND FixedIconKind, int iXFixedIcon, int iYFixedIcon, int iFixedIconWidth );

		//	The list of Icons.
		//DynamicVectorClass<void*> IconList;
		//DynamicVectorClass< IconList_ItemExtras* > ExtrasList;		ajw: creates hellacious linking problems
		DynamicVectorClass< void* > ExtrasList;

		bool bDoAlloc;		//	True if I am responsible for mem. allocation/deletion of strings.
//		bool bMultiSelect;	//	True if we are using the multiple item selection feature.
		int iSelectType;	//	0 for no selection shown, 1 for normal ListClass selection, 2 for n multiple selections
		int	iMaxItems;		//	Number of items to limit list to, if bDoAlloc is true.
};

#endif

#endif
