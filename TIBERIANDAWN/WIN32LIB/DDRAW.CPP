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

/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Westwood Win32 Library                   *
 *                                                                         *
 *                    File Name : DDRAW.CPP                                *
 *                                                                         *
 *                   Programmer : Philip W. Gorrow                         *
 *                                                                         *
 *                   Start Date : October 10, 1995                         *
 *                                                                         *
 *                  Last Update : October 10, 1995   []                    *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*=========================================================================*/
/* The following PRIVATE functions are in this file:                       */
/*=========================================================================*/


/*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/


#include "misc.h"
#include <gbuffer.h>
#include <palette.h>
#include "stdio.h"

LPDIRECTDRAW		DirectDrawObject=NULL;	// Pointer to the direct draw object
LPDIRECTDRAW2	DirectDraw2Interface = NULL;  	// Pointer to direct draw 2 interface

HWND			MainWindow;					// Handle to programs main window
												// this is passed to SetCooperativeLevel
												// so DirectDraw knows which window is ours


PALETTEENTRY				PaletteEntries[256];		// 256 windows palette entries
LPDIRECTDRAWPALETTE	PalettePtr;					// Pointer to direct draw palette object
BOOL						FirstPaletteSet=FALSE;	// Is this the first time 'Set_Palette' has been called?
LPDIRECTDRAWSURFACE	PaletteSurface=NULL;
SurfaceMonitorClass	AllSurfaces;				//List of all direct draw surfaces
BOOL						CanVblankSync = TRUE;

BOOL						SystemToVideoBlits =FALSE;	// Does hardware support system mem to video mem blits?
BOOL						VideoToSystemBlits =FALSE;	// Does hardware support video mem to system mem blits?
BOOL						SystemToSystemBlits = FALSE; 	// Does hardware support system mem to system mem blits?
BOOL						OverlappedVideoBlits = TRUE;	// Can video driver blit overlapped regions?

/*
** Function to call if we detect focus loss
*/
extern	void (*Misc_Focus_Loss_Function)(void) = NULL;
extern	void (*Misc_Focus_Restore_Function)(void) = NULL;


/***********************************************************************************************
 * Process_DD_Result -- Does a message box based on the result of a DD command                 *
 *                                                                                             *
 * INPUT:		HRESULT result				- the result returned from the direct draw command		  *
 *             int     display_ok_msg	- should a message be displayed if command ok			  *                                                                                      *
 *                                                                                             *
 * OUTPUT:		none																									  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/27/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
void Process_DD_Result(HRESULT result, int display_ok_msg)
{
	switch (result) {
		case DD_OK:
			if (display_ok_msg) {
				MessageBox(MainWindow, "Direct Draw request went ok.", "Note", MB_ICONEXCLAMATION|MB_OK);
         }
			break;
		case DDERR_ALREADYINITIALIZED:
			MessageBox(MainWindow, "This object is already initialized ","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_BLTFASTCANTCLIP:
			MessageBox(MainWindow, "Return if a clipper object is attached to the source surface passed into a BltFast call.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_CANNOTATTACHSURFACE:
			MessageBox(MainWindow, "This surface can not be attached to the requested surface.	","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_CANNOTDETACHSURFACE:
			MessageBox(MainWindow, "This surface can not be detached from the requested surface.	","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_CANTCREATEDC:
			MessageBox(MainWindow, "Windows can not create any more DCs","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_CANTDUPLICATE:
			MessageBox(MainWindow, "Can't duplicate primary & 3D surfaces, or surfaces that are implicitly created.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_CANTLOCKSURFACE:
			MessageBox(MainWindow, "Unable to lock surface because no driver exists which can supply a pointer to the surface.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_CLIPPERISUSINGHWND:
			MessageBox(MainWindow, "An attempt was made to set a cliplist for a clipper object that is already monitoring an hwnd.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_COLORKEYNOTSET:
			MessageBox(MainWindow, "No src color key specified for this operation.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_CURRENTLYNOTAVAIL:
			MessageBox(MainWindow, "Support is currently not available.	","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_DIRECTDRAWALREADYCREATED:
			MessageBox(MainWindow, "A DirectDraw object representing this driver has already been created for this process.	","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_EXCEPTION:
			MessageBox(MainWindow, "An exception was encountered while performing the requested operation.	","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_EXCLUSIVEMODEALREADYSET:
			MessageBox(MainWindow, "An attempt was made to set the cooperative level when it was already set to exclusive.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_GENERIC:
			MessageBox(MainWindow, "Generic failure.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_HEIGHTALIGN:
			MessageBox(MainWindow, "Height of rectangle provided is not a multiple of reqd alignment.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_HWNDALREADYSET:
			MessageBox(MainWindow, "The CooperativeLevel HWND has already been set. It can not be reset while the process has surfaces or palettes created.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_HWNDSUBCLASSED:
			MessageBox(MainWindow, "HWND used by DirectDraw CooperativeLevel has been subclassed, this prevents DirectDraw from restoring state.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_IMPLICITLYCREATED:
			MessageBox(MainWindow, "This surface can not be restored because it is an implicitly created surface.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_INCOMPATIBLEPRIMARY:
			MessageBox(MainWindow, "Unable to match primary surface creation request with existing primary surface.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_INVALIDCAPS:
			MessageBox(MainWindow, "One or more of the caps bits passed to the callback are incorrect.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_INVALIDCLIPLIST:
			MessageBox(MainWindow, "DirectDraw does not support the provided cliplist.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_INVALIDDIRECTDRAWGUID:
			MessageBox(MainWindow, "The GUID passed to DirectDrawCreate is not a valid DirectDraw driver identifier.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_INVALIDMODE:
			MessageBox(MainWindow, "DirectDraw does not support the requested mode.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_INVALIDOBJECT:
			MessageBox(MainWindow, "DirectDraw received a pointer that was an invalid DIRECTDRAW object.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_INVALIDPARAMS:
			MessageBox(MainWindow, "One or more of the parameters passed to the function are incorrect.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_INVALIDPIXELFORMAT:
			MessageBox(MainWindow, "The pixel format was invalid as specified.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_INVALIDPOSITION:
			MessageBox(MainWindow, "Returned when the position of the overlay on the destination is no longer legal for that destination.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_INVALIDRECT:
			MessageBox(MainWindow, "Rectangle provided was invalid.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;

		case DDERR_INVALIDSURFACETYPE:
			MessageBox(MainWindow, "The requested action could not be performed because the surface was of the wrong type.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_LOCKEDSURFACES:
			MessageBox(MainWindow, "Operation could not be carried out because one or more surfaces are locked.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NO3D:
			MessageBox(MainWindow, "There is no 3D present.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOALPHAHW:
			MessageBox(MainWindow, "Operation could not be carried out because there is no alpha accleration hardware present or available.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
#if(0)
		case DDERR_NOANTITEARHW:
			MessageBox(MainWindow, "Operation could not be carried out because there is no hardware support for synchronizing blts to avoid tearing.	","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
#endif
		case DDERR_NOBLTHW:
			MessageBox(MainWindow, "No blter hardware present.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
#if(0)
		case DDERR_NOBLTQUEUEHW:
			MessageBox(MainWindow, "Operation could not be carried out because there is no hardware support for asynchronous blting.	","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
#endif
		case DDERR_NOCLIPLIST:
			MessageBox(MainWindow, "No cliplist available.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOCLIPPERATTACHED:
			MessageBox(MainWindow, "No clipper object attached to surface object.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOCOLORCONVHW:
			MessageBox(MainWindow, "Operation could not be carried out because there is no color conversion hardware present or available.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOCOLORKEY:
			MessageBox(MainWindow, "Surface doesn't currently have a color key","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOCOLORKEYHW:
			MessageBox(MainWindow, "Operation could not be carried out because there is no hardware support of the destination color key.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOCOOPERATIVELEVELSET:
			MessageBox(MainWindow, "Create function called without DirectDraw object method SetCooperativeLevel being called.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NODC:
			MessageBox(MainWindow, "No DC was ever created for this surface.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NODDROPSHW:
			MessageBox(MainWindow, "No DirectDraw ROP hardware.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NODIRECTDRAWHW:
			MessageBox(MainWindow, "A hardware-only DirectDraw object creation was attempted but the driver did not support any hardware.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NODIRECTDRAWSUPPORT:
			MessageBox(MainWindow, "No DirectDraw support possible with current display driver.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOEMULATION:
			MessageBox(MainWindow, "Software emulation not available.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOEXCLUSIVEMODE:
			MessageBox(MainWindow, "Operation requires the application to have exclusive mode but the application does not have exclusive mode.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOFLIPHW:
			MessageBox(MainWindow, "Flipping visible surfaces is not supported.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOGDI:
			MessageBox(MainWindow, "There is no GDI present.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOHWND:
			MessageBox(MainWindow, "Clipper notification requires an HWND or no HWND has previously been set as the CooperativeLevel HWND.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOMIRRORHW:
			MessageBox(MainWindow, "Operation could not be carried out because there is no hardware present or available.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOOVERLAYDEST:
			MessageBox(MainWindow, "Returned when GetOverlayPosition is called on an overlay that UpdateOverlay has never been called on to establish a destination.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOOVERLAYHW:
			MessageBox(MainWindow, "Operation could not be carried out because there is no overlay hardware present or available.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOPALETTEATTACHED:
			MessageBox(MainWindow, "No palette object attached to this surface.	","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOPALETTEHW:
			MessageBox(MainWindow, "No hardware support for 16 or 256 color palettes.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NORASTEROPHW:
			MessageBox(MainWindow, "Operation could not be carried out because there is no appropriate raster op hardware present or available.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOROTATIONHW:
			MessageBox(MainWindow, "Operation could not be carried out because there is no rotation hardware present or available.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOSTRETCHHW:
			MessageBox(MainWindow, "Operation could not be carried out because there is no hardware support for stretching.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOT4BITCOLOR:
			MessageBox(MainWindow, "DirectDrawSurface is not in 4 bit color palette and the requested operation requires 4 bit color palette.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOT4BITCOLORINDEX:
			MessageBox(MainWindow, "DirectDrawSurface is not in 4 bit color index palette and the requested operation requires 4 bit color index palette.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOT8BITCOLOR:
			MessageBox(MainWindow, "DirectDrawSurface is not in 8 bit color mode and the requested operation requires 8 bit color.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOTAOVERLAYSURFACE:
			MessageBox(MainWindow, "Returned when an overlay member is called for a non-overlay surface.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOTEXTUREHW:
			MessageBox(MainWindow, "Operation could not be carried out because there is no texture mapping hardware present or available.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOTFLIPPABLE:
			MessageBox(MainWindow, "An attempt has been made to flip a surface that is not flippable.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOTFOUND:
			MessageBox(MainWindow, "Requested item was not found.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOTLOCKED:
			MessageBox(MainWindow, "Surface was not locked.  An attempt to unlock a surface that was not locked at all, or by this process, has been attempted.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOTPALETTIZED:
			MessageBox(MainWindow, "The surface being used is not a palette-based surface.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOVSYNCHW:
			MessageBox(MainWindow, "Operation could not be carried out because there is no hardware support for vertical blank synchronized operations.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOZBUFFERHW:
			MessageBox(MainWindow, "Operation could not be carried out because there is no hardware support for zbuffer blting.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_NOZOVERLAYHW:
			MessageBox(MainWindow, "Overlay surfaces could not be z layered based on their BltOrder because the hardware does not support z layering of overlays.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_OUTOFCAPS:
			MessageBox(MainWindow, "The hardware needed for the requested operation has already been allocated.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_OUTOFMEMORY:
			MessageBox(MainWindow, "DirectDraw does not have enough memory to perform the operation.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_OUTOFVIDEOMEMORY:
			MessageBox(MainWindow, "DirectDraw does not have enough memory to perform the operation.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_OVERLAYCANTCLIP:
			MessageBox(MainWindow, "The hardware does not support clipped overlays.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
			MessageBox(MainWindow, "Can only have ony color key active at one time for overlays.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_OVERLAYNOTVISIBLE:
			MessageBox(MainWindow, "Returned when GetOverlayPosition is called on a hidden overlay.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_PALETTEBUSY:
			MessageBox(MainWindow, "Access to this palette is being refused because the palette is already locked by another thread.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_PRIMARYSURFACEALREADYEXISTS:
			MessageBox(MainWindow, "This process already has created a primary surface.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_REGIONTOOSMALL:
			MessageBox(MainWindow, "Region passed to Clipper::GetClipList is too small.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_SURFACEALREADYATTACHED:
			MessageBox(MainWindow, "This surface is already attached to the surface it is being attached to.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_SURFACEALREADYDEPENDENT:
			MessageBox(MainWindow, "This surface is already a dependency of the surface it is being made a dependency of.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_SURFACEBUSY:
			MessageBox(MainWindow, "Access to this surface is being refused because the surface is already locked by another thread.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_SURFACEISOBSCURED:
			MessageBox(MainWindow, "Access to surface refused because the surface is obscured.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_SURFACELOST:
			MessageBox(MainWindow, "Access to this surface is being refused because the surface memory is gone. The DirectDrawSurface object representing this surface should have Restore called on it.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_SURFACENOTATTACHED:
			MessageBox(MainWindow, "The requested surface is not attached.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_TOOBIGHEIGHT:
			MessageBox(MainWindow, "Height requested by DirectDraw is too large.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_TOOBIGSIZE:
			MessageBox(MainWindow, "Size requested by DirectDraw is too large --	the individual height and width are OK.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_TOOBIGWIDTH:
			MessageBox(MainWindow, "Width requested by DirectDraw is too large.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_UNSUPPORTED:
			MessageBox(MainWindow, "Action not supported.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_UNSUPPORTEDFORMAT:
			MessageBox(MainWindow, "FOURCC format requested is unsupported by DirectDraw.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_UNSUPPORTEDMASK:
			MessageBox(MainWindow, "Bitmask in the pixel format requested is unsupported by DirectDraw.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_VERTICALBLANKINPROGRESS:
			MessageBox(MainWindow, "Vertical blank is in progress.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_WASSTILLDRAWING:
			MessageBox(MainWindow, "Informs DirectDraw that the previous Blt which is transfering information to or from this Surface is incomplete.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_WRONGMODE:
			MessageBox(MainWindow, "This surface can not be restored because it was created in a different mode.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		case DDERR_XALIGN:
			MessageBox(MainWindow, "Rectangle provided was not horizontally aligned on required boundary.","Note", MB_ICONEXCLAMATION|MB_OK);
			break;
		default:
			char string[256];
			sprintf (string, "Unrecognised Direct Draw result code: %d", result & 0xffff);
			MessageBox(MainWindow, string,"Note", MB_ICONEXCLAMATION|MB_OK);
			break;
	}
}



/***********************************************************************************************
 * Check_Overlapped_Blit_Capability -- See if video driver supports blitting overlapped regions*
 *                                                                                             *
 *  We will check for this by drawing something to a video page and blitting it over itself.   *
 * If we end up with the top line repeating then overlapped region blits dont work.            *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 5:06PM ST : Created                                                               *
 *=============================================================================================*/
void Check_Overlapped_Blit_Capability(void)
{

	/*
	** Assume we can until we find out otherwise
	*/
	OverlappedVideoBlits = TRUE;

	GraphicBufferClass test_buffer;

	test_buffer.Init (64, 64, NULL, 0, (GBC_Enum)GBC_VIDEOMEM);

	test_buffer.Clear();

	/*
	** Plot a pixel in the top left corner of the buffer.
	*/
	test_buffer.Put_Pixel(0, 0, 255);

	/*
	** Blit the buffer down by one line. If we end up with a vertical strip of pixel 255's then
	** overlapped blits dont work
	*/

	test_buffer.Blit(test_buffer, 0, 0, 0, 1, test_buffer.Get_Width(), test_buffer.Get_Height()-1);

	if (test_buffer.Get_Pixel (0 ,5) == 255) OverlappedVideoBlits = FALSE;
}



/***********************************************************************************************
 * Set_Video_Mode -- Initializes Direct Draw and sets the required Video Mode                  *
 *                                                                                             *
 * INPUT:  		int width   			- the width of the video mode in pixels						  *
 *					int height           - the height of the video mode in pixels                   *
 *					int bits_per_pixel	- the number of bits per pixel the video mode supports     *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/26/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
BOOL Set_Video_Mode(HWND hwnd, int w, int h, int bits_per_pixel)
{
// ST - 1/3/2019 10:41AM
	return TRUE;
#if (0)
	HRESULT result;
	//
	// If there is not currently a direct draw object then we need to define one.
	//
	if ( DirectDrawObject == NULL ){
		//MessageBox(MainWindow, "In Set_Video_Mode. About to call DirectDrawCreate.","Note", MB_ICONEXCLAMATION|MB_OK);
		result = DirectDrawCreate(NULL, &DirectDrawObject, NULL);
		Process_DD_Result(result, FALSE);
		if (result == DD_OK){
			if (w==320){
				result = DirectDrawObject->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX);
			} else {
				//MessageBox(MainWindow, "In Set_Video_Mode. About to call SetCooperativeLevel.","Note", MB_ICONEXCLAMATION|MB_OK);
				result = DirectDrawObject->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
			}
			Process_DD_Result(result, FALSE);
		}else{
			return (FALSE);
		}
	}

	//
	// Set the required display mode with 8 bits per pixel
	//
	//MessageBox(MainWindow, "In Set_Video_Mode. About to call call SetDisplayMode.","Note", MB_ICONEXCLAMATION|MB_OK);
	result = DirectDrawObject->SetDisplayMode ( w , h , bits_per_pixel );
	if (result != DD_OK){
//		Process_DD_Result(result, FALSE);
		DirectDrawObject->Release();
		DirectDrawObject = NULL;
		return(FALSE);
	}

	//
	// Create a direct draw palette object
	//
	//MessageBox(MainWindow, "In Set_Video_Mode. About to call CreatePalette.","Note", MB_ICONEXCLAMATION|MB_OK);
	result = DirectDrawObject->CreatePalette( DDPCAPS_8BIT | DDPCAPS_ALLOW256, &PaletteEntries[0] , &PalettePtr ,NULL);
	Process_DD_Result(result, FALSE);
	if (result != DD_OK){
		return (FALSE);
	}

	Check_Overlapped_Blit_Capability();

	//MessageBox(MainWindow, "In Set_Video_Mode. About to return success.","Note", MB_ICONEXCLAMATION|MB_OK);
#if (0)
	/*
	** Find out if DirectX 2 extensions are available
	*/
	result = DirectDrawObject->QueryInterface (IID_IDirectDraw2, (LPVOID*)&DirectDraw2Interface);
	SystemToVideoBlits = FALSE;
	VideoToSystemBlits = FALSE;
	SystemToSystemBlits= FALSE;
	if (result != DD_OK){
		DirectDraw2Interface = NULL;
	}else{
		DDCAPS capabilities;
		DDCAPS emulated_capabilities;

		memset ((char*)&capabilities, 0, sizeof(capabilities));
		memset ((char*)&emulated_capabilities, 0, sizeof(emulated_capabilities));
		capabilities.dwSize = sizeof (capabilities);
		emulated_capabilities.dwSize = sizeof (emulated_capabilities);

		DirectDrawObject->GetCaps (&capabilities, &emulated_capabilities);

		if (capabilities.dwCaps & DDCAPS_CANBLTSYSMEM){
			SystemToVideoBlits = (capabilities.dwSVBCaps & DDCAPS_BLT) ? TRUE : FALSE;
			VideoToSystemBlits = (capabilities.dwVSBCaps & DDCAPS_BLT) ? TRUE : FALSE;
			SystemToSystemBlits = (capabilities.dwSSBCaps & DDCAPS_BLT) ? TRUE : FALSE;
		}
	}
#endif	//(0)

	//MessageBox(MainWindow, "In Set_Video_Mode. About to return success.","Note", MB_ICONEXCLAMATION|MB_OK);

	return (TRUE);
#endif
}

/***********************************************************************************************
 * Reset_Video_Mode -- Resets video mode and deletes Direct Draw Object                        *
 *                                                                                             *
 * INPUT:		none                                                                            *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/26/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
void Reset_Video_Mode(void)
{
	HRESULT result;

	//
	// If a direct draw object has been declared and a video mode has been set
	// then reset the video mode and release the direct draw object.
	//
	if ( DirectDrawObject ) {
		result = DirectDrawObject->RestoreDisplayMode();
		Process_DD_Result(result, FALSE);
		result = DirectDrawObject->Release();
		Process_DD_Result(result, FALSE);

		DirectDrawObject = NULL;
	}
}




/***********************************************************************************************
 * Get_Free_Video_Memory -- returns amount of free video memory                                *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   bytes of available video RAM                                                      *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    11/29/95 12:52PM ST : Created                                                            *
 *=============================================================================================*/
unsigned int Get_Free_Video_Memory(void)
{

	DDCAPS	video_capabilities;

	if (DirectDrawObject){

		video_capabilities.dwSize = sizeof (video_capabilities);

		//MessageBox(MainWindow, "In Get_Free_Video_Memory. About to call GetCaps","Note", MB_ICONEXCLAMATION|MB_OK);
		if (DD_OK == DirectDrawObject->GetCaps (&video_capabilities , NULL)){
			char string [256];
			sprintf (string, "In Get_Free_Video_Memory. About to return %d bytes",video_capabilities.dwVidMemFree);
			//MessageBox(MainWindow, string,"Note", MB_ICONEXCLAMATION|MB_OK);
			return (video_capabilities.dwVidMemFree);
		}
	}

	//MessageBox(MainWindow, "In Get_Free_Video_Memory. About to return failure","Note", MB_ICONEXCLAMATION|MB_OK);
	return (0);
}




/***********************************************************************************************
 * Get_Video_Hardware_Caps -- returns bitmask of direct draw video hardware support            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   hardware flags                                                                    *
 *                                                                                             *
 * WARNINGS: Must call Set_Video_Mode 1st to create the direct draw object                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    1/12/96 9:14AM ST : Created                                                              *
 *=============================================================================================*/
unsigned Get_Video_Hardware_Capabilities(void)
{
	DDCAPS	video_capabilities;
	unsigned	video;

	/*
	** Fail if the direct draw object has not been initialised
	*/
	if (!DirectDrawObject) return (0);

	/*
	** Get the capabilities of the direct draw object
	*/
	video_capabilities.dwSize = sizeof(video_capabilities);
	//MessageBox(MainWindow, "In Get_Video_Hardware_Capabilities. About to call GetCaps","Note", MB_ICONEXCLAMATION|MB_OK);
	HRESULT result = DirectDrawObject->GetCaps (&video_capabilities, NULL);
	if (result != DD_OK){
		Process_DD_Result(result, FALSE);
		return (0);
	}

	/*
	** Set flags to indicate the presence of the features we are interested in
	*/
	video = 0;

	/* Hardware blits supported? */
	if (video_capabilities.dwCaps & DDCAPS_BLT) 				video |= VIDEO_BLITTER;

	/* Hardware blits asyncronous? */
	if (video_capabilities.dwCaps & DDCAPS_BLTQUEUE) 		video |= VIDEO_BLITTER_ASYNC;

	/* Can palette changes be synced to vertical refresh? */
	if (video_capabilities.dwCaps & DDCAPS_PALETTEVSYNC) 	video |= VIDEO_SYNC_PALETTE;

	/* Is the video cards memory bank switched? */
	if (video_capabilities.dwCaps & DDCAPS_BANKSWITCHED) 	video |= VIDEO_BANK_SWITCHED;

	/* Can the blitter do filled rectangles? */
	if (video_capabilities.dwCaps & DDCAPS_BLTCOLORFILL)	video |= VIDEO_COLOR_FILL;

	/* Is there no hardware assistance avaailable at all? */
	if (video_capabilities.dwCaps & DDCAPS_NOHARDWARE) 	video |= VIDEO_NO_HARDWARE_ASSIST;

	//MessageBox(MainWindow, "In Get_Video_Hardware_Capabilities. About to return success.","Note", MB_ICONEXCLAMATION|MB_OK);
	return (video);
}




/***********************************************************************************************
 * Wait_Vert_Blank -- Waits for the start (leading edge) of a vertical blank                   *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *=============================================================================================*/
extern int ScreenWidth;
void Wait_Vert_Blank(void)
{
	if (DirectDrawObject == NULL) {
		return;
	}
	if( ScreenWidth!=320 && CanVblankSync){
		HRESULT result = DirectDrawObject->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0);
		if (result == E_NOTIMPL){
			CanVblankSync = FALSE;
			return;
		}
		Process_DD_Result(result, FALSE);
	}
}





/***********************************************************************************************
 * Set_Palette -- set a direct draw palette                                                    *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to 768 rgb palette bytes                                                      *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    10/11/95 3:33PM ST : Created                                                             *
 *=============================================================================================*/
void Set_DD_Palette ( void *palette )
{

	/*
	** Trap null ptr
	*/
	if (!palette) return;

	int	j;
	int	k;
	char	*palette_get;

	if ( DirectDrawObject && PaletteSurface ){

		k=0;

		palette_get = (char*)palette;

		for( j=0 ; j<768 ; j+=3 )
		{
			PaletteEntries[k].peRed = (unsigned char)((*palette_get++)<<2);
			PaletteEntries[k].peGreen = (unsigned char)((*palette_get++)<<2);
			PaletteEntries[k].peBlue = (unsigned char)((*palette_get++)<<2);
			k++;
		}

		if ( !FirstPaletteSet ){
			//MessageBox(MainWindow, "In Set_DD_Palette. About to call SetPalette","Note", MB_ICONEXCLAMATION|MB_OK);
			PaletteSurface->SetPalette( PalettePtr );
			FirstPaletteSet=TRUE;
		}

		//MessageBox(MainWindow, "In Set_DD_Palette. About to call SetEntries","Note", MB_ICONEXCLAMATION|MB_OK);
		PalettePtr->SetEntries( 0 , 0 , 256 , &PaletteEntries[0] );
	}
	//MessageBox(MainWindow, "Leaving Set_DD_Palette","Note", MB_ICONEXCLAMATION|MB_OK);

}





/***********************************************************************************************
 * Wait_Blit -- waits for the DirectDraw blitter to become idle                                *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07-25-95 03:53pm ST : Created                                                             *
 *=============================================================================================*/

void Wait_Blit (void)
{
	HRESULT	return_code;

	do {
		return_code=PaletteSurface->GetBltStatus (DDGBS_ISBLTDONE);
	} while (return_code != DD_OK && return_code != DDERR_SURFACELOST);

}



/***********************************************************************************************
 * SMC::SurfaceMonitorClass -- constructor for surface monitor class                           *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    11/3/95 3:23PM ST : Created                                                              *
 *=============================================================================================*/

SurfaceMonitorClass::SurfaceMonitorClass(void)
{
	for (int i=0 ; i<MAX_SURFACES ; i++)
	{
		Surface[i]=NULL;
	}
	InFocus=FALSE;
	SurfacesRestored=FALSE;
}


/***********************************************************************************************
 * SMC::Add_DD_Surface -- add a new surface to the list                                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to surface                                                                    *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *                 11/3/95 3:24PM ST : Created                                                 *
 *=============================================================================================*/
void SurfaceMonitorClass::Add_DD_Surface (LPDIRECTDRAWSURFACE new_surface)
{
	if ( !Got_Surface_Already (new_surface) ){
		for (int i=0 ; i<MAX_SURFACES ; i++)
		{
			if ( Surface[i]==NULL ){
				Surface[i]=new_surface;
				return;
			}
		}
	}
}


/***********************************************************************************************
 * SMC::Remove_DD_Surface -- remove a direct draw surface from the list                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to Surface                                                                    *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    11/3/95 3:25PM ST : Created                                                              *
 *=============================================================================================*/

void SurfaceMonitorClass::Remove_DD_Surface (LPDIRECTDRAWSURFACE old_surface)
{
	for (int i=0 ; i<MAX_SURFACES ; i++)
	{
		if ( Surface[i]==old_surface ){
			Surface[i]=NULL;
			return;
		}
	}
}


/***********************************************************************************************
 * SMC::Got_Surface_Already -- check if a surface is already in the list                       *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to surface                                                                    *
 *                                                                                             *
 * OUTPUT:   True if surface is in list                                                        *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    11/3/95 3:25PM ST : Created                                                              *
 *=============================================================================================*/

BOOL SurfaceMonitorClass::Got_Surface_Already (LPDIRECTDRAWSURFACE test_surface)
{
	for (int i=0 ; i<MAX_SURFACES ; i++)
	{
		if ( Surface[i]==test_surface ){
			return(TRUE);
		}
	}
	return (FALSE);
}

/***********************************************************************************************
 * SMC::Restore_Surfaces -- restore the direct draw surfaces in the list                       *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    11/3/95 3:26PM ST : Created                                                              *
 *=============================================================================================*/

void SurfaceMonitorClass::Restore_Surfaces (void)
{
	if (InFocus){
		/*
		** Call restore for each Direct Draw surface
		*/
		for (int i=0 ; i<MAX_SURFACES ; i++)
		{
			if ( Surface[i] ){
				if (Surface[i]->Restore() != DD_OK){
					if (Misc_Focus_Loss_Function){
						Misc_Focus_Loss_Function();
					}
					return;
				}
			}
		}

		/*
		** PWG/ST: Now that we know all the surfaces are restored call
		** the function pointer to notify the program that it has
		** happened.  This function pointer is used to clear the pages,
		** etc.
		*/
		if (Misc_Focus_Restore_Function){
			Misc_Focus_Restore_Function();
		}

		SurfacesRestored = TRUE;

		/*
		** Restore the palette
		*/
		Set_DD_Palette (CurrentPalette);
	}
}


/***********************************************************************************************
 * SMC::Set_Surface_Focus -- set the InFocus flag to the given state                           *
 *                                                                                             *
 *  The InFocus flag is used to keep track of whether our application is currently in focus.   *
 *  We dont want to be restoring video surfaces when we are supposed to be running in the      *
 *  background.                                                                                *
 *                                                                                             *
 * INPUT:    bool in focus                                                                     *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    11/6/95 12:21PM ST : Created                                                             *
 *=============================================================================================*/

void SurfaceMonitorClass::Set_Surface_Focus ( BOOL in_focus )
{
	InFocus=in_focus;
}




/***********************************************************************************************
 * SMC::Release -- releases all direct draw surfaces                                           *
 *                                                                                             *
 *  Call this at the end of the game before called RestoreDisplayMode                          *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/6/96 12:23PM ST : Created                                                              *
 *=============================================================================================*/

void SurfaceMonitorClass::Release(void)
{
	/*
	** Call release for each Direct Draw surface
	*/
	for (int i=0 ; i<MAX_SURFACES ; i++)
	{
		if ( Surface[i] ){
			Surface[i]->Release();
			Surface[i] = 0;
		}
	}

}
