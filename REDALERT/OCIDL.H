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

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.00.39 */
/* at Sat Jul 13 21:56:58 1996
 */
/* Compiler settings for c:\oa\src\idl\ocidl.idl:
    Oic (OptLev=i1), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#include "rpc.h"
#include "rpcndr.h"
#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ocidl_h__
#define __ocidl_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IEnumConnections_FWD_DEFINED__
#define __IEnumConnections_FWD_DEFINED__
typedef interface IEnumConnections IEnumConnections;
#endif 	/* __IEnumConnections_FWD_DEFINED__ */


#ifndef __IConnectionPoint_FWD_DEFINED__
#define __IConnectionPoint_FWD_DEFINED__
typedef interface IConnectionPoint IConnectionPoint;
#endif 	/* __IConnectionPoint_FWD_DEFINED__ */


#ifndef __IEnumConnectionPoints_FWD_DEFINED__
#define __IEnumConnectionPoints_FWD_DEFINED__
typedef interface IEnumConnectionPoints IEnumConnectionPoints;
#endif 	/* __IEnumConnectionPoints_FWD_DEFINED__ */


#ifndef __IConnectionPointContainer_FWD_DEFINED__
#define __IConnectionPointContainer_FWD_DEFINED__
typedef interface IConnectionPointContainer IConnectionPointContainer;
#endif 	/* __IConnectionPointContainer_FWD_DEFINED__ */


#ifndef __IClassFactory2_FWD_DEFINED__
#define __IClassFactory2_FWD_DEFINED__
typedef interface IClassFactory2 IClassFactory2;
#endif 	/* __IClassFactory2_FWD_DEFINED__ */


#ifndef __IProvideClassInfo_FWD_DEFINED__
#define __IProvideClassInfo_FWD_DEFINED__
typedef interface IProvideClassInfo IProvideClassInfo;
#endif 	/* __IProvideClassInfo_FWD_DEFINED__ */


#ifndef __IProvideClassInfo2_FWD_DEFINED__
#define __IProvideClassInfo2_FWD_DEFINED__
typedef interface IProvideClassInfo2 IProvideClassInfo2;
#endif 	/* __IProvideClassInfo2_FWD_DEFINED__ */


#ifndef __IOleControl_FWD_DEFINED__
#define __IOleControl_FWD_DEFINED__
typedef interface IOleControl IOleControl;
#endif 	/* __IOleControl_FWD_DEFINED__ */


#ifndef __IOleControlSite_FWD_DEFINED__
#define __IOleControlSite_FWD_DEFINED__
typedef interface IOleControlSite IOleControlSite;
#endif 	/* __IOleControlSite_FWD_DEFINED__ */


#ifndef __IPropertyPage_FWD_DEFINED__
#define __IPropertyPage_FWD_DEFINED__
typedef interface IPropertyPage IPropertyPage;
#endif 	/* __IPropertyPage_FWD_DEFINED__ */


#ifndef __IPropertyPage2_FWD_DEFINED__
#define __IPropertyPage2_FWD_DEFINED__
typedef interface IPropertyPage2 IPropertyPage2;
#endif 	/* __IPropertyPage2_FWD_DEFINED__ */


#ifndef __IPropertyPageSite_FWD_DEFINED__
#define __IPropertyPageSite_FWD_DEFINED__
typedef interface IPropertyPageSite IPropertyPageSite;
#endif 	/* __IPropertyPageSite_FWD_DEFINED__ */


#ifndef __IPropertyNotifySink_FWD_DEFINED__
#define __IPropertyNotifySink_FWD_DEFINED__
typedef interface IPropertyNotifySink IPropertyNotifySink;
#endif 	/* __IPropertyNotifySink_FWD_DEFINED__ */


#ifndef __ISpecifyPropertyPages_FWD_DEFINED__
#define __ISpecifyPropertyPages_FWD_DEFINED__
typedef interface ISpecifyPropertyPages ISpecifyPropertyPages;
#endif 	/* __ISpecifyPropertyPages_FWD_DEFINED__ */


#ifndef __IPersistMemory_FWD_DEFINED__
#define __IPersistMemory_FWD_DEFINED__
typedef interface IPersistMemory IPersistMemory;
#endif 	/* __IPersistMemory_FWD_DEFINED__ */


#ifndef __IPersistStreamInit_FWD_DEFINED__
#define __IPersistStreamInit_FWD_DEFINED__
typedef interface IPersistStreamInit IPersistStreamInit;
#endif 	/* __IPersistStreamInit_FWD_DEFINED__ */


#ifndef __IPersistPropertyBag_FWD_DEFINED__
#define __IPersistPropertyBag_FWD_DEFINED__
typedef interface IPersistPropertyBag IPersistPropertyBag;
#endif 	/* __IPersistPropertyBag_FWD_DEFINED__ */


#ifndef __ISimpleFrameSite_FWD_DEFINED__
#define __ISimpleFrameSite_FWD_DEFINED__
typedef interface ISimpleFrameSite ISimpleFrameSite;
#endif 	/* __ISimpleFrameSite_FWD_DEFINED__ */


#ifndef __IFont_FWD_DEFINED__
#define __IFont_FWD_DEFINED__
typedef interface IFont IFont;
#endif 	/* __IFont_FWD_DEFINED__ */


#ifndef __IPicture_FWD_DEFINED__
#define __IPicture_FWD_DEFINED__
typedef interface IPicture IPicture;
#endif 	/* __IPicture_FWD_DEFINED__ */


#ifndef __IFontDisp_FWD_DEFINED__
#define __IFontDisp_FWD_DEFINED__
typedef interface IFontDisp IFontDisp;
#endif 	/* __IFontDisp_FWD_DEFINED__ */


#ifndef __IPictureDisp_FWD_DEFINED__
#define __IPictureDisp_FWD_DEFINED__
typedef interface IPictureDisp IPictureDisp;
#endif 	/* __IPictureDisp_FWD_DEFINED__ */


#ifndef __IAdviseSinkEx_FWD_DEFINED__
#define __IAdviseSinkEx_FWD_DEFINED__
typedef interface IAdviseSinkEx IAdviseSinkEx;
#endif 	/* __IAdviseSinkEx_FWD_DEFINED__ */


#ifndef __IOleInPlaceObjectWindowless_FWD_DEFINED__
#define __IOleInPlaceObjectWindowless_FWD_DEFINED__
typedef interface IOleInPlaceObjectWindowless IOleInPlaceObjectWindowless;
#endif 	/* __IOleInPlaceObjectWindowless_FWD_DEFINED__ */


#ifndef __IOleInPlaceSiteEx_FWD_DEFINED__
#define __IOleInPlaceSiteEx_FWD_DEFINED__
typedef interface IOleInPlaceSiteEx IOleInPlaceSiteEx;
#endif 	/* __IOleInPlaceSiteEx_FWD_DEFINED__ */


#ifndef __IOleInPlaceSiteWindowless_FWD_DEFINED__
#define __IOleInPlaceSiteWindowless_FWD_DEFINED__
typedef interface IOleInPlaceSiteWindowless IOleInPlaceSiteWindowless;
#endif 	/* __IOleInPlaceSiteWindowless_FWD_DEFINED__ */


#ifndef __IViewObjectEx_FWD_DEFINED__
#define __IViewObjectEx_FWD_DEFINED__
typedef interface IViewObjectEx IViewObjectEx;
#endif 	/* __IViewObjectEx_FWD_DEFINED__ */


#ifndef __IOleUndoUnit_FWD_DEFINED__
#define __IOleUndoUnit_FWD_DEFINED__
typedef interface IOleUndoUnit IOleUndoUnit;
#endif 	/* __IOleUndoUnit_FWD_DEFINED__ */


#ifndef __IOleParentUndoUnit_FWD_DEFINED__
#define __IOleParentUndoUnit_FWD_DEFINED__
typedef interface IOleParentUndoUnit IOleParentUndoUnit;
#endif 	/* __IOleParentUndoUnit_FWD_DEFINED__ */


#ifndef __IEnumOleUndoUnits_FWD_DEFINED__
#define __IEnumOleUndoUnits_FWD_DEFINED__
typedef interface IEnumOleUndoUnits IEnumOleUndoUnits;
#endif 	/* __IEnumOleUndoUnits_FWD_DEFINED__ */


#ifndef __IOleUndoManager_FWD_DEFINED__
#define __IOleUndoManager_FWD_DEFINED__
typedef interface IOleUndoManager IOleUndoManager;
#endif 	/* __IOleUndoManager_FWD_DEFINED__ */


#ifndef __IQuickActivate_FWD_DEFINED__
#define __IQuickActivate_FWD_DEFINED__
typedef interface IQuickActivate IQuickActivate;
#endif 	/* __IQuickActivate_FWD_DEFINED__ */


#ifndef __IPointerInactive_FWD_DEFINED__
#define __IPointerInactive_FWD_DEFINED__
typedef interface IPointerInactive IPointerInactive;
#endif 	/* __IPointerInactive_FWD_DEFINED__ */


#ifndef __IObjectWithSite_FWD_DEFINED__
#define __IObjectWithSite_FWD_DEFINED__
typedef interface IObjectWithSite IObjectWithSite;
#endif 	/* __IObjectWithSite_FWD_DEFINED__ */


#ifndef __IErrorLog_FWD_DEFINED__
#define __IErrorLog_FWD_DEFINED__
typedef interface IErrorLog IErrorLog;
#endif 	/* __IErrorLog_FWD_DEFINED__ */


#ifndef __IPropertyBag_FWD_DEFINED__
#define __IPropertyBag_FWD_DEFINED__
typedef interface IPropertyBag IPropertyBag;
#endif 	/* __IPropertyBag_FWD_DEFINED__ */


#ifndef __IPerPropertyBrowsing_FWD_DEFINED__
#define __IPerPropertyBrowsing_FWD_DEFINED__
typedef interface IPerPropertyBrowsing IPerPropertyBrowsing;
#endif 	/* __IPerPropertyBrowsing_FWD_DEFINED__ */


/* header files for imported files */
#include "oleidl.h"
#include "oaidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/****************************************
 * Generated header for interface: __MIDL__intf_0000
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [local] */ 


//+-------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1993 - 1996.
//
//--------------------------------------------------------------------------







































extern RPC_IF_HANDLE __MIDL__intf_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL__intf_0000_v0_0_s_ifspec;

#ifndef __IOleControlTypes_INTERFACE_DEFINED__
#define __IOleControlTypes_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOleControlTypes
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [auto_handle][unique][version] */ 


typedef /* [v1_enum] */ 
enum tagUASFLAGS
    {	UAS_NORMAL	= 0,
	UAS_BLOCKED	= 0x1,
	UAS_NOPARENTENABLE	= 0x2,
	UAS_MASK	= 0x3
    }	UASFLAGS;

/* State values for the DISPID_READYSTATE property */
typedef /* [v1_enum] */ 
enum tagREADYSTATE
    {	READYSTATE_UNINITIALIZED	= 0,
	READYSTATE_LOADING	= 1,
	READYSTATE_LOADED	= 2,
	READYSTATE_INTERACTIVE	= 3,
	READYSTATE_COMPLETE	= 4
    }	READYSTATE;

typedef /* [represent_as] */ struct  tagUserHWND
    {
    wireHWND pRemHwnd;
    }	UserHWND;

typedef /* [represent_as] */ struct tagUserHWND UserHACCEL;

typedef /* [represent_as] */ struct tagUserHWND UserHDC;

typedef /* [represent_as] */ struct tagUserHWND UserHFONT;

typedef /* [represent_as] */ struct  tagUserMSG
    {
    wireHWND pRemHwnd;
    UINT message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD time;
    POINT pt;
    }	UserMSG;

typedef /* [represent_as] */ struct  tagUserBSTR
    {
    wireBSTR pBstrBlob;
    }	UserBSTR;

typedef struct  tagVARIANT_BLOB
    {
    DWORD clSize;
    DWORD rpcReserved;
    /* [size_is] */ ULONGLONG ahData[ 1 ];
    }	__RPC_FAR *wireVARIANT_BLOB;

typedef /* [represent_as] */ struct  tagUserVARIANT
    {
    wireVARIANT_BLOB pVarBlob;
    }	UserVARIANT;

typedef /* [represent_as] */ struct  tagUserEXCEPINFO
    {
    WORD wCode;
    WORD wReserved;
    wireBSTR bstrSource;
    wireBSTR bstrDescription;
    wireBSTR bstrHelpFile;
    DWORD dwHelpContext;
    ULONG pvReserved;
    ULONG pfnDeferredFillIn;
    SCODE scode;
    }	UserEXCEPINFO;



extern RPC_IF_HANDLE IOleControlTypes_v1_0_c_ifspec;
extern RPC_IF_HANDLE IOleControlTypes_v1_0_s_ifspec;
#endif /* __IOleControlTypes_INTERFACE_DEFINED__ */

#ifndef __IEnumConnections_INTERFACE_DEFINED__
#define __IEnumConnections_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IEnumConnections
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IEnumConnections __RPC_FAR *PENUMCONNECTIONS;

typedef IEnumConnections __RPC_FAR *LPENUMCONNECTIONS;

typedef struct  tagCONNECTDATA
    {
    IUnknown __RPC_FAR *pUnk;
    DWORD dwCookie;
    }	CONNECTDATA;

typedef struct tagCONNECTDATA __RPC_FAR *PCONNECTDATA;

typedef struct tagCONNECTDATA __RPC_FAR *LPCONNECTDATA;


EXTERN_C const IID IID_IEnumConnections;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IEnumConnections : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Next( 
            /* [in] */ ULONG cConnections,
            /* [length_is][size_is][out] */ LPCONNECTDATA rgcd,
            /* [out] */ ULONG __RPC_FAR *pcFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG cConnections) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumConnections __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumConnectionsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEnumConnections __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEnumConnections __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEnumConnections __RPC_FAR * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Next )( 
            IEnumConnections __RPC_FAR * This,
            /* [in] */ ULONG cConnections,
            /* [length_is][size_is][out] */ LPCONNECTDATA rgcd,
            /* [out] */ ULONG __RPC_FAR *pcFetched);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Skip )( 
            IEnumConnections __RPC_FAR * This,
            /* [in] */ ULONG cConnections);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reset )( 
            IEnumConnections __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IEnumConnections __RPC_FAR * This,
            /* [out] */ IEnumConnections __RPC_FAR *__RPC_FAR *ppEnum);
        
        END_INTERFACE
    } IEnumConnectionsVtbl;

    interface IEnumConnections
    {
        CONST_VTBL struct IEnumConnectionsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumConnections_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumConnections_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumConnections_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumConnections_Next(This,cConnections,rgcd,pcFetched)	\
    (This)->lpVtbl -> Next(This,cConnections,rgcd,pcFetched)

#define IEnumConnections_Skip(This,cConnections)	\
    (This)->lpVtbl -> Skip(This,cConnections)

#define IEnumConnections_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumConnections_Clone(This,ppEnum)	\
    (This)->lpVtbl -> Clone(This,ppEnum)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT STDMETHODCALLTYPE IEnumConnections_RemoteNext_Proxy( 
    IEnumConnections __RPC_FAR * This,
    /* [in] */ ULONG cConnections,
    /* [length_is][size_is][out] */ LPCONNECTDATA rgcd,
    /* [out] */ ULONG __RPC_FAR *pcFetched);


void __RPC_STUB IEnumConnections_RemoteNext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumConnections_Skip_Proxy( 
    IEnumConnections __RPC_FAR * This,
    /* [in] */ ULONG cConnections);


void __RPC_STUB IEnumConnections_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumConnections_Reset_Proxy( 
    IEnumConnections __RPC_FAR * This);


void __RPC_STUB IEnumConnections_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumConnections_Clone_Proxy( 
    IEnumConnections __RPC_FAR * This,
    /* [out] */ IEnumConnections __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IEnumConnections_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumConnections_INTERFACE_DEFINED__ */


#ifndef __IConnectionPoint_INTERFACE_DEFINED__
#define __IConnectionPoint_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IConnectionPoint
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IConnectionPoint __RPC_FAR *PCONNECTIONPOINT;

typedef IConnectionPoint __RPC_FAR *LPCONNECTIONPOINT;


EXTERN_C const IID IID_IConnectionPoint;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IConnectionPoint : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetConnectionInterface( 
            /* [out] */ IID __RPC_FAR *pIID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetConnectionPointContainer( 
            /* [out] */ IConnectionPointContainer __RPC_FAR *__RPC_FAR *ppCPC) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Advise( 
            /* [in] */ IUnknown __RPC_FAR *pUnkSink,
            /* [out] */ DWORD __RPC_FAR *pdwCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Unadvise( 
            /* [in] */ DWORD dwCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumConnections( 
            /* [out] */ IEnumConnections __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConnectionPointVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConnectionPoint __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConnectionPoint __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConnectionPoint __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetConnectionInterface )( 
            IConnectionPoint __RPC_FAR * This,
            /* [out] */ IID __RPC_FAR *pIID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetConnectionPointContainer )( 
            IConnectionPoint __RPC_FAR * This,
            /* [out] */ IConnectionPointContainer __RPC_FAR *__RPC_FAR *ppCPC);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Advise )( 
            IConnectionPoint __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *pUnkSink,
            /* [out] */ DWORD __RPC_FAR *pdwCookie);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unadvise )( 
            IConnectionPoint __RPC_FAR * This,
            /* [in] */ DWORD dwCookie);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumConnections )( 
            IConnectionPoint __RPC_FAR * This,
            /* [out] */ IEnumConnections __RPC_FAR *__RPC_FAR *ppEnum);
        
        END_INTERFACE
    } IConnectionPointVtbl;

    interface IConnectionPoint
    {
        CONST_VTBL struct IConnectionPointVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConnectionPoint_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConnectionPoint_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConnectionPoint_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConnectionPoint_GetConnectionInterface(This,pIID)	\
    (This)->lpVtbl -> GetConnectionInterface(This,pIID)

#define IConnectionPoint_GetConnectionPointContainer(This,ppCPC)	\
    (This)->lpVtbl -> GetConnectionPointContainer(This,ppCPC)

#define IConnectionPoint_Advise(This,pUnkSink,pdwCookie)	\
    (This)->lpVtbl -> Advise(This,pUnkSink,pdwCookie)

#define IConnectionPoint_Unadvise(This,dwCookie)	\
    (This)->lpVtbl -> Unadvise(This,dwCookie)

#define IConnectionPoint_EnumConnections(This,ppEnum)	\
    (This)->lpVtbl -> EnumConnections(This,ppEnum)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConnectionPoint_GetConnectionInterface_Proxy( 
    IConnectionPoint __RPC_FAR * This,
    /* [out] */ IID __RPC_FAR *pIID);


void __RPC_STUB IConnectionPoint_GetConnectionInterface_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConnectionPoint_GetConnectionPointContainer_Proxy( 
    IConnectionPoint __RPC_FAR * This,
    /* [out] */ IConnectionPointContainer __RPC_FAR *__RPC_FAR *ppCPC);


void __RPC_STUB IConnectionPoint_GetConnectionPointContainer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConnectionPoint_Advise_Proxy( 
    IConnectionPoint __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *pUnkSink,
    /* [out] */ DWORD __RPC_FAR *pdwCookie);


void __RPC_STUB IConnectionPoint_Advise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConnectionPoint_Unadvise_Proxy( 
    IConnectionPoint __RPC_FAR * This,
    /* [in] */ DWORD dwCookie);


void __RPC_STUB IConnectionPoint_Unadvise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConnectionPoint_EnumConnections_Proxy( 
    IConnectionPoint __RPC_FAR * This,
    /* [out] */ IEnumConnections __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IConnectionPoint_EnumConnections_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConnectionPoint_INTERFACE_DEFINED__ */


#ifndef __IEnumConnectionPoints_INTERFACE_DEFINED__
#define __IEnumConnectionPoints_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IEnumConnectionPoints
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IEnumConnectionPoints __RPC_FAR *PENUMCONNECTIONPOINTS;

typedef IEnumConnectionPoints __RPC_FAR *LPENUMCONNECTIONPOINTS;


EXTERN_C const IID IID_IEnumConnectionPoints;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IEnumConnectionPoints : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Next( 
            /* [in] */ ULONG cConnections,
            /* [length_is][size_is][out] */ LPCONNECTIONPOINT __RPC_FAR *ppCP,
            /* [out] */ ULONG __RPC_FAR *pcFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG cConnections) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumConnectionPoints __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumConnectionPointsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEnumConnectionPoints __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEnumConnectionPoints __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEnumConnectionPoints __RPC_FAR * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Next )( 
            IEnumConnectionPoints __RPC_FAR * This,
            /* [in] */ ULONG cConnections,
            /* [length_is][size_is][out] */ LPCONNECTIONPOINT __RPC_FAR *ppCP,
            /* [out] */ ULONG __RPC_FAR *pcFetched);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Skip )( 
            IEnumConnectionPoints __RPC_FAR * This,
            /* [in] */ ULONG cConnections);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reset )( 
            IEnumConnectionPoints __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IEnumConnectionPoints __RPC_FAR * This,
            /* [out] */ IEnumConnectionPoints __RPC_FAR *__RPC_FAR *ppEnum);
        
        END_INTERFACE
    } IEnumConnectionPointsVtbl;

    interface IEnumConnectionPoints
    {
        CONST_VTBL struct IEnumConnectionPointsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumConnectionPoints_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumConnectionPoints_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumConnectionPoints_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumConnectionPoints_Next(This,cConnections,ppCP,pcFetched)	\
    (This)->lpVtbl -> Next(This,cConnections,ppCP,pcFetched)

#define IEnumConnectionPoints_Skip(This,cConnections)	\
    (This)->lpVtbl -> Skip(This,cConnections)

#define IEnumConnectionPoints_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumConnectionPoints_Clone(This,ppEnum)	\
    (This)->lpVtbl -> Clone(This,ppEnum)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT STDMETHODCALLTYPE IEnumConnectionPoints_RemoteNext_Proxy( 
    IEnumConnectionPoints __RPC_FAR * This,
    /* [in] */ ULONG cConnections,
    /* [length_is][size_is][out] */ LPCONNECTIONPOINT __RPC_FAR *ppCP,
    /* [out] */ ULONG __RPC_FAR *pcFetched);


void __RPC_STUB IEnumConnectionPoints_RemoteNext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumConnectionPoints_Skip_Proxy( 
    IEnumConnectionPoints __RPC_FAR * This,
    /* [in] */ ULONG cConnections);


void __RPC_STUB IEnumConnectionPoints_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumConnectionPoints_Reset_Proxy( 
    IEnumConnectionPoints __RPC_FAR * This);


void __RPC_STUB IEnumConnectionPoints_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumConnectionPoints_Clone_Proxy( 
    IEnumConnectionPoints __RPC_FAR * This,
    /* [out] */ IEnumConnectionPoints __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IEnumConnectionPoints_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumConnectionPoints_INTERFACE_DEFINED__ */


#ifndef __IConnectionPointContainer_INTERFACE_DEFINED__
#define __IConnectionPointContainer_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IConnectionPointContainer
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IConnectionPointContainer __RPC_FAR *PCONNECTIONPOINTCONTAINER;

typedef IConnectionPointContainer __RPC_FAR *LPCONNECTIONPOINTCONTAINER;


EXTERN_C const IID IID_IConnectionPointContainer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IConnectionPointContainer : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE EnumConnectionPoints( 
            /* [out] */ IEnumConnectionPoints __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindConnectionPoint( 
            /* [in] */ REFIID riid,
            /* [out] */ IConnectionPoint __RPC_FAR *__RPC_FAR *ppCP) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConnectionPointContainerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConnectionPointContainer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConnectionPointContainer __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConnectionPointContainer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumConnectionPoints )( 
            IConnectionPointContainer __RPC_FAR * This,
            /* [out] */ IEnumConnectionPoints __RPC_FAR *__RPC_FAR *ppEnum);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindConnectionPoint )( 
            IConnectionPointContainer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [out] */ IConnectionPoint __RPC_FAR *__RPC_FAR *ppCP);
        
        END_INTERFACE
    } IConnectionPointContainerVtbl;

    interface IConnectionPointContainer
    {
        CONST_VTBL struct IConnectionPointContainerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConnectionPointContainer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConnectionPointContainer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConnectionPointContainer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConnectionPointContainer_EnumConnectionPoints(This,ppEnum)	\
    (This)->lpVtbl -> EnumConnectionPoints(This,ppEnum)

#define IConnectionPointContainer_FindConnectionPoint(This,riid,ppCP)	\
    (This)->lpVtbl -> FindConnectionPoint(This,riid,ppCP)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConnectionPointContainer_EnumConnectionPoints_Proxy( 
    IConnectionPointContainer __RPC_FAR * This,
    /* [out] */ IEnumConnectionPoints __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IConnectionPointContainer_EnumConnectionPoints_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConnectionPointContainer_FindConnectionPoint_Proxy( 
    IConnectionPointContainer __RPC_FAR * This,
    /* [in] */ REFIID riid,
    /* [out] */ IConnectionPoint __RPC_FAR *__RPC_FAR *ppCP);


void __RPC_STUB IConnectionPointContainer_FindConnectionPoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConnectionPointContainer_INTERFACE_DEFINED__ */


#ifndef __IClassFactory2_INTERFACE_DEFINED__
#define __IClassFactory2_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IClassFactory2
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IClassFactory2 __RPC_FAR *LPCLASSFACTORY2;

typedef struct  tagLICINFO
    {
    LONG cbLicInfo;
    BOOL fRuntimeKeyAvail;
    BOOL fLicVerified;
    }	LICINFO;

typedef struct tagLICINFO __RPC_FAR *LPLICINFO;


EXTERN_C const IID IID_IClassFactory2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IClassFactory2 : public IClassFactory
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetLicInfo( 
            /* [out] */ LICINFO __RPC_FAR *pLicInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RequestLicKey( 
            /* [in] */ DWORD dwReserved,
            /* [out] */ BSTR __RPC_FAR *pBstrKey) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE CreateInstanceLic( 
            /* [in] */ IUnknown __RPC_FAR *pUnkOuter,
            /* [in] */ IUnknown __RPC_FAR *pUnkReserved,
            /* [in] */ REFIID riid,
            /* [in] */ BSTR bstrKey,
            /* [iid_is][out] */ PVOID __RPC_FAR *ppvObj) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IClassFactory2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IClassFactory2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IClassFactory2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IClassFactory2 __RPC_FAR * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateInstance )( 
            IClassFactory2 __RPC_FAR * This,
            /* [unique][in] */ IUnknown __RPC_FAR *pUnkOuter,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LockServer )( 
            IClassFactory2 __RPC_FAR * This,
            /* [in] */ BOOL fLock);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLicInfo )( 
            IClassFactory2 __RPC_FAR * This,
            /* [out] */ LICINFO __RPC_FAR *pLicInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RequestLicKey )( 
            IClassFactory2 __RPC_FAR * This,
            /* [in] */ DWORD dwReserved,
            /* [out] */ BSTR __RPC_FAR *pBstrKey);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateInstanceLic )( 
            IClassFactory2 __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *pUnkOuter,
            /* [in] */ IUnknown __RPC_FAR *pUnkReserved,
            /* [in] */ REFIID riid,
            /* [in] */ BSTR bstrKey,
            /* [iid_is][out] */ PVOID __RPC_FAR *ppvObj);
        
        END_INTERFACE
    } IClassFactory2Vtbl;

    interface IClassFactory2
    {
        CONST_VTBL struct IClassFactory2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IClassFactory2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IClassFactory2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IClassFactory2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IClassFactory2_CreateInstance(This,pUnkOuter,riid,ppvObject)	\
    (This)->lpVtbl -> CreateInstance(This,pUnkOuter,riid,ppvObject)

#define IClassFactory2_LockServer(This,fLock)	\
    (This)->lpVtbl -> LockServer(This,fLock)


#define IClassFactory2_GetLicInfo(This,pLicInfo)	\
    (This)->lpVtbl -> GetLicInfo(This,pLicInfo)

#define IClassFactory2_RequestLicKey(This,dwReserved,pBstrKey)	\
    (This)->lpVtbl -> RequestLicKey(This,dwReserved,pBstrKey)

#define IClassFactory2_CreateInstanceLic(This,pUnkOuter,pUnkReserved,riid,bstrKey,ppvObj)	\
    (This)->lpVtbl -> CreateInstanceLic(This,pUnkOuter,pUnkReserved,riid,bstrKey,ppvObj)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IClassFactory2_GetLicInfo_Proxy( 
    IClassFactory2 __RPC_FAR * This,
    /* [out] */ LICINFO __RPC_FAR *pLicInfo);


void __RPC_STUB IClassFactory2_GetLicInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IClassFactory2_RequestLicKey_Proxy( 
    IClassFactory2 __RPC_FAR * This,
    /* [in] */ DWORD dwReserved,
    /* [out] */ BSTR __RPC_FAR *pBstrKey);


void __RPC_STUB IClassFactory2_RequestLicKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IClassFactory2_RemoteCreateInstanceLic_Proxy( 
    IClassFactory2 __RPC_FAR * This,
    /* [in] */ REFIID riid,
    /* [in] */ BSTR bstrKey,
    /* [iid_is][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppvObj);


void __RPC_STUB IClassFactory2_RemoteCreateInstanceLic_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IClassFactory2_INTERFACE_DEFINED__ */


#ifndef __IProvideClassInfo_INTERFACE_DEFINED__
#define __IProvideClassInfo_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IProvideClassInfo
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IProvideClassInfo __RPC_FAR *LPPROVIDECLASSINFO;


EXTERN_C const IID IID_IProvideClassInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IProvideClassInfo : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetClassInfo( 
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTI) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IProvideClassInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IProvideClassInfo __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IProvideClassInfo __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IProvideClassInfo __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetClassInfo )( 
            IProvideClassInfo __RPC_FAR * This,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTI);
        
        END_INTERFACE
    } IProvideClassInfoVtbl;

    interface IProvideClassInfo
    {
        CONST_VTBL struct IProvideClassInfoVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IProvideClassInfo_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IProvideClassInfo_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IProvideClassInfo_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IProvideClassInfo_GetClassInfo(This,ppTI)	\
    (This)->lpVtbl -> GetClassInfo(This,ppTI)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IProvideClassInfo_GetClassInfo_Proxy( 
    IProvideClassInfo __RPC_FAR * This,
    /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTI);


void __RPC_STUB IProvideClassInfo_GetClassInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IProvideClassInfo_INTERFACE_DEFINED__ */


#ifndef __IProvideClassInfo2_INTERFACE_DEFINED__
#define __IProvideClassInfo2_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IProvideClassInfo2
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IProvideClassInfo2 __RPC_FAR *LPPROVIDECLASSINFO2;

typedef 
enum tagGUIDKIND
    {	GUIDKIND_DEFAULT_SOURCE_DISP_IID	= 1
    }	GUIDKIND;


EXTERN_C const IID IID_IProvideClassInfo2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IProvideClassInfo2 : public IProvideClassInfo
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetGUID( 
            /* [in] */ DWORD dwGuidKind,
            /* [out] */ GUID __RPC_FAR *pGUID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IProvideClassInfo2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IProvideClassInfo2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IProvideClassInfo2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IProvideClassInfo2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetClassInfo )( 
            IProvideClassInfo2 __RPC_FAR * This,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTI);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetGUID )( 
            IProvideClassInfo2 __RPC_FAR * This,
            /* [in] */ DWORD dwGuidKind,
            /* [out] */ GUID __RPC_FAR *pGUID);
        
        END_INTERFACE
    } IProvideClassInfo2Vtbl;

    interface IProvideClassInfo2
    {
        CONST_VTBL struct IProvideClassInfo2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IProvideClassInfo2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IProvideClassInfo2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IProvideClassInfo2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IProvideClassInfo2_GetClassInfo(This,ppTI)	\
    (This)->lpVtbl -> GetClassInfo(This,ppTI)


#define IProvideClassInfo2_GetGUID(This,dwGuidKind,pGUID)	\
    (This)->lpVtbl -> GetGUID(This,dwGuidKind,pGUID)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IProvideClassInfo2_GetGUID_Proxy( 
    IProvideClassInfo2 __RPC_FAR * This,
    /* [in] */ DWORD dwGuidKind,
    /* [out] */ GUID __RPC_FAR *pGUID);


void __RPC_STUB IProvideClassInfo2_GetGUID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IProvideClassInfo2_INTERFACE_DEFINED__ */


#ifndef __IOleControl_INTERFACE_DEFINED__
#define __IOleControl_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOleControl
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IOleControl __RPC_FAR *LPOLECONTROL;

typedef struct  tagCONTROLINFO
    {
    ULONG cb;
    HACCEL hAccel;
    USHORT cAccel;
    DWORD dwFlags;
    }	CONTROLINFO;

typedef struct tagCONTROLINFO __RPC_FAR *LPCONTROLINFO;

typedef 
enum tagCTRLINFO
    {	CTRLINFO_EATS_RETURN	= 1,
	CTRLINFO_EATS_ESCAPE	= 2
    }	CTRLINFO;


EXTERN_C const IID IID_IOleControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IOleControl : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetControlInfo( 
            /* [out] */ CONTROLINFO __RPC_FAR *pCI) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnMnemonic( 
            /* [in] */ MSG __RPC_FAR *pMsg) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnAmbientPropertyChange( 
            /* [in] */ DISPID dispID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FreezeEvents( 
            /* [in] */ BOOL bFreeze) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOleControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOleControl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOleControl __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOleControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetControlInfo )( 
            IOleControl __RPC_FAR * This,
            /* [out] */ CONTROLINFO __RPC_FAR *pCI);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnMnemonic )( 
            IOleControl __RPC_FAR * This,
            /* [in] */ MSG __RPC_FAR *pMsg);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnAmbientPropertyChange )( 
            IOleControl __RPC_FAR * This,
            /* [in] */ DISPID dispID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FreezeEvents )( 
            IOleControl __RPC_FAR * This,
            /* [in] */ BOOL bFreeze);
        
        END_INTERFACE
    } IOleControlVtbl;

    interface IOleControl
    {
        CONST_VTBL struct IOleControlVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOleControl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOleControl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOleControl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOleControl_GetControlInfo(This,pCI)	\
    (This)->lpVtbl -> GetControlInfo(This,pCI)

#define IOleControl_OnMnemonic(This,pMsg)	\
    (This)->lpVtbl -> OnMnemonic(This,pMsg)

#define IOleControl_OnAmbientPropertyChange(This,dispID)	\
    (This)->lpVtbl -> OnAmbientPropertyChange(This,dispID)

#define IOleControl_FreezeEvents(This,bFreeze)	\
    (This)->lpVtbl -> FreezeEvents(This,bFreeze)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOleControl_GetControlInfo_Proxy( 
    IOleControl __RPC_FAR * This,
    /* [out] */ CONTROLINFO __RPC_FAR *pCI);


void __RPC_STUB IOleControl_GetControlInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleControl_OnMnemonic_Proxy( 
    IOleControl __RPC_FAR * This,
    /* [in] */ MSG __RPC_FAR *pMsg);


void __RPC_STUB IOleControl_OnMnemonic_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleControl_OnAmbientPropertyChange_Proxy( 
    IOleControl __RPC_FAR * This,
    /* [in] */ DISPID dispID);


void __RPC_STUB IOleControl_OnAmbientPropertyChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleControl_FreezeEvents_Proxy( 
    IOleControl __RPC_FAR * This,
    /* [in] */ BOOL bFreeze);


void __RPC_STUB IOleControl_FreezeEvents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOleControl_INTERFACE_DEFINED__ */


#ifndef __IOleControlSite_INTERFACE_DEFINED__
#define __IOleControlSite_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOleControlSite
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IOleControlSite __RPC_FAR *LPOLECONTROLSITE;

typedef struct  tagPOINTF
    {
    FLOAT x;
    FLOAT y;
    }	POINTF;

typedef struct tagPOINTF __RPC_FAR *LPPOINTF;

typedef 
enum tagXFORMCOORDS
    {	XFORMCOORDS_POSITION	= 0x1,
	XFORMCOORDS_SIZE	= 0x2,
	XFORMCOORDS_HIMETRICTOCONTAINER	= 0x4,
	XFORMCOORDS_CONTAINERTOHIMETRIC	= 0x8
    }	XFORMCOORDS;


EXTERN_C const IID IID_IOleControlSite;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IOleControlSite : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnControlInfoChanged( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LockInPlaceActive( 
            /* [in] */ BOOL fLock) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetExtendedControl( 
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDisp) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE TransformCoords( 
            /* [out][in] */ POINTL __RPC_FAR *pPtlHimetric,
            /* [out][in] */ POINTF __RPC_FAR *pPtfContainer,
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator( 
            /* [in] */ MSG __RPC_FAR *pMsg,
            /* [in] */ DWORD grfModifiers) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnFocus( 
            /* [in] */ BOOL fGotFocus) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ShowPropertyFrame( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOleControlSiteVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOleControlSite __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOleControlSite __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOleControlSite __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnControlInfoChanged )( 
            IOleControlSite __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LockInPlaceActive )( 
            IOleControlSite __RPC_FAR * This,
            /* [in] */ BOOL fLock);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetExtendedControl )( 
            IOleControlSite __RPC_FAR * This,
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDisp);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TransformCoords )( 
            IOleControlSite __RPC_FAR * This,
            /* [out][in] */ POINTL __RPC_FAR *pPtlHimetric,
            /* [out][in] */ POINTF __RPC_FAR *pPtfContainer,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TranslateAccelerator )( 
            IOleControlSite __RPC_FAR * This,
            /* [in] */ MSG __RPC_FAR *pMsg,
            /* [in] */ DWORD grfModifiers);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnFocus )( 
            IOleControlSite __RPC_FAR * This,
            /* [in] */ BOOL fGotFocus);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowPropertyFrame )( 
            IOleControlSite __RPC_FAR * This);
        
        END_INTERFACE
    } IOleControlSiteVtbl;

    interface IOleControlSite
    {
        CONST_VTBL struct IOleControlSiteVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOleControlSite_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOleControlSite_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOleControlSite_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOleControlSite_OnControlInfoChanged(This)	\
    (This)->lpVtbl -> OnControlInfoChanged(This)

#define IOleControlSite_LockInPlaceActive(This,fLock)	\
    (This)->lpVtbl -> LockInPlaceActive(This,fLock)

#define IOleControlSite_GetExtendedControl(This,ppDisp)	\
    (This)->lpVtbl -> GetExtendedControl(This,ppDisp)

#define IOleControlSite_TransformCoords(This,pPtlHimetric,pPtfContainer,dwFlags)	\
    (This)->lpVtbl -> TransformCoords(This,pPtlHimetric,pPtfContainer,dwFlags)

#define IOleControlSite_TranslateAccelerator(This,pMsg,grfModifiers)	\
    (This)->lpVtbl -> TranslateAccelerator(This,pMsg,grfModifiers)

#define IOleControlSite_OnFocus(This,fGotFocus)	\
    (This)->lpVtbl -> OnFocus(This,fGotFocus)

#define IOleControlSite_ShowPropertyFrame(This)	\
    (This)->lpVtbl -> ShowPropertyFrame(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOleControlSite_OnControlInfoChanged_Proxy( 
    IOleControlSite __RPC_FAR * This);


void __RPC_STUB IOleControlSite_OnControlInfoChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleControlSite_LockInPlaceActive_Proxy( 
    IOleControlSite __RPC_FAR * This,
    /* [in] */ BOOL fLock);


void __RPC_STUB IOleControlSite_LockInPlaceActive_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleControlSite_GetExtendedControl_Proxy( 
    IOleControlSite __RPC_FAR * This,
    /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDisp);


void __RPC_STUB IOleControlSite_GetExtendedControl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleControlSite_TransformCoords_Proxy( 
    IOleControlSite __RPC_FAR * This,
    /* [out][in] */ POINTL __RPC_FAR *pPtlHimetric,
    /* [out][in] */ POINTF __RPC_FAR *pPtfContainer,
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IOleControlSite_TransformCoords_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleControlSite_TranslateAccelerator_Proxy( 
    IOleControlSite __RPC_FAR * This,
    /* [in] */ MSG __RPC_FAR *pMsg,
    /* [in] */ DWORD grfModifiers);


void __RPC_STUB IOleControlSite_TranslateAccelerator_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleControlSite_OnFocus_Proxy( 
    IOleControlSite __RPC_FAR * This,
    /* [in] */ BOOL fGotFocus);


void __RPC_STUB IOleControlSite_OnFocus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleControlSite_ShowPropertyFrame_Proxy( 
    IOleControlSite __RPC_FAR * This);


void __RPC_STUB IOleControlSite_ShowPropertyFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOleControlSite_INTERFACE_DEFINED__ */


#ifndef __IPropertyPage_INTERFACE_DEFINED__
#define __IPropertyPage_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IPropertyPage
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IPropertyPage __RPC_FAR *LPPROPERTYPAGE;

typedef struct  tagPROPPAGEINFO
    {
    ULONG cb;
    LPOLESTR pszTitle;
    SIZE size;
    LPOLESTR pszDocString;
    LPOLESTR pszHelpFile;
    DWORD dwHelpContext;
    }	PROPPAGEINFO;

typedef struct tagPROPPAGEINFO __RPC_FAR *LPPROPPAGEINFO;


EXTERN_C const IID IID_IPropertyPage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IPropertyPage : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetPageSite( 
            /* [in] */ IPropertyPageSite __RPC_FAR *pPageSite) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Activate( 
            /* [in] */ HWND hWndParent,
            /* [in] */ LPCRECT pRect,
            /* [in] */ BOOL bModal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Deactivate( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPageInfo( 
            /* [out] */ PROPPAGEINFO __RPC_FAR *pPageInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetObjects( 
            /* [in] */ ULONG cObjects,
            /* [size_is][in] */ IUnknown __RPC_FAR *__RPC_FAR *ppUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Show( 
            /* [in] */ UINT nCmdShow) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Move( 
            /* [in] */ LPCRECT pRect) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsPageDirty( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Apply( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Help( 
            /* [in] */ LPCOLESTR pszHelpDir) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator( 
            /* [in] */ MSG __RPC_FAR *pMsg) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPropertyPageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPropertyPage __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPropertyPage __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPropertyPage __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetPageSite )( 
            IPropertyPage __RPC_FAR * This,
            /* [in] */ IPropertyPageSite __RPC_FAR *pPageSite);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Activate )( 
            IPropertyPage __RPC_FAR * This,
            /* [in] */ HWND hWndParent,
            /* [in] */ LPCRECT pRect,
            /* [in] */ BOOL bModal);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Deactivate )( 
            IPropertyPage __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPageInfo )( 
            IPropertyPage __RPC_FAR * This,
            /* [out] */ PROPPAGEINFO __RPC_FAR *pPageInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetObjects )( 
            IPropertyPage __RPC_FAR * This,
            /* [in] */ ULONG cObjects,
            /* [size_is][in] */ IUnknown __RPC_FAR *__RPC_FAR *ppUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Show )( 
            IPropertyPage __RPC_FAR * This,
            /* [in] */ UINT nCmdShow);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Move )( 
            IPropertyPage __RPC_FAR * This,
            /* [in] */ LPCRECT pRect);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsPageDirty )( 
            IPropertyPage __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Apply )( 
            IPropertyPage __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Help )( 
            IPropertyPage __RPC_FAR * This,
            /* [in] */ LPCOLESTR pszHelpDir);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TranslateAccelerator )( 
            IPropertyPage __RPC_FAR * This,
            /* [in] */ MSG __RPC_FAR *pMsg);
        
        END_INTERFACE
    } IPropertyPageVtbl;

    interface IPropertyPage
    {
        CONST_VTBL struct IPropertyPageVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPropertyPage_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPropertyPage_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPropertyPage_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPropertyPage_SetPageSite(This,pPageSite)	\
    (This)->lpVtbl -> SetPageSite(This,pPageSite)

#define IPropertyPage_Activate(This,hWndParent,pRect,bModal)	\
    (This)->lpVtbl -> Activate(This,hWndParent,pRect,bModal)

#define IPropertyPage_Deactivate(This)	\
    (This)->lpVtbl -> Deactivate(This)

#define IPropertyPage_GetPageInfo(This,pPageInfo)	\
    (This)->lpVtbl -> GetPageInfo(This,pPageInfo)

#define IPropertyPage_SetObjects(This,cObjects,ppUnk)	\
    (This)->lpVtbl -> SetObjects(This,cObjects,ppUnk)

#define IPropertyPage_Show(This,nCmdShow)	\
    (This)->lpVtbl -> Show(This,nCmdShow)

#define IPropertyPage_Move(This,pRect)	\
    (This)->lpVtbl -> Move(This,pRect)

#define IPropertyPage_IsPageDirty(This)	\
    (This)->lpVtbl -> IsPageDirty(This)

#define IPropertyPage_Apply(This)	\
    (This)->lpVtbl -> Apply(This)

#define IPropertyPage_Help(This,pszHelpDir)	\
    (This)->lpVtbl -> Help(This,pszHelpDir)

#define IPropertyPage_TranslateAccelerator(This,pMsg)	\
    (This)->lpVtbl -> TranslateAccelerator(This,pMsg)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IPropertyPage_SetPageSite_Proxy( 
    IPropertyPage __RPC_FAR * This,
    /* [in] */ IPropertyPageSite __RPC_FAR *pPageSite);


void __RPC_STUB IPropertyPage_SetPageSite_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPropertyPage_Activate_Proxy( 
    IPropertyPage __RPC_FAR * This,
    /* [in] */ HWND hWndParent,
    /* [in] */ LPCRECT pRect,
    /* [in] */ BOOL bModal);


void __RPC_STUB IPropertyPage_Activate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPropertyPage_Deactivate_Proxy( 
    IPropertyPage __RPC_FAR * This);


void __RPC_STUB IPropertyPage_Deactivate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPropertyPage_GetPageInfo_Proxy( 
    IPropertyPage __RPC_FAR * This,
    /* [out] */ PROPPAGEINFO __RPC_FAR *pPageInfo);


void __RPC_STUB IPropertyPage_GetPageInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPropertyPage_SetObjects_Proxy( 
    IPropertyPage __RPC_FAR * This,
    /* [in] */ ULONG cObjects,
    /* [size_is][in] */ IUnknown __RPC_FAR *__RPC_FAR *ppUnk);


void __RPC_STUB IPropertyPage_SetObjects_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPropertyPage_Show_Proxy( 
    IPropertyPage __RPC_FAR * This,
    /* [in] */ UINT nCmdShow);


void __RPC_STUB IPropertyPage_Show_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPropertyPage_Move_Proxy( 
    IPropertyPage __RPC_FAR * This,
    /* [in] */ LPCRECT pRect);


void __RPC_STUB IPropertyPage_Move_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPropertyPage_IsPageDirty_Proxy( 
    IPropertyPage __RPC_FAR * This);


void __RPC_STUB IPropertyPage_IsPageDirty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPropertyPage_Apply_Proxy( 
    IPropertyPage __RPC_FAR * This);


void __RPC_STUB IPropertyPage_Apply_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPropertyPage_Help_Proxy( 
    IPropertyPage __RPC_FAR * This,
    /* [in] */ LPCOLESTR pszHelpDir);


void __RPC_STUB IPropertyPage_Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPropertyPage_TranslateAccelerator_Proxy( 
    IPropertyPage __RPC_FAR * This,
    /* [in] */ MSG __RPC_FAR *pMsg);


void __RPC_STUB IPropertyPage_TranslateAccelerator_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPropertyPage_INTERFACE_DEFINED__ */


#ifndef __IPropertyPage2_INTERFACE_DEFINED__
#define __IPropertyPage2_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IPropertyPage2
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IPropertyPage2 __RPC_FAR *LPPROPERTYPAGE2;


EXTERN_C const IID IID_IPropertyPage2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IPropertyPage2 : public IPropertyPage
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE EditProperty( 
            /* [in] */ DISPID dispID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPropertyPage2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPropertyPage2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPropertyPage2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPropertyPage2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetPageSite )( 
            IPropertyPage2 __RPC_FAR * This,
            /* [in] */ IPropertyPageSite __RPC_FAR *pPageSite);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Activate )( 
            IPropertyPage2 __RPC_FAR * This,
            /* [in] */ HWND hWndParent,
            /* [in] */ LPCRECT pRect,
            /* [in] */ BOOL bModal);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Deactivate )( 
            IPropertyPage2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPageInfo )( 
            IPropertyPage2 __RPC_FAR * This,
            /* [out] */ PROPPAGEINFO __RPC_FAR *pPageInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetObjects )( 
            IPropertyPage2 __RPC_FAR * This,
            /* [in] */ ULONG cObjects,
            /* [size_is][in] */ IUnknown __RPC_FAR *__RPC_FAR *ppUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Show )( 
            IPropertyPage2 __RPC_FAR * This,
            /* [in] */ UINT nCmdShow);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Move )( 
            IPropertyPage2 __RPC_FAR * This,
            /* [in] */ LPCRECT pRect);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsPageDirty )( 
            IPropertyPage2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Apply )( 
            IPropertyPage2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Help )( 
            IPropertyPage2 __RPC_FAR * This,
            /* [in] */ LPCOLESTR pszHelpDir);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TranslateAccelerator )( 
            IPropertyPage2 __RPC_FAR * This,
            /* [in] */ MSG __RPC_FAR *pMsg);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EditProperty )( 
            IPropertyPage2 __RPC_FAR * This,
            /* [in] */ DISPID dispID);
        
        END_INTERFACE
    } IPropertyPage2Vtbl;

    interface IPropertyPage2
    {
        CONST_VTBL struct IPropertyPage2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPropertyPage2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPropertyPage2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPropertyPage2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPropertyPage2_SetPageSite(This,pPageSite)	\
    (This)->lpVtbl -> SetPageSite(This,pPageSite)

#define IPropertyPage2_Activate(This,hWndParent,pRect,bModal)	\
    (This)->lpVtbl -> Activate(This,hWndParent,pRect,bModal)

#define IPropertyPage2_Deactivate(This)	\
    (This)->lpVtbl -> Deactivate(This)

#define IPropertyPage2_GetPageInfo(This,pPageInfo)	\
    (This)->lpVtbl -> GetPageInfo(This,pPageInfo)

#define IPropertyPage2_SetObjects(This,cObjects,ppUnk)	\
    (This)->lpVtbl -> SetObjects(This,cObjects,ppUnk)

#define IPropertyPage2_Show(This,nCmdShow)	\
    (This)->lpVtbl -> Show(This,nCmdShow)

#define IPropertyPage2_Move(This,pRect)	\
    (This)->lpVtbl -> Move(This,pRect)

#define IPropertyPage2_IsPageDirty(This)	\
    (This)->lpVtbl -> IsPageDirty(This)

#define IPropertyPage2_Apply(This)	\
    (This)->lpVtbl -> Apply(This)

#define IPropertyPage2_Help(This,pszHelpDir)	\
    (This)->lpVtbl -> Help(This,pszHelpDir)

#define IPropertyPage2_TranslateAccelerator(This,pMsg)	\
    (This)->lpVtbl -> TranslateAccelerator(This,pMsg)


#define IPropertyPage2_EditProperty(This,dispID)	\
    (This)->lpVtbl -> EditProperty(This,dispID)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IPropertyPage2_EditProperty_Proxy( 
    IPropertyPage2 __RPC_FAR * This,
    /* [in] */ DISPID dispID);


void __RPC_STUB IPropertyPage2_EditProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPropertyPage2_INTERFACE_DEFINED__ */


#ifndef __IPropertyPageSite_INTERFACE_DEFINED__
#define __IPropertyPageSite_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IPropertyPageSite
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IPropertyPageSite __RPC_FAR *LPPROPERTYPAGESITE;

typedef 
enum tagPROPPAGESTATUS
    {	PROPPAGESTATUS_DIRTY	= 0x1,
	PROPPAGESTATUS_VALIDATE	= 0x2,
	PROPPAGESTATUS_CLEAN	= 0x4
    }	PROPPAGESTATUS;


EXTERN_C const IID IID_IPropertyPageSite;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IPropertyPageSite : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnStatusChange( 
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLocaleID( 
            /* [out] */ LCID __RPC_FAR *pLocaleID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPageContainer( 
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator( 
            /* [in] */ MSG __RPC_FAR *pMsg) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPropertyPageSiteVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPropertyPageSite __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPropertyPageSite __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPropertyPageSite __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnStatusChange )( 
            IPropertyPageSite __RPC_FAR * This,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLocaleID )( 
            IPropertyPageSite __RPC_FAR * This,
            /* [out] */ LCID __RPC_FAR *pLocaleID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPageContainer )( 
            IPropertyPageSite __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TranslateAccelerator )( 
            IPropertyPageSite __RPC_FAR * This,
            /* [in] */ MSG __RPC_FAR *pMsg);
        
        END_INTERFACE
    } IPropertyPageSiteVtbl;

    interface IPropertyPageSite
    {
        CONST_VTBL struct IPropertyPageSiteVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPropertyPageSite_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPropertyPageSite_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPropertyPageSite_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPropertyPageSite_OnStatusChange(This,dwFlags)	\
    (This)->lpVtbl -> OnStatusChange(This,dwFlags)

#define IPropertyPageSite_GetLocaleID(This,pLocaleID)	\
    (This)->lpVtbl -> GetLocaleID(This,pLocaleID)

#define IPropertyPageSite_GetPageContainer(This,ppUnk)	\
    (This)->lpVtbl -> GetPageContainer(This,ppUnk)

#define IPropertyPageSite_TranslateAccelerator(This,pMsg)	\
    (This)->lpVtbl -> TranslateAccelerator(This,pMsg)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IPropertyPageSite_OnStatusChange_Proxy( 
    IPropertyPageSite __RPC_FAR * This,
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IPropertyPageSite_OnStatusChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPropertyPageSite_GetLocaleID_Proxy( 
    IPropertyPageSite __RPC_FAR * This,
    /* [out] */ LCID __RPC_FAR *pLocaleID);


void __RPC_STUB IPropertyPageSite_GetLocaleID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPropertyPageSite_GetPageContainer_Proxy( 
    IPropertyPageSite __RPC_FAR * This,
    /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppUnk);


void __RPC_STUB IPropertyPageSite_GetPageContainer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPropertyPageSite_TranslateAccelerator_Proxy( 
    IPropertyPageSite __RPC_FAR * This,
    /* [in] */ MSG __RPC_FAR *pMsg);


void __RPC_STUB IPropertyPageSite_TranslateAccelerator_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPropertyPageSite_INTERFACE_DEFINED__ */


#ifndef __IPropertyNotifySink_INTERFACE_DEFINED__
#define __IPropertyNotifySink_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IPropertyNotifySink
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IPropertyNotifySink __RPC_FAR *LPPROPERTYNOTIFYSINK;


EXTERN_C const IID IID_IPropertyNotifySink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IPropertyNotifySink : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnChanged( 
            /* [in] */ DISPID dispID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnRequestEdit( 
            /* [in] */ DISPID dispID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPropertyNotifySinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPropertyNotifySink __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPropertyNotifySink __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPropertyNotifySink __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnChanged )( 
            IPropertyNotifySink __RPC_FAR * This,
            /* [in] */ DISPID dispID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnRequestEdit )( 
            IPropertyNotifySink __RPC_FAR * This,
            /* [in] */ DISPID dispID);
        
        END_INTERFACE
    } IPropertyNotifySinkVtbl;

    interface IPropertyNotifySink
    {
        CONST_VTBL struct IPropertyNotifySinkVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPropertyNotifySink_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPropertyNotifySink_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPropertyNotifySink_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPropertyNotifySink_OnChanged(This,dispID)	\
    (This)->lpVtbl -> OnChanged(This,dispID)

#define IPropertyNotifySink_OnRequestEdit(This,dispID)	\
    (This)->lpVtbl -> OnRequestEdit(This,dispID)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IPropertyNotifySink_OnChanged_Proxy( 
    IPropertyNotifySink __RPC_FAR * This,
    /* [in] */ DISPID dispID);


void __RPC_STUB IPropertyNotifySink_OnChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPropertyNotifySink_OnRequestEdit_Proxy( 
    IPropertyNotifySink __RPC_FAR * This,
    /* [in] */ DISPID dispID);


void __RPC_STUB IPropertyNotifySink_OnRequestEdit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPropertyNotifySink_INTERFACE_DEFINED__ */


#ifndef __ISpecifyPropertyPages_INTERFACE_DEFINED__
#define __ISpecifyPropertyPages_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: ISpecifyPropertyPages
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef ISpecifyPropertyPages __RPC_FAR *LPSPECIFYPROPERTYPAGES;

typedef struct  tagCAUUID
    {
    ULONG cElems;
    /* [size_is] */ GUID __RPC_FAR *pElems;
    }	CAUUID;

typedef struct tagCAUUID __RPC_FAR *LPCAUUID;


EXTERN_C const IID IID_ISpecifyPropertyPages;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface ISpecifyPropertyPages : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetPages( 
            /* [out] */ CAUUID __RPC_FAR *pPages) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISpecifyPropertyPagesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISpecifyPropertyPages __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISpecifyPropertyPages __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISpecifyPropertyPages __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPages )( 
            ISpecifyPropertyPages __RPC_FAR * This,
            /* [out] */ CAUUID __RPC_FAR *pPages);
        
        END_INTERFACE
    } ISpecifyPropertyPagesVtbl;

    interface ISpecifyPropertyPages
    {
        CONST_VTBL struct ISpecifyPropertyPagesVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISpecifyPropertyPages_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISpecifyPropertyPages_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISpecifyPropertyPages_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISpecifyPropertyPages_GetPages(This,pPages)	\
    (This)->lpVtbl -> GetPages(This,pPages)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ISpecifyPropertyPages_GetPages_Proxy( 
    ISpecifyPropertyPages __RPC_FAR * This,
    /* [out] */ CAUUID __RPC_FAR *pPages);


void __RPC_STUB ISpecifyPropertyPages_GetPages_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISpecifyPropertyPages_INTERFACE_DEFINED__ */


#ifndef __IPersistMemory_INTERFACE_DEFINED__
#define __IPersistMemory_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IPersistMemory
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IPersistMemory __RPC_FAR *LPPERSISTMEMORY;


EXTERN_C const IID IID_IPersistMemory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IPersistMemory : public IPersist
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE IsDirty( void) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Load( 
            /* [size_is][in] */ LPVOID pMem,
            /* [in] */ ULONG cbSize) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Save( 
            /* [size_is][in] */ LPVOID pMem,
            /* [in] */ BOOL fClearDirty,
            /* [in] */ ULONG cbSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSizeMax( 
            /* [out] */ ULONG __RPC_FAR *pCbSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InitNew( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPersistMemoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPersistMemory __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPersistMemory __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPersistMemory __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetClassID )( 
            IPersistMemory __RPC_FAR * This,
            /* [out] */ CLSID __RPC_FAR *pClassID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsDirty )( 
            IPersistMemory __RPC_FAR * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Load )( 
            IPersistMemory __RPC_FAR * This,
            /* [size_is][in] */ LPVOID pMem,
            /* [in] */ ULONG cbSize);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Save )( 
            IPersistMemory __RPC_FAR * This,
            /* [size_is][in] */ LPVOID pMem,
            /* [in] */ BOOL fClearDirty,
            /* [in] */ ULONG cbSize);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSizeMax )( 
            IPersistMemory __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pCbSize);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitNew )( 
            IPersistMemory __RPC_FAR * This);
        
        END_INTERFACE
    } IPersistMemoryVtbl;

    interface IPersistMemory
    {
        CONST_VTBL struct IPersistMemoryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPersistMemory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPersistMemory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPersistMemory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPersistMemory_GetClassID(This,pClassID)	\
    (This)->lpVtbl -> GetClassID(This,pClassID)


#define IPersistMemory_IsDirty(This)	\
    (This)->lpVtbl -> IsDirty(This)

#define IPersistMemory_Load(This,pMem,cbSize)	\
    (This)->lpVtbl -> Load(This,pMem,cbSize)

#define IPersistMemory_Save(This,pMem,fClearDirty,cbSize)	\
    (This)->lpVtbl -> Save(This,pMem,fClearDirty,cbSize)

#define IPersistMemory_GetSizeMax(This,pCbSize)	\
    (This)->lpVtbl -> GetSizeMax(This,pCbSize)

#define IPersistMemory_InitNew(This)	\
    (This)->lpVtbl -> InitNew(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IPersistMemory_IsDirty_Proxy( 
    IPersistMemory __RPC_FAR * This);


void __RPC_STUB IPersistMemory_IsDirty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IPersistMemory_RemoteLoad_Proxy( 
    IPersistMemory __RPC_FAR * This,
    /* [size_is][in] */ BYTE __RPC_FAR *pMem,
    /* [in] */ ULONG cbSize);


void __RPC_STUB IPersistMemory_RemoteLoad_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IPersistMemory_RemoteSave_Proxy( 
    IPersistMemory __RPC_FAR * This,
    /* [size_is][in] */ BYTE __RPC_FAR *pMem,
    /* [in] */ BOOL fClearDirty,
    /* [in] */ ULONG cbSize);


void __RPC_STUB IPersistMemory_RemoteSave_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPersistMemory_GetSizeMax_Proxy( 
    IPersistMemory __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pCbSize);


void __RPC_STUB IPersistMemory_GetSizeMax_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPersistMemory_InitNew_Proxy( 
    IPersistMemory __RPC_FAR * This);


void __RPC_STUB IPersistMemory_InitNew_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPersistMemory_INTERFACE_DEFINED__ */


#ifndef __IPersistStreamInit_INTERFACE_DEFINED__
#define __IPersistStreamInit_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IPersistStreamInit
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IPersistStreamInit __RPC_FAR *LPPERSISTSTREAMINIT;


EXTERN_C const IID IID_IPersistStreamInit;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IPersistStreamInit : public IPersist
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE IsDirty( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ LPSTREAM pStm) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Save( 
            /* [in] */ LPSTREAM pStm,
            /* [in] */ BOOL fClearDirty) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSizeMax( 
            /* [out] */ ULARGE_INTEGER __RPC_FAR *pCbSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InitNew( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPersistStreamInitVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPersistStreamInit __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPersistStreamInit __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPersistStreamInit __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetClassID )( 
            IPersistStreamInit __RPC_FAR * This,
            /* [out] */ CLSID __RPC_FAR *pClassID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsDirty )( 
            IPersistStreamInit __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Load )( 
            IPersistStreamInit __RPC_FAR * This,
            /* [in] */ LPSTREAM pStm);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Save )( 
            IPersistStreamInit __RPC_FAR * This,
            /* [in] */ LPSTREAM pStm,
            /* [in] */ BOOL fClearDirty);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSizeMax )( 
            IPersistStreamInit __RPC_FAR * This,
            /* [out] */ ULARGE_INTEGER __RPC_FAR *pCbSize);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitNew )( 
            IPersistStreamInit __RPC_FAR * This);
        
        END_INTERFACE
    } IPersistStreamInitVtbl;

    interface IPersistStreamInit
    {
        CONST_VTBL struct IPersistStreamInitVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPersistStreamInit_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPersistStreamInit_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPersistStreamInit_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPersistStreamInit_GetClassID(This,pClassID)	\
    (This)->lpVtbl -> GetClassID(This,pClassID)


#define IPersistStreamInit_IsDirty(This)	\
    (This)->lpVtbl -> IsDirty(This)

#define IPersistStreamInit_Load(This,pStm)	\
    (This)->lpVtbl -> Load(This,pStm)

#define IPersistStreamInit_Save(This,pStm,fClearDirty)	\
    (This)->lpVtbl -> Save(This,pStm,fClearDirty)

#define IPersistStreamInit_GetSizeMax(This,pCbSize)	\
    (This)->lpVtbl -> GetSizeMax(This,pCbSize)

#define IPersistStreamInit_InitNew(This)	\
    (This)->lpVtbl -> InitNew(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IPersistStreamInit_IsDirty_Proxy( 
    IPersistStreamInit __RPC_FAR * This);


void __RPC_STUB IPersistStreamInit_IsDirty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPersistStreamInit_Load_Proxy( 
    IPersistStreamInit __RPC_FAR * This,
    /* [in] */ LPSTREAM pStm);


void __RPC_STUB IPersistStreamInit_Load_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPersistStreamInit_Save_Proxy( 
    IPersistStreamInit __RPC_FAR * This,
    /* [in] */ LPSTREAM pStm,
    /* [in] */ BOOL fClearDirty);


void __RPC_STUB IPersistStreamInit_Save_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPersistStreamInit_GetSizeMax_Proxy( 
    IPersistStreamInit __RPC_FAR * This,
    /* [out] */ ULARGE_INTEGER __RPC_FAR *pCbSize);


void __RPC_STUB IPersistStreamInit_GetSizeMax_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPersistStreamInit_InitNew_Proxy( 
    IPersistStreamInit __RPC_FAR * This);


void __RPC_STUB IPersistStreamInit_InitNew_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPersistStreamInit_INTERFACE_DEFINED__ */


#ifndef __IPersistPropertyBag_INTERFACE_DEFINED__
#define __IPersistPropertyBag_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IPersistPropertyBag
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IPersistPropertyBag __RPC_FAR *LPPERSISTPROPERTYBAG;


EXTERN_C const IID IID_IPersistPropertyBag;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IPersistPropertyBag : public IPersist
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE InitNew( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ IPropertyBag __RPC_FAR *pPropBag,
            /* [in] */ IErrorLog __RPC_FAR *pErrorLog) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Save( 
            /* [in] */ IPropertyBag __RPC_FAR *pPropBag,
            /* [in] */ BOOL fClearDirty,
            /* [in] */ BOOL fSaveAllProperties) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPersistPropertyBagVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPersistPropertyBag __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPersistPropertyBag __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPersistPropertyBag __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetClassID )( 
            IPersistPropertyBag __RPC_FAR * This,
            /* [out] */ CLSID __RPC_FAR *pClassID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitNew )( 
            IPersistPropertyBag __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Load )( 
            IPersistPropertyBag __RPC_FAR * This,
            /* [in] */ IPropertyBag __RPC_FAR *pPropBag,
            /* [in] */ IErrorLog __RPC_FAR *pErrorLog);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Save )( 
            IPersistPropertyBag __RPC_FAR * This,
            /* [in] */ IPropertyBag __RPC_FAR *pPropBag,
            /* [in] */ BOOL fClearDirty,
            /* [in] */ BOOL fSaveAllProperties);
        
        END_INTERFACE
    } IPersistPropertyBagVtbl;

    interface IPersistPropertyBag
    {
        CONST_VTBL struct IPersistPropertyBagVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPersistPropertyBag_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPersistPropertyBag_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPersistPropertyBag_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPersistPropertyBag_GetClassID(This,pClassID)	\
    (This)->lpVtbl -> GetClassID(This,pClassID)


#define IPersistPropertyBag_InitNew(This)	\
    (This)->lpVtbl -> InitNew(This)

#define IPersistPropertyBag_Load(This,pPropBag,pErrorLog)	\
    (This)->lpVtbl -> Load(This,pPropBag,pErrorLog)

#define IPersistPropertyBag_Save(This,pPropBag,fClearDirty,fSaveAllProperties)	\
    (This)->lpVtbl -> Save(This,pPropBag,fClearDirty,fSaveAllProperties)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IPersistPropertyBag_InitNew_Proxy( 
    IPersistPropertyBag __RPC_FAR * This);


void __RPC_STUB IPersistPropertyBag_InitNew_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPersistPropertyBag_Load_Proxy( 
    IPersistPropertyBag __RPC_FAR * This,
    /* [in] */ IPropertyBag __RPC_FAR *pPropBag,
    /* [in] */ IErrorLog __RPC_FAR *pErrorLog);


void __RPC_STUB IPersistPropertyBag_Load_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPersistPropertyBag_Save_Proxy( 
    IPersistPropertyBag __RPC_FAR * This,
    /* [in] */ IPropertyBag __RPC_FAR *pPropBag,
    /* [in] */ BOOL fClearDirty,
    /* [in] */ BOOL fSaveAllProperties);


void __RPC_STUB IPersistPropertyBag_Save_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPersistPropertyBag_INTERFACE_DEFINED__ */


#ifndef __ISimpleFrameSite_INTERFACE_DEFINED__
#define __ISimpleFrameSite_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: ISimpleFrameSite
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef ISimpleFrameSite __RPC_FAR *LPSIMPLEFRAMESITE;


EXTERN_C const IID IID_ISimpleFrameSite;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface ISimpleFrameSite : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE PreMessageFilter( 
            /* [in] */ HWND hWnd,
            /* [in] */ UINT msg,
            /* [in] */ WPARAM wp,
            /* [in] */ LPARAM lp,
            /* [out] */ LRESULT __RPC_FAR *plResult,
            /* [out] */ DWORD __RPC_FAR *pdwCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PostMessageFilter( 
            /* [in] */ HWND hWnd,
            /* [in] */ UINT msg,
            /* [in] */ WPARAM wp,
            /* [in] */ LPARAM lp,
            /* [out] */ LRESULT __RPC_FAR *plResult,
            /* [in] */ DWORD dwCookie) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISimpleFrameSiteVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISimpleFrameSite __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISimpleFrameSite __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISimpleFrameSite __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PreMessageFilter )( 
            ISimpleFrameSite __RPC_FAR * This,
            /* [in] */ HWND hWnd,
            /* [in] */ UINT msg,
            /* [in] */ WPARAM wp,
            /* [in] */ LPARAM lp,
            /* [out] */ LRESULT __RPC_FAR *plResult,
            /* [out] */ DWORD __RPC_FAR *pdwCookie);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PostMessageFilter )( 
            ISimpleFrameSite __RPC_FAR * This,
            /* [in] */ HWND hWnd,
            /* [in] */ UINT msg,
            /* [in] */ WPARAM wp,
            /* [in] */ LPARAM lp,
            /* [out] */ LRESULT __RPC_FAR *plResult,
            /* [in] */ DWORD dwCookie);
        
        END_INTERFACE
    } ISimpleFrameSiteVtbl;

    interface ISimpleFrameSite
    {
        CONST_VTBL struct ISimpleFrameSiteVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISimpleFrameSite_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISimpleFrameSite_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISimpleFrameSite_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISimpleFrameSite_PreMessageFilter(This,hWnd,msg,wp,lp,plResult,pdwCookie)	\
    (This)->lpVtbl -> PreMessageFilter(This,hWnd,msg,wp,lp,plResult,pdwCookie)

#define ISimpleFrameSite_PostMessageFilter(This,hWnd,msg,wp,lp,plResult,dwCookie)	\
    (This)->lpVtbl -> PostMessageFilter(This,hWnd,msg,wp,lp,plResult,dwCookie)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ISimpleFrameSite_PreMessageFilter_Proxy( 
    ISimpleFrameSite __RPC_FAR * This,
    /* [in] */ HWND hWnd,
    /* [in] */ UINT msg,
    /* [in] */ WPARAM wp,
    /* [in] */ LPARAM lp,
    /* [out] */ LRESULT __RPC_FAR *plResult,
    /* [out] */ DWORD __RPC_FAR *pdwCookie);


void __RPC_STUB ISimpleFrameSite_PreMessageFilter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISimpleFrameSite_PostMessageFilter_Proxy( 
    ISimpleFrameSite __RPC_FAR * This,
    /* [in] */ HWND hWnd,
    /* [in] */ UINT msg,
    /* [in] */ WPARAM wp,
    /* [in] */ LPARAM lp,
    /* [out] */ LRESULT __RPC_FAR *plResult,
    /* [in] */ DWORD dwCookie);


void __RPC_STUB ISimpleFrameSite_PostMessageFilter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISimpleFrameSite_INTERFACE_DEFINED__ */


#ifndef __IFont_INTERFACE_DEFINED__
#define __IFont_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IFont
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IFont __RPC_FAR *LPFONT;

#if defined(_WIN32) && !defined(OLE2ANSI)
typedef TEXTMETRICW TEXTMETRICOLE;

#else
typedef TEXTMETRIC TEXTMETRICOLE;
#endif
typedef TEXTMETRICOLE __RPC_FAR *LPTEXTMETRICOLE;


EXTERN_C const IID IID_IFont;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IFont : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE get_Name( 
            /* [out] */ BSTR __RPC_FAR *pName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR name) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Size( 
            /* [out] */ CY __RPC_FAR *pSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_Size( 
            /* [in] */ CY size) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Bold( 
            /* [out] */ BOOL __RPC_FAR *pBold) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_Bold( 
            /* [in] */ BOOL bold) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Italic( 
            /* [out] */ BOOL __RPC_FAR *pItalic) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_Italic( 
            /* [in] */ BOOL italic) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Underline( 
            /* [out] */ BOOL __RPC_FAR *pUnderline) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_Underline( 
            /* [in] */ BOOL underline) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Strikethrough( 
            /* [out] */ BOOL __RPC_FAR *pStrikethrough) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_Strikethrough( 
            /* [in] */ BOOL strikethrough) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Weight( 
            /* [out] */ SHORT __RPC_FAR *pWeight) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_Weight( 
            /* [in] */ SHORT weight) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Charset( 
            /* [out] */ SHORT __RPC_FAR *pCharset) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_Charset( 
            /* [in] */ SHORT charset) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_hFont( 
            /* [out] */ HFONT __RPC_FAR *phFont) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IFont __RPC_FAR *__RPC_FAR *ppFont) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsEqual( 
            /* [in] */ IFont __RPC_FAR *pFontOther) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetRatio( 
            /* [in] */ LONG cyLogical,
            /* [in] */ LONG cyHimetric) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryTextMetrics( 
            /* [out] */ TEXTMETRICOLE __RPC_FAR *pTM) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddRefHfont( 
            /* [in] */ HFONT hFont) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReleaseHfont( 
            /* [in] */ HFONT hFont) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetHdc( 
            /* [in] */ HDC hDC) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFontVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFont __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFont __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFont __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IFont __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *pName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            IFont __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Size )( 
            IFont __RPC_FAR * This,
            /* [out] */ CY __RPC_FAR *pSize);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Size )( 
            IFont __RPC_FAR * This,
            /* [in] */ CY size);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Bold )( 
            IFont __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pBold);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Bold )( 
            IFont __RPC_FAR * This,
            /* [in] */ BOOL bold);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Italic )( 
            IFont __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pItalic);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Italic )( 
            IFont __RPC_FAR * This,
            /* [in] */ BOOL italic);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Underline )( 
            IFont __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pUnderline);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Underline )( 
            IFont __RPC_FAR * This,
            /* [in] */ BOOL underline);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Strikethrough )( 
            IFont __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pStrikethrough);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Strikethrough )( 
            IFont __RPC_FAR * This,
            /* [in] */ BOOL strikethrough);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Weight )( 
            IFont __RPC_FAR * This,
            /* [out] */ SHORT __RPC_FAR *pWeight);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Weight )( 
            IFont __RPC_FAR * This,
            /* [in] */ SHORT weight);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Charset )( 
            IFont __RPC_FAR * This,
            /* [out] */ SHORT __RPC_FAR *pCharset);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Charset )( 
            IFont __RPC_FAR * This,
            /* [in] */ SHORT charset);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hFont )( 
            IFont __RPC_FAR * This,
            /* [out] */ HFONT __RPC_FAR *phFont);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IFont __RPC_FAR * This,
            /* [out] */ IFont __RPC_FAR *__RPC_FAR *ppFont);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsEqual )( 
            IFont __RPC_FAR * This,
            /* [in] */ IFont __RPC_FAR *pFontOther);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetRatio )( 
            IFont __RPC_FAR * This,
            /* [in] */ LONG cyLogical,
            /* [in] */ LONG cyHimetric);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryTextMetrics )( 
            IFont __RPC_FAR * This,
            /* [out] */ TEXTMETRICOLE __RPC_FAR *pTM);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddRefHfont )( 
            IFont __RPC_FAR * This,
            /* [in] */ HFONT hFont);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReleaseHfont )( 
            IFont __RPC_FAR * This,
            /* [in] */ HFONT hFont);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetHdc )( 
            IFont __RPC_FAR * This,
            /* [in] */ HDC hDC);
        
        END_INTERFACE
    } IFontVtbl;

    interface IFont
    {
        CONST_VTBL struct IFontVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFont_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFont_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFont_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFont_get_Name(This,pName)	\
    (This)->lpVtbl -> get_Name(This,pName)

#define IFont_put_Name(This,name)	\
    (This)->lpVtbl -> put_Name(This,name)

#define IFont_get_Size(This,pSize)	\
    (This)->lpVtbl -> get_Size(This,pSize)

#define IFont_put_Size(This,size)	\
    (This)->lpVtbl -> put_Size(This,size)

#define IFont_get_Bold(This,pBold)	\
    (This)->lpVtbl -> get_Bold(This,pBold)

#define IFont_put_Bold(This,bold)	\
    (This)->lpVtbl -> put_Bold(This,bold)

#define IFont_get_Italic(This,pItalic)	\
    (This)->lpVtbl -> get_Italic(This,pItalic)

#define IFont_put_Italic(This,italic)	\
    (This)->lpVtbl -> put_Italic(This,italic)

#define IFont_get_Underline(This,pUnderline)	\
    (This)->lpVtbl -> get_Underline(This,pUnderline)

#define IFont_put_Underline(This,underline)	\
    (This)->lpVtbl -> put_Underline(This,underline)

#define IFont_get_Strikethrough(This,pStrikethrough)	\
    (This)->lpVtbl -> get_Strikethrough(This,pStrikethrough)

#define IFont_put_Strikethrough(This,strikethrough)	\
    (This)->lpVtbl -> put_Strikethrough(This,strikethrough)

#define IFont_get_Weight(This,pWeight)	\
    (This)->lpVtbl -> get_Weight(This,pWeight)

#define IFont_put_Weight(This,weight)	\
    (This)->lpVtbl -> put_Weight(This,weight)

#define IFont_get_Charset(This,pCharset)	\
    (This)->lpVtbl -> get_Charset(This,pCharset)

#define IFont_put_Charset(This,charset)	\
    (This)->lpVtbl -> put_Charset(This,charset)

#define IFont_get_hFont(This,phFont)	\
    (This)->lpVtbl -> get_hFont(This,phFont)

#define IFont_Clone(This,ppFont)	\
    (This)->lpVtbl -> Clone(This,ppFont)

#define IFont_IsEqual(This,pFontOther)	\
    (This)->lpVtbl -> IsEqual(This,pFontOther)

#define IFont_SetRatio(This,cyLogical,cyHimetric)	\
    (This)->lpVtbl -> SetRatio(This,cyLogical,cyHimetric)

#define IFont_QueryTextMetrics(This,pTM)	\
    (This)->lpVtbl -> QueryTextMetrics(This,pTM)

#define IFont_AddRefHfont(This,hFont)	\
    (This)->lpVtbl -> AddRefHfont(This,hFont)

#define IFont_ReleaseHfont(This,hFont)	\
    (This)->lpVtbl -> ReleaseHfont(This,hFont)

#define IFont_SetHdc(This,hDC)	\
    (This)->lpVtbl -> SetHdc(This,hDC)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IFont_get_Name_Proxy( 
    IFont __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *pName);


void __RPC_STUB IFont_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_put_Name_Proxy( 
    IFont __RPC_FAR * This,
    /* [in] */ BSTR name);


void __RPC_STUB IFont_put_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_get_Size_Proxy( 
    IFont __RPC_FAR * This,
    /* [out] */ CY __RPC_FAR *pSize);


void __RPC_STUB IFont_get_Size_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_put_Size_Proxy( 
    IFont __RPC_FAR * This,
    /* [in] */ CY size);


void __RPC_STUB IFont_put_Size_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_get_Bold_Proxy( 
    IFont __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pBold);


void __RPC_STUB IFont_get_Bold_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_put_Bold_Proxy( 
    IFont __RPC_FAR * This,
    /* [in] */ BOOL bold);


void __RPC_STUB IFont_put_Bold_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_get_Italic_Proxy( 
    IFont __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pItalic);


void __RPC_STUB IFont_get_Italic_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_put_Italic_Proxy( 
    IFont __RPC_FAR * This,
    /* [in] */ BOOL italic);


void __RPC_STUB IFont_put_Italic_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_get_Underline_Proxy( 
    IFont __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pUnderline);


void __RPC_STUB IFont_get_Underline_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_put_Underline_Proxy( 
    IFont __RPC_FAR * This,
    /* [in] */ BOOL underline);


void __RPC_STUB IFont_put_Underline_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_get_Strikethrough_Proxy( 
    IFont __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pStrikethrough);


void __RPC_STUB IFont_get_Strikethrough_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_put_Strikethrough_Proxy( 
    IFont __RPC_FAR * This,
    /* [in] */ BOOL strikethrough);


void __RPC_STUB IFont_put_Strikethrough_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_get_Weight_Proxy( 
    IFont __RPC_FAR * This,
    /* [out] */ SHORT __RPC_FAR *pWeight);


void __RPC_STUB IFont_get_Weight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_put_Weight_Proxy( 
    IFont __RPC_FAR * This,
    /* [in] */ SHORT weight);


void __RPC_STUB IFont_put_Weight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_get_Charset_Proxy( 
    IFont __RPC_FAR * This,
    /* [out] */ SHORT __RPC_FAR *pCharset);


void __RPC_STUB IFont_get_Charset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_put_Charset_Proxy( 
    IFont __RPC_FAR * This,
    /* [in] */ SHORT charset);


void __RPC_STUB IFont_put_Charset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_get_hFont_Proxy( 
    IFont __RPC_FAR * This,
    /* [out] */ HFONT __RPC_FAR *phFont);


void __RPC_STUB IFont_get_hFont_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_Clone_Proxy( 
    IFont __RPC_FAR * This,
    /* [out] */ IFont __RPC_FAR *__RPC_FAR *ppFont);


void __RPC_STUB IFont_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_IsEqual_Proxy( 
    IFont __RPC_FAR * This,
    /* [in] */ IFont __RPC_FAR *pFontOther);


void __RPC_STUB IFont_IsEqual_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_SetRatio_Proxy( 
    IFont __RPC_FAR * This,
    /* [in] */ LONG cyLogical,
    /* [in] */ LONG cyHimetric);


void __RPC_STUB IFont_SetRatio_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_QueryTextMetrics_Proxy( 
    IFont __RPC_FAR * This,
    /* [out] */ TEXTMETRICOLE __RPC_FAR *pTM);


void __RPC_STUB IFont_QueryTextMetrics_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_AddRefHfont_Proxy( 
    IFont __RPC_FAR * This,
    /* [in] */ HFONT hFont);


void __RPC_STUB IFont_AddRefHfont_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_ReleaseHfont_Proxy( 
    IFont __RPC_FAR * This,
    /* [in] */ HFONT hFont);


void __RPC_STUB IFont_ReleaseHfont_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFont_SetHdc_Proxy( 
    IFont __RPC_FAR * This,
    /* [in] */ HDC hDC);


void __RPC_STUB IFont_SetHdc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFont_INTERFACE_DEFINED__ */


#ifndef __IPicture_INTERFACE_DEFINED__
#define __IPicture_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IPicture
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IPicture __RPC_FAR *LPPICTURE;

typedef 
enum tagPictureAttributes
    {	PICTURE_SCALABLE	= 0x1,
	PICTURE_TRANSPARENT	= 0x2
    }	PICTUREATTRIBUTES;

typedef UINT OLE_HANDLE;

typedef LONG OLE_XPOS_HIMETRIC;

typedef LONG OLE_YPOS_HIMETRIC;

typedef LONG OLE_XSIZE_HIMETRIC;

typedef LONG OLE_YSIZE_HIMETRIC;


EXTERN_C const IID IID_IPicture;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IPicture : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE get_Handle( 
            /* [out] */ OLE_HANDLE __RPC_FAR *pHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_hPal( 
            /* [out] */ OLE_HANDLE __RPC_FAR *phPal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Type( 
            /* [out] */ SHORT __RPC_FAR *pType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Width( 
            /* [out] */ OLE_XSIZE_HIMETRIC __RPC_FAR *pWidth) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Height( 
            /* [out] */ OLE_YSIZE_HIMETRIC __RPC_FAR *pHeight) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Render( 
            /* [in] */ HDC hDC,
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [in] */ LONG cx,
            /* [in] */ LONG cy,
            /* [in] */ OLE_XPOS_HIMETRIC xSrc,
            /* [in] */ OLE_YPOS_HIMETRIC ySrc,
            /* [in] */ OLE_XSIZE_HIMETRIC cxSrc,
            /* [in] */ OLE_YSIZE_HIMETRIC cySrc,
            /* [in] */ LPCRECT pRcWBounds) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE set_hPal( 
            /* [in] */ OLE_HANDLE hPal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_CurDC( 
            /* [out] */ HDC __RPC_FAR *phDC) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectPicture( 
            /* [in] */ HDC hDCIn,
            /* [out] */ HDC __RPC_FAR *phDCOut,
            /* [out] */ OLE_HANDLE __RPC_FAR *phBmpOut) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_KeepOriginalFormat( 
            /* [out] */ BOOL __RPC_FAR *pKeep) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_KeepOriginalFormat( 
            /* [in] */ BOOL keep) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PictureChanged( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SaveAsFile( 
            /* [in] */ LPSTREAM pStream,
            /* [in] */ BOOL fSaveMemCopy,
            /* [out] */ LONG __RPC_FAR *pCbSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Attributes( 
            /* [out] */ DWORD __RPC_FAR *pDwAttr) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPictureVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPicture __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPicture __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPicture __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Handle )( 
            IPicture __RPC_FAR * This,
            /* [out] */ OLE_HANDLE __RPC_FAR *pHandle);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hPal )( 
            IPicture __RPC_FAR * This,
            /* [out] */ OLE_HANDLE __RPC_FAR *phPal);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IPicture __RPC_FAR * This,
            /* [out] */ SHORT __RPC_FAR *pType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Width )( 
            IPicture __RPC_FAR * This,
            /* [out] */ OLE_XSIZE_HIMETRIC __RPC_FAR *pWidth);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Height )( 
            IPicture __RPC_FAR * This,
            /* [out] */ OLE_YSIZE_HIMETRIC __RPC_FAR *pHeight);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Render )( 
            IPicture __RPC_FAR * This,
            /* [in] */ HDC hDC,
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [in] */ LONG cx,
            /* [in] */ LONG cy,
            /* [in] */ OLE_XPOS_HIMETRIC xSrc,
            /* [in] */ OLE_YPOS_HIMETRIC ySrc,
            /* [in] */ OLE_XSIZE_HIMETRIC cxSrc,
            /* [in] */ OLE_YSIZE_HIMETRIC cySrc,
            /* [in] */ LPCRECT pRcWBounds);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set_hPal )( 
            IPicture __RPC_FAR * This,
            /* [in] */ OLE_HANDLE hPal);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CurDC )( 
            IPicture __RPC_FAR * This,
            /* [out] */ HDC __RPC_FAR *phDC);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectPicture )( 
            IPicture __RPC_FAR * This,
            /* [in] */ HDC hDCIn,
            /* [out] */ HDC __RPC_FAR *phDCOut,
            /* [out] */ OLE_HANDLE __RPC_FAR *phBmpOut);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_KeepOriginalFormat )( 
            IPicture __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pKeep);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_KeepOriginalFormat )( 
            IPicture __RPC_FAR * This,
            /* [in] */ BOOL keep);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PictureChanged )( 
            IPicture __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveAsFile )( 
            IPicture __RPC_FAR * This,
            /* [in] */ LPSTREAM pStream,
            /* [in] */ BOOL fSaveMemCopy,
            /* [out] */ LONG __RPC_FAR *pCbSize);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Attributes )( 
            IPicture __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pDwAttr);
        
        END_INTERFACE
    } IPictureVtbl;

    interface IPicture
    {
        CONST_VTBL struct IPictureVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPicture_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPicture_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPicture_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPicture_get_Handle(This,pHandle)	\
    (This)->lpVtbl -> get_Handle(This,pHandle)

#define IPicture_get_hPal(This,phPal)	\
    (This)->lpVtbl -> get_hPal(This,phPal)

#define IPicture_get_Type(This,pType)	\
    (This)->lpVtbl -> get_Type(This,pType)

#define IPicture_get_Width(This,pWidth)	\
    (This)->lpVtbl -> get_Width(This,pWidth)

#define IPicture_get_Height(This,pHeight)	\
    (This)->lpVtbl -> get_Height(This,pHeight)

#define IPicture_Render(This,hDC,x,y,cx,cy,xSrc,ySrc,cxSrc,cySrc,pRcWBounds)	\
    (This)->lpVtbl -> Render(This,hDC,x,y,cx,cy,xSrc,ySrc,cxSrc,cySrc,pRcWBounds)

#define IPicture_set_hPal(This,hPal)	\
    (This)->lpVtbl -> set_hPal(This,hPal)

#define IPicture_get_CurDC(This,phDC)	\
    (This)->lpVtbl -> get_CurDC(This,phDC)

#define IPicture_SelectPicture(This,hDCIn,phDCOut,phBmpOut)	\
    (This)->lpVtbl -> SelectPicture(This,hDCIn,phDCOut,phBmpOut)

#define IPicture_get_KeepOriginalFormat(This,pKeep)	\
    (This)->lpVtbl -> get_KeepOriginalFormat(This,pKeep)

#define IPicture_put_KeepOriginalFormat(This,keep)	\
    (This)->lpVtbl -> put_KeepOriginalFormat(This,keep)

#define IPicture_PictureChanged(This)	\
    (This)->lpVtbl -> PictureChanged(This)

#define IPicture_SaveAsFile(This,pStream,fSaveMemCopy,pCbSize)	\
    (This)->lpVtbl -> SaveAsFile(This,pStream,fSaveMemCopy,pCbSize)

#define IPicture_get_Attributes(This,pDwAttr)	\
    (This)->lpVtbl -> get_Attributes(This,pDwAttr)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IPicture_get_Handle_Proxy( 
    IPicture __RPC_FAR * This,
    /* [out] */ OLE_HANDLE __RPC_FAR *pHandle);


void __RPC_STUB IPicture_get_Handle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPicture_get_hPal_Proxy( 
    IPicture __RPC_FAR * This,
    /* [out] */ OLE_HANDLE __RPC_FAR *phPal);


void __RPC_STUB IPicture_get_hPal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPicture_get_Type_Proxy( 
    IPicture __RPC_FAR * This,
    /* [out] */ SHORT __RPC_FAR *pType);


void __RPC_STUB IPicture_get_Type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPicture_get_Width_Proxy( 
    IPicture __RPC_FAR * This,
    /* [out] */ OLE_XSIZE_HIMETRIC __RPC_FAR *pWidth);


void __RPC_STUB IPicture_get_Width_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPicture_get_Height_Proxy( 
    IPicture __RPC_FAR * This,
    /* [out] */ OLE_YSIZE_HIMETRIC __RPC_FAR *pHeight);


void __RPC_STUB IPicture_get_Height_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPicture_Render_Proxy( 
    IPicture __RPC_FAR * This,
    /* [in] */ HDC hDC,
    /* [in] */ LONG x,
    /* [in] */ LONG y,
    /* [in] */ LONG cx,
    /* [in] */ LONG cy,
    /* [in] */ OLE_XPOS_HIMETRIC xSrc,
    /* [in] */ OLE_YPOS_HIMETRIC ySrc,
    /* [in] */ OLE_XSIZE_HIMETRIC cxSrc,
    /* [in] */ OLE_YSIZE_HIMETRIC cySrc,
    /* [in] */ LPCRECT pRcWBounds);


void __RPC_STUB IPicture_Render_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPicture_set_hPal_Proxy( 
    IPicture __RPC_FAR * This,
    /* [in] */ OLE_HANDLE hPal);


void __RPC_STUB IPicture_set_hPal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPicture_get_CurDC_Proxy( 
    IPicture __RPC_FAR * This,
    /* [out] */ HDC __RPC_FAR *phDC);


void __RPC_STUB IPicture_get_CurDC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPicture_SelectPicture_Proxy( 
    IPicture __RPC_FAR * This,
    /* [in] */ HDC hDCIn,
    /* [out] */ HDC __RPC_FAR *phDCOut,
    /* [out] */ OLE_HANDLE __RPC_FAR *phBmpOut);


void __RPC_STUB IPicture_SelectPicture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPicture_get_KeepOriginalFormat_Proxy( 
    IPicture __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pKeep);


void __RPC_STUB IPicture_get_KeepOriginalFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPicture_put_KeepOriginalFormat_Proxy( 
    IPicture __RPC_FAR * This,
    /* [in] */ BOOL keep);


void __RPC_STUB IPicture_put_KeepOriginalFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPicture_PictureChanged_Proxy( 
    IPicture __RPC_FAR * This);


void __RPC_STUB IPicture_PictureChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPicture_SaveAsFile_Proxy( 
    IPicture __RPC_FAR * This,
    /* [in] */ LPSTREAM pStream,
    /* [in] */ BOOL fSaveMemCopy,
    /* [out] */ LONG __RPC_FAR *pCbSize);


void __RPC_STUB IPicture_SaveAsFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPicture_get_Attributes_Proxy( 
    IPicture __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pDwAttr);


void __RPC_STUB IPicture_get_Attributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPicture_INTERFACE_DEFINED__ */


#ifndef __IFontDisp_INTERFACE_DEFINED__
#define __IFontDisp_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IFontDisp
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IFontDisp __RPC_FAR *LPFONTDISP;


EXTERN_C const IID IID_IFontDisp;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IFontDisp : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IFontDispVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFontDisp __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFontDisp __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFontDisp __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IFontDisp __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IFontDisp __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IFontDisp __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IFontDisp __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } IFontDispVtbl;

    interface IFontDisp
    {
        CONST_VTBL struct IFontDispVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFontDisp_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFontDisp_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFontDisp_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFontDisp_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFontDisp_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFontDisp_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFontDisp_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IFontDisp_INTERFACE_DEFINED__ */


#ifndef __IPictureDisp_INTERFACE_DEFINED__
#define __IPictureDisp_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IPictureDisp
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IPictureDisp __RPC_FAR *LPPICTUREDISP;


EXTERN_C const IID IID_IPictureDisp;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IPictureDisp : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IPictureDispVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPictureDisp __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPictureDisp __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPictureDisp __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPictureDisp __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPictureDisp __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPictureDisp __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPictureDisp __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } IPictureDispVtbl;

    interface IPictureDisp
    {
        CONST_VTBL struct IPictureDispVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPictureDisp_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPictureDisp_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPictureDisp_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPictureDisp_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPictureDisp_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPictureDisp_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPictureDisp_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPictureDisp_INTERFACE_DEFINED__ */


#ifndef __IAdviseSinkEx_INTERFACE_DEFINED__
#define __IAdviseSinkEx_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IAdviseSinkEx
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [uuid][unique][object] */ 


typedef IAdviseSinkEx __RPC_FAR *LPADVISESINKEX;


EXTERN_C const IID IID_IAdviseSinkEx;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IAdviseSinkEx : public IAdviseSink
    {
    public:
        virtual /* [local] */ void STDMETHODCALLTYPE OnViewStatusChange( 
            /* [in] */ DWORD dwViewStatus) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAdviseSinkExVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAdviseSinkEx __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAdviseSinkEx __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAdviseSinkEx __RPC_FAR * This);
        
        /* [local] */ void ( STDMETHODCALLTYPE __RPC_FAR *OnDataChange )( 
            IAdviseSinkEx __RPC_FAR * This,
            /* [unique][in] */ FORMATETC __RPC_FAR *pFormatetc,
            /* [unique][in] */ STGMEDIUM __RPC_FAR *pStgmed);
        
        /* [local] */ void ( STDMETHODCALLTYPE __RPC_FAR *OnViewChange )( 
            IAdviseSinkEx __RPC_FAR * This,
            /* [in] */ DWORD dwAspect,
            /* [in] */ LONG lindex);
        
        /* [local] */ void ( STDMETHODCALLTYPE __RPC_FAR *OnRename )( 
            IAdviseSinkEx __RPC_FAR * This,
            /* [in] */ IMoniker __RPC_FAR *pmk);
        
        /* [local] */ void ( STDMETHODCALLTYPE __RPC_FAR *OnSave )( 
            IAdviseSinkEx __RPC_FAR * This);
        
        /* [local] */ void ( STDMETHODCALLTYPE __RPC_FAR *OnClose )( 
            IAdviseSinkEx __RPC_FAR * This);
        
        /* [local] */ void ( STDMETHODCALLTYPE __RPC_FAR *OnViewStatusChange )( 
            IAdviseSinkEx __RPC_FAR * This,
            /* [in] */ DWORD dwViewStatus);
        
        END_INTERFACE
    } IAdviseSinkExVtbl;

    interface IAdviseSinkEx
    {
        CONST_VTBL struct IAdviseSinkExVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAdviseSinkEx_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAdviseSinkEx_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAdviseSinkEx_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAdviseSinkEx_OnDataChange(This,pFormatetc,pStgmed)	\
    (This)->lpVtbl -> OnDataChange(This,pFormatetc,pStgmed)

#define IAdviseSinkEx_OnViewChange(This,dwAspect,lindex)	\
    (This)->lpVtbl -> OnViewChange(This,dwAspect,lindex)

#define IAdviseSinkEx_OnRename(This,pmk)	\
    (This)->lpVtbl -> OnRename(This,pmk)

#define IAdviseSinkEx_OnSave(This)	\
    (This)->lpVtbl -> OnSave(This)

#define IAdviseSinkEx_OnClose(This)	\
    (This)->lpVtbl -> OnClose(This)


#define IAdviseSinkEx_OnViewStatusChange(This,dwViewStatus)	\
    (This)->lpVtbl -> OnViewStatusChange(This,dwViewStatus)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [async][call_as] */ void STDMETHODCALLTYPE IAdviseSinkEx_RemoteOnViewStatusChange_Proxy( 
    IAdviseSinkEx __RPC_FAR * This,
    /* [in] */ DWORD dwViewStatus);


void __RPC_STUB IAdviseSinkEx_RemoteOnViewStatusChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAdviseSinkEx_INTERFACE_DEFINED__ */


#ifndef __IOleInPlaceObjectWindowless_INTERFACE_DEFINED__
#define __IOleInPlaceObjectWindowless_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOleInPlaceObjectWindowless
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [uuid][unique][object][local] */ 


typedef IOleInPlaceObjectWindowless __RPC_FAR *LPOLEINPLACEOBJECTWINDOWLESS;


EXTERN_C const IID IID_IOleInPlaceObjectWindowless;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IOleInPlaceObjectWindowless : public IOleInPlaceObject
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnWindowMessage( 
            /* [in] */ UINT msg,
            /* [in] */ WPARAM wParam,
            /* [in] */ LPARAM lParam,
            /* [out] */ LRESULT __RPC_FAR *plResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDropTarget( 
            /* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOleInPlaceObjectWindowlessVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOleInPlaceObjectWindowless __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOleInPlaceObjectWindowless __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOleInPlaceObjectWindowless __RPC_FAR * This);
        
        /* [input_sync] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetWindow )( 
            IOleInPlaceObjectWindowless __RPC_FAR * This,
            /* [out] */ HWND __RPC_FAR *phwnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ContextSensitiveHelp )( 
            IOleInPlaceObjectWindowless __RPC_FAR * This,
            /* [in] */ BOOL fEnterMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InPlaceDeactivate )( 
            IOleInPlaceObjectWindowless __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UIDeactivate )( 
            IOleInPlaceObjectWindowless __RPC_FAR * This);
        
        /* [input_sync] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetObjectRects )( 
            IOleInPlaceObjectWindowless __RPC_FAR * This,
            /* [in] */ LPCRECT lprcPosRect,
            /* [in] */ LPCRECT lprcClipRect);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReactivateAndUndo )( 
            IOleInPlaceObjectWindowless __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnWindowMessage )( 
            IOleInPlaceObjectWindowless __RPC_FAR * This,
            /* [in] */ UINT msg,
            /* [in] */ WPARAM wParam,
            /* [in] */ LPARAM lParam,
            /* [out] */ LRESULT __RPC_FAR *plResult);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDropTarget )( 
            IOleInPlaceObjectWindowless __RPC_FAR * This,
            /* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget);
        
        END_INTERFACE
    } IOleInPlaceObjectWindowlessVtbl;

    interface IOleInPlaceObjectWindowless
    {
        CONST_VTBL struct IOleInPlaceObjectWindowlessVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOleInPlaceObjectWindowless_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOleInPlaceObjectWindowless_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOleInPlaceObjectWindowless_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOleInPlaceObjectWindowless_GetWindow(This,phwnd)	\
    (This)->lpVtbl -> GetWindow(This,phwnd)

#define IOleInPlaceObjectWindowless_ContextSensitiveHelp(This,fEnterMode)	\
    (This)->lpVtbl -> ContextSensitiveHelp(This,fEnterMode)


#define IOleInPlaceObjectWindowless_InPlaceDeactivate(This)	\
    (This)->lpVtbl -> InPlaceDeactivate(This)

#define IOleInPlaceObjectWindowless_UIDeactivate(This)	\
    (This)->lpVtbl -> UIDeactivate(This)

#define IOleInPlaceObjectWindowless_SetObjectRects(This,lprcPosRect,lprcClipRect)	\
    (This)->lpVtbl -> SetObjectRects(This,lprcPosRect,lprcClipRect)

#define IOleInPlaceObjectWindowless_ReactivateAndUndo(This)	\
    (This)->lpVtbl -> ReactivateAndUndo(This)


#define IOleInPlaceObjectWindowless_OnWindowMessage(This,msg,wParam,lParam,plResult)	\
    (This)->lpVtbl -> OnWindowMessage(This,msg,wParam,lParam,plResult)

#define IOleInPlaceObjectWindowless_GetDropTarget(This,ppDropTarget)	\
    (This)->lpVtbl -> GetDropTarget(This,ppDropTarget)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOleInPlaceObjectWindowless_OnWindowMessage_Proxy( 
    IOleInPlaceObjectWindowless __RPC_FAR * This,
    /* [in] */ UINT msg,
    /* [in] */ WPARAM wParam,
    /* [in] */ LPARAM lParam,
    /* [out] */ LRESULT __RPC_FAR *plResult);


void __RPC_STUB IOleInPlaceObjectWindowless_OnWindowMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleInPlaceObjectWindowless_GetDropTarget_Proxy( 
    IOleInPlaceObjectWindowless __RPC_FAR * This,
    /* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget);


void __RPC_STUB IOleInPlaceObjectWindowless_GetDropTarget_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOleInPlaceObjectWindowless_INTERFACE_DEFINED__ */


#ifndef __IOleInPlaceSiteEx_INTERFACE_DEFINED__
#define __IOleInPlaceSiteEx_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOleInPlaceSiteEx
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [uuid][unique][object] */ 


typedef IOleInPlaceSiteEx __RPC_FAR *LPOLEINPLACESITEEX;

typedef /* [v1_enum] */ 
enum tagACTIVATEFLAGS
    {	ACTIVATE_WINDOWLESS	= 1
    }	ACTIVATEFLAGS;


EXTERN_C const IID IID_IOleInPlaceSiteEx;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IOleInPlaceSiteEx : public IOleInPlaceSite
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnInPlaceActivateEx( 
            /* [out] */ BOOL __RPC_FAR *pfNoRedraw,
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnInPlaceDeactivateEx( 
            /* [in] */ BOOL fNoRedraw) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RequestUIActivate( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOleInPlaceSiteExVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOleInPlaceSiteEx __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOleInPlaceSiteEx __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOleInPlaceSiteEx __RPC_FAR * This);
        
        /* [input_sync] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetWindow )( 
            IOleInPlaceSiteEx __RPC_FAR * This,
            /* [out] */ HWND __RPC_FAR *phwnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ContextSensitiveHelp )( 
            IOleInPlaceSiteEx __RPC_FAR * This,
            /* [in] */ BOOL fEnterMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CanInPlaceActivate )( 
            IOleInPlaceSiteEx __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnInPlaceActivate )( 
            IOleInPlaceSiteEx __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUIActivate )( 
            IOleInPlaceSiteEx __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetWindowContext )( 
            IOleInPlaceSiteEx __RPC_FAR * This,
            /* [out] */ IOleInPlaceFrame __RPC_FAR *__RPC_FAR *ppFrame,
            /* [out] */ IOleInPlaceUIWindow __RPC_FAR *__RPC_FAR *ppDoc,
            /* [out] */ LPRECT lprcPosRect,
            /* [out] */ LPRECT lprcClipRect,
            /* [out][in] */ LPOLEINPLACEFRAMEINFO lpFrameInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Scroll )( 
            IOleInPlaceSiteEx __RPC_FAR * This,
            /* [in] */ SIZE scrollExtant);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUIDeactivate )( 
            IOleInPlaceSiteEx __RPC_FAR * This,
            /* [in] */ BOOL fUndoable);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnInPlaceDeactivate )( 
            IOleInPlaceSiteEx __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DiscardUndoState )( 
            IOleInPlaceSiteEx __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeactivateAndUndo )( 
            IOleInPlaceSiteEx __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnPosRectChange )( 
            IOleInPlaceSiteEx __RPC_FAR * This,
            /* [in] */ LPCRECT lprcPosRect);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnInPlaceActivateEx )( 
            IOleInPlaceSiteEx __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pfNoRedraw,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnInPlaceDeactivateEx )( 
            IOleInPlaceSiteEx __RPC_FAR * This,
            /* [in] */ BOOL fNoRedraw);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RequestUIActivate )( 
            IOleInPlaceSiteEx __RPC_FAR * This);
        
        END_INTERFACE
    } IOleInPlaceSiteExVtbl;

    interface IOleInPlaceSiteEx
    {
        CONST_VTBL struct IOleInPlaceSiteExVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOleInPlaceSiteEx_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOleInPlaceSiteEx_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOleInPlaceSiteEx_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOleInPlaceSiteEx_GetWindow(This,phwnd)	\
    (This)->lpVtbl -> GetWindow(This,phwnd)

#define IOleInPlaceSiteEx_ContextSensitiveHelp(This,fEnterMode)	\
    (This)->lpVtbl -> ContextSensitiveHelp(This,fEnterMode)


#define IOleInPlaceSiteEx_CanInPlaceActivate(This)	\
    (This)->lpVtbl -> CanInPlaceActivate(This)

#define IOleInPlaceSiteEx_OnInPlaceActivate(This)	\
    (This)->lpVtbl -> OnInPlaceActivate(This)

#define IOleInPlaceSiteEx_OnUIActivate(This)	\
    (This)->lpVtbl -> OnUIActivate(This)

#define IOleInPlaceSiteEx_GetWindowContext(This,ppFrame,ppDoc,lprcPosRect,lprcClipRect,lpFrameInfo)	\
    (This)->lpVtbl -> GetWindowContext(This,ppFrame,ppDoc,lprcPosRect,lprcClipRect,lpFrameInfo)

#define IOleInPlaceSiteEx_Scroll(This,scrollExtant)	\
    (This)->lpVtbl -> Scroll(This,scrollExtant)

#define IOleInPlaceSiteEx_OnUIDeactivate(This,fUndoable)	\
    (This)->lpVtbl -> OnUIDeactivate(This,fUndoable)

#define IOleInPlaceSiteEx_OnInPlaceDeactivate(This)	\
    (This)->lpVtbl -> OnInPlaceDeactivate(This)

#define IOleInPlaceSiteEx_DiscardUndoState(This)	\
    (This)->lpVtbl -> DiscardUndoState(This)

#define IOleInPlaceSiteEx_DeactivateAndUndo(This)	\
    (This)->lpVtbl -> DeactivateAndUndo(This)

#define IOleInPlaceSiteEx_OnPosRectChange(This,lprcPosRect)	\
    (This)->lpVtbl -> OnPosRectChange(This,lprcPosRect)


#define IOleInPlaceSiteEx_OnInPlaceActivateEx(This,pfNoRedraw,dwFlags)	\
    (This)->lpVtbl -> OnInPlaceActivateEx(This,pfNoRedraw,dwFlags)

#define IOleInPlaceSiteEx_OnInPlaceDeactivateEx(This,fNoRedraw)	\
    (This)->lpVtbl -> OnInPlaceDeactivateEx(This,fNoRedraw)

#define IOleInPlaceSiteEx_RequestUIActivate(This)	\
    (This)->lpVtbl -> RequestUIActivate(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOleInPlaceSiteEx_OnInPlaceActivateEx_Proxy( 
    IOleInPlaceSiteEx __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pfNoRedraw,
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IOleInPlaceSiteEx_OnInPlaceActivateEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleInPlaceSiteEx_OnInPlaceDeactivateEx_Proxy( 
    IOleInPlaceSiteEx __RPC_FAR * This,
    /* [in] */ BOOL fNoRedraw);


void __RPC_STUB IOleInPlaceSiteEx_OnInPlaceDeactivateEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleInPlaceSiteEx_RequestUIActivate_Proxy( 
    IOleInPlaceSiteEx __RPC_FAR * This);


void __RPC_STUB IOleInPlaceSiteEx_RequestUIActivate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOleInPlaceSiteEx_INTERFACE_DEFINED__ */


#ifndef __IOleInPlaceSiteWindowless_INTERFACE_DEFINED__
#define __IOleInPlaceSiteWindowless_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOleInPlaceSiteWindowless
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [uuid][unique][object][local] */ 


typedef IOleInPlaceSiteWindowless __RPC_FAR *LPOLEINPLACESITEWINDOWLESS;

typedef /* [v1_enum] */ 
enum tagOLEDCFLAGS
    {	OLEDC_NODRAW	= 0x1,
	OLEDC_PAINTBKGND	= 0x2,
	OLEDC_OFFSCREEN	= 0x4
    }	OLEDCFLAGS;


EXTERN_C const IID IID_IOleInPlaceSiteWindowless;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IOleInPlaceSiteWindowless : public IOleInPlaceSiteEx
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CanWindowlessActivate( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCapture( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCapture( 
            /* [in] */ BOOL fCapture) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFocus( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetFocus( 
            /* [in] */ BOOL fFocus) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDC( 
            /* [in] */ LPCRECT pRect,
            /* [in] */ DWORD grfFlags,
            /* [out] */ HDC __RPC_FAR *phDC) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReleaseDC( 
            /* [in] */ HDC hDC) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InvalidateRect( 
            /* [in] */ LPCRECT pRect,
            /* [in] */ BOOL fErase) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InvalidateRgn( 
            /* [in] */ HRGN hRGN,
            /* [in] */ BOOL fErase) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ScrollRect( 
            /* [in] */ INT dx,
            /* [in] */ INT dy,
            /* [in] */ LPCRECT pRectScroll,
            /* [in] */ LPCRECT pRectClip) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AdjustRect( 
            /* [out][in] */ LPRECT prc) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnDefWindowMessage( 
            /* [in] */ UINT msg,
            /* [in] */ WPARAM wParam,
            /* [in] */ LPARAM lParam,
            /* [out] */ LRESULT __RPC_FAR *plResult) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOleInPlaceSiteWindowlessVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This);
        
        /* [input_sync] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetWindow )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This,
            /* [out] */ HWND __RPC_FAR *phwnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ContextSensitiveHelp )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This,
            /* [in] */ BOOL fEnterMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CanInPlaceActivate )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnInPlaceActivate )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUIActivate )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetWindowContext )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This,
            /* [out] */ IOleInPlaceFrame __RPC_FAR *__RPC_FAR *ppFrame,
            /* [out] */ IOleInPlaceUIWindow __RPC_FAR *__RPC_FAR *ppDoc,
            /* [out] */ LPRECT lprcPosRect,
            /* [out] */ LPRECT lprcClipRect,
            /* [out][in] */ LPOLEINPLACEFRAMEINFO lpFrameInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Scroll )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This,
            /* [in] */ SIZE scrollExtant);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUIDeactivate )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This,
            /* [in] */ BOOL fUndoable);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnInPlaceDeactivate )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DiscardUndoState )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeactivateAndUndo )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnPosRectChange )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This,
            /* [in] */ LPCRECT lprcPosRect);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnInPlaceActivateEx )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pfNoRedraw,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnInPlaceDeactivateEx )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This,
            /* [in] */ BOOL fNoRedraw);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RequestUIActivate )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CanWindowlessActivate )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCapture )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCapture )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This,
            /* [in] */ BOOL fCapture);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFocus )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFocus )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This,
            /* [in] */ BOOL fFocus);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDC )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This,
            /* [in] */ LPCRECT pRect,
            /* [in] */ DWORD grfFlags,
            /* [out] */ HDC __RPC_FAR *phDC);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReleaseDC )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This,
            /* [in] */ HDC hDC);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvalidateRect )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This,
            /* [in] */ LPCRECT pRect,
            /* [in] */ BOOL fErase);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvalidateRgn )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This,
            /* [in] */ HRGN hRGN,
            /* [in] */ BOOL fErase);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ScrollRect )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This,
            /* [in] */ INT dx,
            /* [in] */ INT dy,
            /* [in] */ LPCRECT pRectScroll,
            /* [in] */ LPCRECT pRectClip);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AdjustRect )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This,
            /* [out][in] */ LPRECT prc);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnDefWindowMessage )( 
            IOleInPlaceSiteWindowless __RPC_FAR * This,
            /* [in] */ UINT msg,
            /* [in] */ WPARAM wParam,
            /* [in] */ LPARAM lParam,
            /* [out] */ LRESULT __RPC_FAR *plResult);
        
        END_INTERFACE
    } IOleInPlaceSiteWindowlessVtbl;

    interface IOleInPlaceSiteWindowless
    {
        CONST_VTBL struct IOleInPlaceSiteWindowlessVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOleInPlaceSiteWindowless_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOleInPlaceSiteWindowless_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOleInPlaceSiteWindowless_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOleInPlaceSiteWindowless_GetWindow(This,phwnd)	\
    (This)->lpVtbl -> GetWindow(This,phwnd)

#define IOleInPlaceSiteWindowless_ContextSensitiveHelp(This,fEnterMode)	\
    (This)->lpVtbl -> ContextSensitiveHelp(This,fEnterMode)


#define IOleInPlaceSiteWindowless_CanInPlaceActivate(This)	\
    (This)->lpVtbl -> CanInPlaceActivate(This)

#define IOleInPlaceSiteWindowless_OnInPlaceActivate(This)	\
    (This)->lpVtbl -> OnInPlaceActivate(This)

#define IOleInPlaceSiteWindowless_OnUIActivate(This)	\
    (This)->lpVtbl -> OnUIActivate(This)

#define IOleInPlaceSiteWindowless_GetWindowContext(This,ppFrame,ppDoc,lprcPosRect,lprcClipRect,lpFrameInfo)	\
    (This)->lpVtbl -> GetWindowContext(This,ppFrame,ppDoc,lprcPosRect,lprcClipRect,lpFrameInfo)

#define IOleInPlaceSiteWindowless_Scroll(This,scrollExtant)	\
    (This)->lpVtbl -> Scroll(This,scrollExtant)

#define IOleInPlaceSiteWindowless_OnUIDeactivate(This,fUndoable)	\
    (This)->lpVtbl -> OnUIDeactivate(This,fUndoable)

#define IOleInPlaceSiteWindowless_OnInPlaceDeactivate(This)	\
    (This)->lpVtbl -> OnInPlaceDeactivate(This)

#define IOleInPlaceSiteWindowless_DiscardUndoState(This)	\
    (This)->lpVtbl -> DiscardUndoState(This)

#define IOleInPlaceSiteWindowless_DeactivateAndUndo(This)	\
    (This)->lpVtbl -> DeactivateAndUndo(This)

#define IOleInPlaceSiteWindowless_OnPosRectChange(This,lprcPosRect)	\
    (This)->lpVtbl -> OnPosRectChange(This,lprcPosRect)


#define IOleInPlaceSiteWindowless_OnInPlaceActivateEx(This,pfNoRedraw,dwFlags)	\
    (This)->lpVtbl -> OnInPlaceActivateEx(This,pfNoRedraw,dwFlags)

#define IOleInPlaceSiteWindowless_OnInPlaceDeactivateEx(This,fNoRedraw)	\
    (This)->lpVtbl -> OnInPlaceDeactivateEx(This,fNoRedraw)

#define IOleInPlaceSiteWindowless_RequestUIActivate(This)	\
    (This)->lpVtbl -> RequestUIActivate(This)


#define IOleInPlaceSiteWindowless_CanWindowlessActivate(This)	\
    (This)->lpVtbl -> CanWindowlessActivate(This)

#define IOleInPlaceSiteWindowless_GetCapture(This)	\
    (This)->lpVtbl -> GetCapture(This)

#define IOleInPlaceSiteWindowless_SetCapture(This,fCapture)	\
    (This)->lpVtbl -> SetCapture(This,fCapture)

#define IOleInPlaceSiteWindowless_GetFocus(This)	\
    (This)->lpVtbl -> GetFocus(This)

#define IOleInPlaceSiteWindowless_SetFocus(This,fFocus)	\
    (This)->lpVtbl -> SetFocus(This,fFocus)

#define IOleInPlaceSiteWindowless_GetDC(This,pRect,grfFlags,phDC)	\
    (This)->lpVtbl -> GetDC(This,pRect,grfFlags,phDC)

#define IOleInPlaceSiteWindowless_ReleaseDC(This,hDC)	\
    (This)->lpVtbl -> ReleaseDC(This,hDC)

#define IOleInPlaceSiteWindowless_InvalidateRect(This,pRect,fErase)	\
    (This)->lpVtbl -> InvalidateRect(This,pRect,fErase)

#define IOleInPlaceSiteWindowless_InvalidateRgn(This,hRGN,fErase)	\
    (This)->lpVtbl -> InvalidateRgn(This,hRGN,fErase)

#define IOleInPlaceSiteWindowless_ScrollRect(This,dx,dy,pRectScroll,pRectClip)	\
    (This)->lpVtbl -> ScrollRect(This,dx,dy,pRectScroll,pRectClip)

#define IOleInPlaceSiteWindowless_AdjustRect(This,prc)	\
    (This)->lpVtbl -> AdjustRect(This,prc)

#define IOleInPlaceSiteWindowless_OnDefWindowMessage(This,msg,wParam,lParam,plResult)	\
    (This)->lpVtbl -> OnDefWindowMessage(This,msg,wParam,lParam,plResult)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOleInPlaceSiteWindowless_CanWindowlessActivate_Proxy( 
    IOleInPlaceSiteWindowless __RPC_FAR * This);


void __RPC_STUB IOleInPlaceSiteWindowless_CanWindowlessActivate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleInPlaceSiteWindowless_GetCapture_Proxy( 
    IOleInPlaceSiteWindowless __RPC_FAR * This);


void __RPC_STUB IOleInPlaceSiteWindowless_GetCapture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleInPlaceSiteWindowless_SetCapture_Proxy( 
    IOleInPlaceSiteWindowless __RPC_FAR * This,
    /* [in] */ BOOL fCapture);


void __RPC_STUB IOleInPlaceSiteWindowless_SetCapture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleInPlaceSiteWindowless_GetFocus_Proxy( 
    IOleInPlaceSiteWindowless __RPC_FAR * This);


void __RPC_STUB IOleInPlaceSiteWindowless_GetFocus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleInPlaceSiteWindowless_SetFocus_Proxy( 
    IOleInPlaceSiteWindowless __RPC_FAR * This,
    /* [in] */ BOOL fFocus);


void __RPC_STUB IOleInPlaceSiteWindowless_SetFocus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleInPlaceSiteWindowless_GetDC_Proxy( 
    IOleInPlaceSiteWindowless __RPC_FAR * This,
    /* [in] */ LPCRECT pRect,
    /* [in] */ DWORD grfFlags,
    /* [out] */ HDC __RPC_FAR *phDC);


void __RPC_STUB IOleInPlaceSiteWindowless_GetDC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleInPlaceSiteWindowless_ReleaseDC_Proxy( 
    IOleInPlaceSiteWindowless __RPC_FAR * This,
    /* [in] */ HDC hDC);


void __RPC_STUB IOleInPlaceSiteWindowless_ReleaseDC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleInPlaceSiteWindowless_InvalidateRect_Proxy( 
    IOleInPlaceSiteWindowless __RPC_FAR * This,
    /* [in] */ LPCRECT pRect,
    /* [in] */ BOOL fErase);


void __RPC_STUB IOleInPlaceSiteWindowless_InvalidateRect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleInPlaceSiteWindowless_InvalidateRgn_Proxy( 
    IOleInPlaceSiteWindowless __RPC_FAR * This,
    /* [in] */ HRGN hRGN,
    /* [in] */ BOOL fErase);


void __RPC_STUB IOleInPlaceSiteWindowless_InvalidateRgn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleInPlaceSiteWindowless_ScrollRect_Proxy( 
    IOleInPlaceSiteWindowless __RPC_FAR * This,
    /* [in] */ INT dx,
    /* [in] */ INT dy,
    /* [in] */ LPCRECT pRectScroll,
    /* [in] */ LPCRECT pRectClip);


void __RPC_STUB IOleInPlaceSiteWindowless_ScrollRect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleInPlaceSiteWindowless_AdjustRect_Proxy( 
    IOleInPlaceSiteWindowless __RPC_FAR * This,
    /* [out][in] */ LPRECT prc);


void __RPC_STUB IOleInPlaceSiteWindowless_AdjustRect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleInPlaceSiteWindowless_OnDefWindowMessage_Proxy( 
    IOleInPlaceSiteWindowless __RPC_FAR * This,
    /* [in] */ UINT msg,
    /* [in] */ WPARAM wParam,
    /* [in] */ LPARAM lParam,
    /* [out] */ LRESULT __RPC_FAR *plResult);


void __RPC_STUB IOleInPlaceSiteWindowless_OnDefWindowMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOleInPlaceSiteWindowless_INTERFACE_DEFINED__ */


#ifndef __IViewObjectEx_INTERFACE_DEFINED__
#define __IViewObjectEx_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IViewObjectEx
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [uuid][unique][object][local] */ 


typedef IViewObjectEx __RPC_FAR *LPVIEWOBJECTEX;

typedef /* [v1_enum] */ 
enum tagVIEWSTATUS
    {	VIEWSTATUS_OPAQUE	= 1,
	VIEWSTATUS_SOLIDBKGND	= 2,
	VIEWSTATUS_DVASPECTOPAQUE	= 4,
	VIEWSTATUS_DVASPECTTRANSPARENT	= 8
    }	VIEWSTATUS;

typedef /* [v1_enum] */ 
enum tagHITRESULT
    {	HITRESULT_OUTSIDE	= 0,
	HITRESULT_TRANSPARENT	= 1,
	HITRESULT_CLOSE	= 2,
	HITRESULT_HIT	= 3
    }	HITRESULT;

typedef /* [v1_enum] */ 
enum tagDVASPECT2
    {	DVASPECT_OPAQUE	= 16,
	DVASPECT_TRANSPARENT	= 32
    }	DVASPECT2;

typedef struct  tagExtentInfo
    {
    ULONG cb;
    DWORD dwExtentMode;
    SIZEL sizelProposed;
    }	DVEXTENTINFO;

typedef /* [v1_enum] */ 
enum tagExtentMode
    {	DVEXTENT_CONTENT	= 0,
	DVEXTENT_INTEGRAL	= DVEXTENT_CONTENT + 1
    }	DVEXTENTMODE;

typedef /* [v1_enum] */ 
enum tagAspectInfoFlag
    {	DVASPECTINFOFLAG_CANOPTIMIZE	= 1
    }	DVASPECTINFOFLAG;

typedef struct  tagAspectInfo
    {
    ULONG cb;
    DWORD dwFlags;
    }	DVASPECTINFO;


EXTERN_C const IID IID_IViewObjectEx;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IViewObjectEx : public IViewObject2
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetRect( 
            /* [in] */ DWORD dwAspect,
            /* [out] */ LPRECTL pRect) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetViewStatus( 
            /* [out] */ DWORD __RPC_FAR *pdwStatus) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryHitPoint( 
            /* [in] */ DWORD dwAspect,
            /* [in] */ LPCRECT pRectBounds,
            /* [in] */ POINT ptlLoc,
            /* [in] */ LONG lCloseHint,
            /* [out] */ DWORD __RPC_FAR *pHitResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryHitRect( 
            /* [in] */ DWORD dwAspect,
            /* [in] */ LPCRECT pRectBounds,
            /* [in] */ LPCRECT pRectLoc,
            /* [in] */ LONG lCloseHint,
            /* [out] */ DWORD __RPC_FAR *pHitResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNaturalExtent( 
            /* [in] */ DWORD dwAspect,
            /* [in] */ LONG lindex,
            /* [in] */ DVTARGETDEVICE __RPC_FAR *ptd,
            /* [in] */ HDC hicTargetDev,
            /* [in] */ DVEXTENTINFO __RPC_FAR *pExtentInfo,
            /* [out] */ LPSIZEL pSizel) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IViewObjectExVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IViewObjectEx __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IViewObjectEx __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IViewObjectEx __RPC_FAR * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Draw )( 
            IViewObjectEx __RPC_FAR * This,
            /* [in] */ DWORD dwDrawAspect,
            /* [in] */ LONG lindex,
            /* [unique][in] */ void __RPC_FAR *pvAspect,
            /* [unique][in] */ DVTARGETDEVICE __RPC_FAR *ptd,
            /* [in] */ HDC hdcTargetDev,
            /* [in] */ HDC hdcDraw,
            /* [in] */ LPCRECTL lprcBounds,
            /* [unique][in] */ LPCRECTL lprcWBounds,
            /* [in] */ BOOL ( STDMETHODCALLTYPE __RPC_FAR *pfnContinue )( 
                DWORD dwContinue),
            /* [in] */ DWORD dwContinue);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetColorSet )( 
            IViewObjectEx __RPC_FAR * This,
            /* [in] */ DWORD dwDrawAspect,
            /* [in] */ LONG lindex,
            /* [unique][in] */ void __RPC_FAR *pvAspect,
            /* [unique][in] */ DVTARGETDEVICE __RPC_FAR *ptd,
            /* [in] */ HDC hicTargetDev,
            /* [out] */ LOGPALETTE __RPC_FAR *__RPC_FAR *ppColorSet);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Freeze )( 
            IViewObjectEx __RPC_FAR * This,
            /* [in] */ DWORD dwDrawAspect,
            /* [in] */ LONG lindex,
            /* [unique][in] */ void __RPC_FAR *pvAspect,
            /* [out] */ DWORD __RPC_FAR *pdwFreeze);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unfreeze )( 
            IViewObjectEx __RPC_FAR * This,
            /* [in] */ DWORD dwFreeze);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetAdvise )( 
            IViewObjectEx __RPC_FAR * This,
            /* [in] */ DWORD aspects,
            /* [in] */ DWORD advf,
            /* [unique][in] */ IAdviseSink __RPC_FAR *pAdvSink);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAdvise )( 
            IViewObjectEx __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pAspects,
            /* [out] */ DWORD __RPC_FAR *pAdvf,
            /* [out] */ IAdviseSink __RPC_FAR *__RPC_FAR *ppAdvSink);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetExtent )( 
            IViewObjectEx __RPC_FAR * This,
            /* [in] */ DWORD dwDrawAspect,
            /* [in] */ LONG lindex,
            /* [unique][in] */ DVTARGETDEVICE __RPC_FAR *ptd,
            /* [out] */ LPSIZEL lpsizel);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRect )( 
            IViewObjectEx __RPC_FAR * This,
            /* [in] */ DWORD dwAspect,
            /* [out] */ LPRECTL pRect);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetViewStatus )( 
            IViewObjectEx __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwStatus);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryHitPoint )( 
            IViewObjectEx __RPC_FAR * This,
            /* [in] */ DWORD dwAspect,
            /* [in] */ LPCRECT pRectBounds,
            /* [in] */ POINT ptlLoc,
            /* [in] */ LONG lCloseHint,
            /* [out] */ DWORD __RPC_FAR *pHitResult);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryHitRect )( 
            IViewObjectEx __RPC_FAR * This,
            /* [in] */ DWORD dwAspect,
            /* [in] */ LPCRECT pRectBounds,
            /* [in] */ LPCRECT pRectLoc,
            /* [in] */ LONG lCloseHint,
            /* [out] */ DWORD __RPC_FAR *pHitResult);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNaturalExtent )( 
            IViewObjectEx __RPC_FAR * This,
            /* [in] */ DWORD dwAspect,
            /* [in] */ LONG lindex,
            /* [in] */ DVTARGETDEVICE __RPC_FAR *ptd,
            /* [in] */ HDC hicTargetDev,
            /* [in] */ DVEXTENTINFO __RPC_FAR *pExtentInfo,
            /* [out] */ LPSIZEL pSizel);
        
        END_INTERFACE
    } IViewObjectExVtbl;

    interface IViewObjectEx
    {
        CONST_VTBL struct IViewObjectExVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IViewObjectEx_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IViewObjectEx_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IViewObjectEx_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IViewObjectEx_Draw(This,dwDrawAspect,lindex,pvAspect,ptd,hdcTargetDev,hdcDraw,lprcBounds,lprcWBounds,pfnContinue,dwContinue)	\
    (This)->lpVtbl -> Draw(This,dwDrawAspect,lindex,pvAspect,ptd,hdcTargetDev,hdcDraw,lprcBounds,lprcWBounds,pfnContinue,dwContinue)

#define IViewObjectEx_GetColorSet(This,dwDrawAspect,lindex,pvAspect,ptd,hicTargetDev,ppColorSet)	\
    (This)->lpVtbl -> GetColorSet(This,dwDrawAspect,lindex,pvAspect,ptd,hicTargetDev,ppColorSet)

#define IViewObjectEx_Freeze(This,dwDrawAspect,lindex,pvAspect,pdwFreeze)	\
    (This)->lpVtbl -> Freeze(This,dwDrawAspect,lindex,pvAspect,pdwFreeze)

#define IViewObjectEx_Unfreeze(This,dwFreeze)	\
    (This)->lpVtbl -> Unfreeze(This,dwFreeze)

#define IViewObjectEx_SetAdvise(This,aspects,advf,pAdvSink)	\
    (This)->lpVtbl -> SetAdvise(This,aspects,advf,pAdvSink)

#define IViewObjectEx_GetAdvise(This,pAspects,pAdvf,ppAdvSink)	\
    (This)->lpVtbl -> GetAdvise(This,pAspects,pAdvf,ppAdvSink)


#define IViewObjectEx_GetExtent(This,dwDrawAspect,lindex,ptd,lpsizel)	\
    (This)->lpVtbl -> GetExtent(This,dwDrawAspect,lindex,ptd,lpsizel)


#define IViewObjectEx_GetRect(This,dwAspect,pRect)	\
    (This)->lpVtbl -> GetRect(This,dwAspect,pRect)

#define IViewObjectEx_GetViewStatus(This,pdwStatus)	\
    (This)->lpVtbl -> GetViewStatus(This,pdwStatus)

#define IViewObjectEx_QueryHitPoint(This,dwAspect,pRectBounds,ptlLoc,lCloseHint,pHitResult)	\
    (This)->lpVtbl -> QueryHitPoint(This,dwAspect,pRectBounds,ptlLoc,lCloseHint,pHitResult)

#define IViewObjectEx_QueryHitRect(This,dwAspect,pRectBounds,pRectLoc,lCloseHint,pHitResult)	\
    (This)->lpVtbl -> QueryHitRect(This,dwAspect,pRectBounds,pRectLoc,lCloseHint,pHitResult)

#define IViewObjectEx_GetNaturalExtent(This,dwAspect,lindex,ptd,hicTargetDev,pExtentInfo,pSizel)	\
    (This)->lpVtbl -> GetNaturalExtent(This,dwAspect,lindex,ptd,hicTargetDev,pExtentInfo,pSizel)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IViewObjectEx_GetRect_Proxy( 
    IViewObjectEx __RPC_FAR * This,
    /* [in] */ DWORD dwAspect,
    /* [out] */ LPRECTL pRect);


void __RPC_STUB IViewObjectEx_GetRect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IViewObjectEx_GetViewStatus_Proxy( 
    IViewObjectEx __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwStatus);


void __RPC_STUB IViewObjectEx_GetViewStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IViewObjectEx_QueryHitPoint_Proxy( 
    IViewObjectEx __RPC_FAR * This,
    /* [in] */ DWORD dwAspect,
    /* [in] */ LPCRECT pRectBounds,
    /* [in] */ POINT ptlLoc,
    /* [in] */ LONG lCloseHint,
    /* [out] */ DWORD __RPC_FAR *pHitResult);


void __RPC_STUB IViewObjectEx_QueryHitPoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IViewObjectEx_QueryHitRect_Proxy( 
    IViewObjectEx __RPC_FAR * This,
    /* [in] */ DWORD dwAspect,
    /* [in] */ LPCRECT pRectBounds,
    /* [in] */ LPCRECT pRectLoc,
    /* [in] */ LONG lCloseHint,
    /* [out] */ DWORD __RPC_FAR *pHitResult);


void __RPC_STUB IViewObjectEx_QueryHitRect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IViewObjectEx_GetNaturalExtent_Proxy( 
    IViewObjectEx __RPC_FAR * This,
    /* [in] */ DWORD dwAspect,
    /* [in] */ LONG lindex,
    /* [in] */ DVTARGETDEVICE __RPC_FAR *ptd,
    /* [in] */ HDC hicTargetDev,
    /* [in] */ DVEXTENTINFO __RPC_FAR *pExtentInfo,
    /* [out] */ LPSIZEL pSizel);


void __RPC_STUB IViewObjectEx_GetNaturalExtent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IViewObjectEx_INTERFACE_DEFINED__ */


#ifndef __IOleUndoUnit_INTERFACE_DEFINED__
#define __IOleUndoUnit_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOleUndoUnit
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [uuid][unique][object] */ 


typedef IOleUndoUnit __RPC_FAR *LPOLEUNDOUNIT;


EXTERN_C const IID IID_IOleUndoUnit;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IOleUndoUnit : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Do( 
            /* [in] */ IOleUndoManager __RPC_FAR *pUndoManager) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDescription( 
            /* [out] */ BSTR __RPC_FAR *pBstr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUnitType( 
            /* [out] */ CLSID __RPC_FAR *pClsid,
            /* [out] */ LONG __RPC_FAR *plID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnNextAdd( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOleUndoUnitVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOleUndoUnit __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOleUndoUnit __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOleUndoUnit __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Do )( 
            IOleUndoUnit __RPC_FAR * This,
            /* [in] */ IOleUndoManager __RPC_FAR *pUndoManager);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDescription )( 
            IOleUndoUnit __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *pBstr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetUnitType )( 
            IOleUndoUnit __RPC_FAR * This,
            /* [out] */ CLSID __RPC_FAR *pClsid,
            /* [out] */ LONG __RPC_FAR *plID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnNextAdd )( 
            IOleUndoUnit __RPC_FAR * This);
        
        END_INTERFACE
    } IOleUndoUnitVtbl;

    interface IOleUndoUnit
    {
        CONST_VTBL struct IOleUndoUnitVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOleUndoUnit_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOleUndoUnit_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOleUndoUnit_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOleUndoUnit_Do(This,pUndoManager)	\
    (This)->lpVtbl -> Do(This,pUndoManager)

#define IOleUndoUnit_GetDescription(This,pBstr)	\
    (This)->lpVtbl -> GetDescription(This,pBstr)

#define IOleUndoUnit_GetUnitType(This,pClsid,plID)	\
    (This)->lpVtbl -> GetUnitType(This,pClsid,plID)

#define IOleUndoUnit_OnNextAdd(This)	\
    (This)->lpVtbl -> OnNextAdd(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOleUndoUnit_Do_Proxy( 
    IOleUndoUnit __RPC_FAR * This,
    /* [in] */ IOleUndoManager __RPC_FAR *pUndoManager);


void __RPC_STUB IOleUndoUnit_Do_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleUndoUnit_GetDescription_Proxy( 
    IOleUndoUnit __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *pBstr);


void __RPC_STUB IOleUndoUnit_GetDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleUndoUnit_GetUnitType_Proxy( 
    IOleUndoUnit __RPC_FAR * This,
    /* [out] */ CLSID __RPC_FAR *pClsid,
    /* [out] */ LONG __RPC_FAR *plID);


void __RPC_STUB IOleUndoUnit_GetUnitType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleUndoUnit_OnNextAdd_Proxy( 
    IOleUndoUnit __RPC_FAR * This);


void __RPC_STUB IOleUndoUnit_OnNextAdd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOleUndoUnit_INTERFACE_DEFINED__ */


#ifndef __IOleParentUndoUnit_INTERFACE_DEFINED__
#define __IOleParentUndoUnit_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOleParentUndoUnit
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [uuid][unique][object] */ 


typedef IOleParentUndoUnit __RPC_FAR *LPOLEPARENTUNDOUNIT;


EXTERN_C const IID IID_IOleParentUndoUnit;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IOleParentUndoUnit : public IOleUndoUnit
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ IOleParentUndoUnit __RPC_FAR *pPUU) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Close( 
            /* [in] */ IOleParentUndoUnit __RPC_FAR *pPUU,
            /* [in] */ BOOL fCommit) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ IOleUndoUnit __RPC_FAR *pUU) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindUnit( 
            /* [in] */ IOleUndoUnit __RPC_FAR *pUU) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetParentState( 
            /* [out] */ DWORD __RPC_FAR *pdwState) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOleParentUndoUnitVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOleParentUndoUnit __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOleParentUndoUnit __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOleParentUndoUnit __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Do )( 
            IOleParentUndoUnit __RPC_FAR * This,
            /* [in] */ IOleUndoManager __RPC_FAR *pUndoManager);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDescription )( 
            IOleParentUndoUnit __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *pBstr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetUnitType )( 
            IOleParentUndoUnit __RPC_FAR * This,
            /* [out] */ CLSID __RPC_FAR *pClsid,
            /* [out] */ LONG __RPC_FAR *plID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnNextAdd )( 
            IOleParentUndoUnit __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open )( 
            IOleParentUndoUnit __RPC_FAR * This,
            /* [in] */ IOleParentUndoUnit __RPC_FAR *pPUU);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Close )( 
            IOleParentUndoUnit __RPC_FAR * This,
            /* [in] */ IOleParentUndoUnit __RPC_FAR *pPUU,
            /* [in] */ BOOL fCommit);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add )( 
            IOleParentUndoUnit __RPC_FAR * This,
            /* [in] */ IOleUndoUnit __RPC_FAR *pUU);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindUnit )( 
            IOleParentUndoUnit __RPC_FAR * This,
            /* [in] */ IOleUndoUnit __RPC_FAR *pUU);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetParentState )( 
            IOleParentUndoUnit __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwState);
        
        END_INTERFACE
    } IOleParentUndoUnitVtbl;

    interface IOleParentUndoUnit
    {
        CONST_VTBL struct IOleParentUndoUnitVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOleParentUndoUnit_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOleParentUndoUnit_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOleParentUndoUnit_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOleParentUndoUnit_Do(This,pUndoManager)	\
    (This)->lpVtbl -> Do(This,pUndoManager)

#define IOleParentUndoUnit_GetDescription(This,pBstr)	\
    (This)->lpVtbl -> GetDescription(This,pBstr)

#define IOleParentUndoUnit_GetUnitType(This,pClsid,plID)	\
    (This)->lpVtbl -> GetUnitType(This,pClsid,plID)

#define IOleParentUndoUnit_OnNextAdd(This)	\
    (This)->lpVtbl -> OnNextAdd(This)


#define IOleParentUndoUnit_Open(This,pPUU)	\
    (This)->lpVtbl -> Open(This,pPUU)

#define IOleParentUndoUnit_Close(This,pPUU,fCommit)	\
    (This)->lpVtbl -> Close(This,pPUU,fCommit)

#define IOleParentUndoUnit_Add(This,pUU)	\
    (This)->lpVtbl -> Add(This,pUU)

#define IOleParentUndoUnit_FindUnit(This,pUU)	\
    (This)->lpVtbl -> FindUnit(This,pUU)

#define IOleParentUndoUnit_GetParentState(This,pdwState)	\
    (This)->lpVtbl -> GetParentState(This,pdwState)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOleParentUndoUnit_Open_Proxy( 
    IOleParentUndoUnit __RPC_FAR * This,
    /* [in] */ IOleParentUndoUnit __RPC_FAR *pPUU);


void __RPC_STUB IOleParentUndoUnit_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleParentUndoUnit_Close_Proxy( 
    IOleParentUndoUnit __RPC_FAR * This,
    /* [in] */ IOleParentUndoUnit __RPC_FAR *pPUU,
    /* [in] */ BOOL fCommit);


void __RPC_STUB IOleParentUndoUnit_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleParentUndoUnit_Add_Proxy( 
    IOleParentUndoUnit __RPC_FAR * This,
    /* [in] */ IOleUndoUnit __RPC_FAR *pUU);


void __RPC_STUB IOleParentUndoUnit_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleParentUndoUnit_FindUnit_Proxy( 
    IOleParentUndoUnit __RPC_FAR * This,
    /* [in] */ IOleUndoUnit __RPC_FAR *pUU);


void __RPC_STUB IOleParentUndoUnit_FindUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleParentUndoUnit_GetParentState_Proxy( 
    IOleParentUndoUnit __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwState);


void __RPC_STUB IOleParentUndoUnit_GetParentState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOleParentUndoUnit_INTERFACE_DEFINED__ */


#ifndef __IEnumOleUndoUnits_INTERFACE_DEFINED__
#define __IEnumOleUndoUnits_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IEnumOleUndoUnits
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [uuid][unique][object] */ 


typedef IEnumOleUndoUnits __RPC_FAR *LPENUMOLEUNDOUNITS;


EXTERN_C const IID IID_IEnumOleUndoUnits;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IEnumOleUndoUnits : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Next( 
            /* [in] */ ULONG cElt,
            /* [length_is][size_is][out] */ IOleUndoUnit __RPC_FAR *__RPC_FAR *rgElt,
            /* [out] */ ULONG __RPC_FAR *pcEltFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG cElt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumOleUndoUnits __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumOleUndoUnitsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEnumOleUndoUnits __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEnumOleUndoUnits __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEnumOleUndoUnits __RPC_FAR * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Next )( 
            IEnumOleUndoUnits __RPC_FAR * This,
            /* [in] */ ULONG cElt,
            /* [length_is][size_is][out] */ IOleUndoUnit __RPC_FAR *__RPC_FAR *rgElt,
            /* [out] */ ULONG __RPC_FAR *pcEltFetched);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Skip )( 
            IEnumOleUndoUnits __RPC_FAR * This,
            /* [in] */ ULONG cElt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reset )( 
            IEnumOleUndoUnits __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IEnumOleUndoUnits __RPC_FAR * This,
            /* [out] */ IEnumOleUndoUnits __RPC_FAR *__RPC_FAR *ppEnum);
        
        END_INTERFACE
    } IEnumOleUndoUnitsVtbl;

    interface IEnumOleUndoUnits
    {
        CONST_VTBL struct IEnumOleUndoUnitsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumOleUndoUnits_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumOleUndoUnits_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumOleUndoUnits_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumOleUndoUnits_Next(This,cElt,rgElt,pcEltFetched)	\
    (This)->lpVtbl -> Next(This,cElt,rgElt,pcEltFetched)

#define IEnumOleUndoUnits_Skip(This,cElt)	\
    (This)->lpVtbl -> Skip(This,cElt)

#define IEnumOleUndoUnits_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumOleUndoUnits_Clone(This,ppEnum)	\
    (This)->lpVtbl -> Clone(This,ppEnum)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT STDMETHODCALLTYPE IEnumOleUndoUnits_RemoteNext_Proxy( 
    IEnumOleUndoUnits __RPC_FAR * This,
    /* [in] */ ULONG cElt,
    /* [length_is][size_is][out] */ IOleUndoUnit __RPC_FAR *__RPC_FAR *rgElt,
    /* [out] */ ULONG __RPC_FAR *pcEltFetched);


void __RPC_STUB IEnumOleUndoUnits_RemoteNext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumOleUndoUnits_Skip_Proxy( 
    IEnumOleUndoUnits __RPC_FAR * This,
    /* [in] */ ULONG cElt);


void __RPC_STUB IEnumOleUndoUnits_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumOleUndoUnits_Reset_Proxy( 
    IEnumOleUndoUnits __RPC_FAR * This);


void __RPC_STUB IEnumOleUndoUnits_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumOleUndoUnits_Clone_Proxy( 
    IEnumOleUndoUnits __RPC_FAR * This,
    /* [out] */ IEnumOleUndoUnits __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IEnumOleUndoUnits_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumOleUndoUnits_INTERFACE_DEFINED__ */


#ifndef __IOleUndoManager_INTERFACE_DEFINED__
#define __IOleUndoManager_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOleUndoManager
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [uuid][unique][object] */ 


#define SID_SOleUndoManager IID_IOleUndoManager;
typedef IOleUndoManager __RPC_FAR *LPOLEUNDOMANAGER;


EXTERN_C const IID IID_IOleUndoManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IOleUndoManager : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ IOleParentUndoUnit __RPC_FAR *pPUU) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Close( 
            /* [in] */ IOleParentUndoUnit __RPC_FAR *pPUU,
            /* [in] */ BOOL fCommit) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ IOleUndoUnit __RPC_FAR *pUU) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetOpenParentState( 
            /* [out] */ DWORD __RPC_FAR *pdwState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DiscardFrom( 
            /* [in] */ IOleUndoUnit __RPC_FAR *pUU) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UndoTo( 
            /* [in] */ IOleUndoUnit __RPC_FAR *pUU) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RedoTo( 
            /* [in] */ IOleUndoUnit __RPC_FAR *pUU) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumUndoable( 
            /* [out] */ IEnumOleUndoUnits __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumRedoable( 
            /* [out] */ IEnumOleUndoUnits __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLastUndoDescription( 
            /* [out] */ BSTR __RPC_FAR *pBstr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLastRedoDescription( 
            /* [out] */ BSTR __RPC_FAR *pBstr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Enable( 
            /* [in] */ BOOL fEnable) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOleUndoManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOleUndoManager __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOleUndoManager __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOleUndoManager __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open )( 
            IOleUndoManager __RPC_FAR * This,
            /* [in] */ IOleParentUndoUnit __RPC_FAR *pPUU);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Close )( 
            IOleUndoManager __RPC_FAR * This,
            /* [in] */ IOleParentUndoUnit __RPC_FAR *pPUU,
            /* [in] */ BOOL fCommit);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add )( 
            IOleUndoManager __RPC_FAR * This,
            /* [in] */ IOleUndoUnit __RPC_FAR *pUU);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetOpenParentState )( 
            IOleUndoManager __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwState);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DiscardFrom )( 
            IOleUndoManager __RPC_FAR * This,
            /* [in] */ IOleUndoUnit __RPC_FAR *pUU);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UndoTo )( 
            IOleUndoManager __RPC_FAR * This,
            /* [in] */ IOleUndoUnit __RPC_FAR *pUU);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RedoTo )( 
            IOleUndoManager __RPC_FAR * This,
            /* [in] */ IOleUndoUnit __RPC_FAR *pUU);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumUndoable )( 
            IOleUndoManager __RPC_FAR * This,
            /* [out] */ IEnumOleUndoUnits __RPC_FAR *__RPC_FAR *ppEnum);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumRedoable )( 
            IOleUndoManager __RPC_FAR * This,
            /* [out] */ IEnumOleUndoUnits __RPC_FAR *__RPC_FAR *ppEnum);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLastUndoDescription )( 
            IOleUndoManager __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *pBstr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLastRedoDescription )( 
            IOleUndoManager __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *pBstr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Enable )( 
            IOleUndoManager __RPC_FAR * This,
            /* [in] */ BOOL fEnable);
        
        END_INTERFACE
    } IOleUndoManagerVtbl;

    interface IOleUndoManager
    {
        CONST_VTBL struct IOleUndoManagerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOleUndoManager_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOleUndoManager_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOleUndoManager_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOleUndoManager_Open(This,pPUU)	\
    (This)->lpVtbl -> Open(This,pPUU)

#define IOleUndoManager_Close(This,pPUU,fCommit)	\
    (This)->lpVtbl -> Close(This,pPUU,fCommit)

#define IOleUndoManager_Add(This,pUU)	\
    (This)->lpVtbl -> Add(This,pUU)

#define IOleUndoManager_GetOpenParentState(This,pdwState)	\
    (This)->lpVtbl -> GetOpenParentState(This,pdwState)

#define IOleUndoManager_DiscardFrom(This,pUU)	\
    (This)->lpVtbl -> DiscardFrom(This,pUU)

#define IOleUndoManager_UndoTo(This,pUU)	\
    (This)->lpVtbl -> UndoTo(This,pUU)

#define IOleUndoManager_RedoTo(This,pUU)	\
    (This)->lpVtbl -> RedoTo(This,pUU)

#define IOleUndoManager_EnumUndoable(This,ppEnum)	\
    (This)->lpVtbl -> EnumUndoable(This,ppEnum)

#define IOleUndoManager_EnumRedoable(This,ppEnum)	\
    (This)->lpVtbl -> EnumRedoable(This,ppEnum)

#define IOleUndoManager_GetLastUndoDescription(This,pBstr)	\
    (This)->lpVtbl -> GetLastUndoDescription(This,pBstr)

#define IOleUndoManager_GetLastRedoDescription(This,pBstr)	\
    (This)->lpVtbl -> GetLastRedoDescription(This,pBstr)

#define IOleUndoManager_Enable(This,fEnable)	\
    (This)->lpVtbl -> Enable(This,fEnable)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOleUndoManager_Open_Proxy( 
    IOleUndoManager __RPC_FAR * This,
    /* [in] */ IOleParentUndoUnit __RPC_FAR *pPUU);


void __RPC_STUB IOleUndoManager_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleUndoManager_Close_Proxy( 
    IOleUndoManager __RPC_FAR * This,
    /* [in] */ IOleParentUndoUnit __RPC_FAR *pPUU,
    /* [in] */ BOOL fCommit);


void __RPC_STUB IOleUndoManager_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleUndoManager_Add_Proxy( 
    IOleUndoManager __RPC_FAR * This,
    /* [in] */ IOleUndoUnit __RPC_FAR *pUU);


void __RPC_STUB IOleUndoManager_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleUndoManager_GetOpenParentState_Proxy( 
    IOleUndoManager __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwState);


void __RPC_STUB IOleUndoManager_GetOpenParentState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleUndoManager_DiscardFrom_Proxy( 
    IOleUndoManager __RPC_FAR * This,
    /* [in] */ IOleUndoUnit __RPC_FAR *pUU);


void __RPC_STUB IOleUndoManager_DiscardFrom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleUndoManager_UndoTo_Proxy( 
    IOleUndoManager __RPC_FAR * This,
    /* [in] */ IOleUndoUnit __RPC_FAR *pUU);


void __RPC_STUB IOleUndoManager_UndoTo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleUndoManager_RedoTo_Proxy( 
    IOleUndoManager __RPC_FAR * This,
    /* [in] */ IOleUndoUnit __RPC_FAR *pUU);


void __RPC_STUB IOleUndoManager_RedoTo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleUndoManager_EnumUndoable_Proxy( 
    IOleUndoManager __RPC_FAR * This,
    /* [out] */ IEnumOleUndoUnits __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IOleUndoManager_EnumUndoable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleUndoManager_EnumRedoable_Proxy( 
    IOleUndoManager __RPC_FAR * This,
    /* [out] */ IEnumOleUndoUnits __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IOleUndoManager_EnumRedoable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleUndoManager_GetLastUndoDescription_Proxy( 
    IOleUndoManager __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *pBstr);


void __RPC_STUB IOleUndoManager_GetLastUndoDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleUndoManager_GetLastRedoDescription_Proxy( 
    IOleUndoManager __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *pBstr);


void __RPC_STUB IOleUndoManager_GetLastRedoDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOleUndoManager_Enable_Proxy( 
    IOleUndoManager __RPC_FAR * This,
    /* [in] */ BOOL fEnable);


void __RPC_STUB IOleUndoManager_Enable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOleUndoManager_INTERFACE_DEFINED__ */


#ifndef __IQuickActivate_INTERFACE_DEFINED__
#define __IQuickActivate_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IQuickActivate
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [uuid][unique][object][local] */ 


typedef IQuickActivate __RPC_FAR *LPQUICKACTIVATE;

typedef /* [v1_enum] */ 
enum tagQACONTAINERFLAGS
    {	QACONTAINER_SHOWHATCHING	= 0x1,
	QACONTAINER_SHOWGRABHANDLES	= 0x2,
	QACONTAINER_USERMODE	= 0x4,
	QACONTAINER_DISPLAYASDEFAULT	= 0x8,
	QACONTAINER_UIDEAD	= 0x10,
	QACONTAINER_AUTOCLIP	= 0x20,
	QACONTAINER_MESSAGEREFLECT	= 0x40,
	QACONTAINER_SUPPORTSMNEMONICS	= 0x80
    }	QACONTAINERFLAGS;

typedef DWORD OLE_COLOR;

typedef struct  tagQACONTAINER
    {
    ULONG cbSize;
    IOleClientSite __RPC_FAR *pClientSite;
    IAdviseSinkEx __RPC_FAR *pAdviseSink;
    IPropertyNotifySink __RPC_FAR *pPropertyNotifySink;
    IUnknown __RPC_FAR *pUnkEventSink;
    DWORD dwAmbientFlags;
    OLE_COLOR colorFore;
    OLE_COLOR colorBack;
    IFont __RPC_FAR *pFont;
    IOleUndoManager __RPC_FAR *pUndoMgr;
    DWORD dwAppearance;
    LONG lcid;
    HPALETTE hpal;
    struct IBindHost __RPC_FAR *pBindHost;
    }	QACONTAINER;

typedef struct  tagQACONTROL
    {
    ULONG cbSize;
    DWORD dwMiscStatus;
    DWORD dwViewStatus;
    DWORD dwEventCookie;
    DWORD dwPropNotifyCookie;
    DWORD dwPointerActivationPolicy;
    }	QACONTROL;


EXTERN_C const IID IID_IQuickActivate;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IQuickActivate : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE QuickActivate( 
            /* [in] */ QACONTAINER __RPC_FAR *pQaContainer,
            /* [out] */ QACONTROL __RPC_FAR *pQaControl) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetContentExtent( 
            LPSIZEL pSizel) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetContentExtent( 
            LPSIZEL pSizel) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IQuickActivateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IQuickActivate __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IQuickActivate __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IQuickActivate __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QuickActivate )( 
            IQuickActivate __RPC_FAR * This,
            /* [in] */ QACONTAINER __RPC_FAR *pQaContainer,
            /* [out] */ QACONTROL __RPC_FAR *pQaControl);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetContentExtent )( 
            IQuickActivate __RPC_FAR * This,
            LPSIZEL pSizel);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetContentExtent )( 
            IQuickActivate __RPC_FAR * This,
            LPSIZEL pSizel);
        
        END_INTERFACE
    } IQuickActivateVtbl;

    interface IQuickActivate
    {
        CONST_VTBL struct IQuickActivateVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IQuickActivate_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IQuickActivate_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IQuickActivate_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IQuickActivate_QuickActivate(This,pQaContainer,pQaControl)	\
    (This)->lpVtbl -> QuickActivate(This,pQaContainer,pQaControl)

#define IQuickActivate_SetContentExtent(This,pSizel)	\
    (This)->lpVtbl -> SetContentExtent(This,pSizel)

#define IQuickActivate_GetContentExtent(This,pSizel)	\
    (This)->lpVtbl -> GetContentExtent(This,pSizel)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IQuickActivate_QuickActivate_Proxy( 
    IQuickActivate __RPC_FAR * This,
    /* [in] */ QACONTAINER __RPC_FAR *pQaContainer,
    /* [out] */ QACONTROL __RPC_FAR *pQaControl);


void __RPC_STUB IQuickActivate_QuickActivate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQuickActivate_SetContentExtent_Proxy( 
    IQuickActivate __RPC_FAR * This,
    LPSIZEL pSizel);


void __RPC_STUB IQuickActivate_SetContentExtent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQuickActivate_GetContentExtent_Proxy( 
    IQuickActivate __RPC_FAR * This,
    LPSIZEL pSizel);


void __RPC_STUB IQuickActivate_GetContentExtent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IQuickActivate_INTERFACE_DEFINED__ */


#ifndef __IPointerInactive_INTERFACE_DEFINED__
#define __IPointerInactive_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IPointerInactive
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [uuid][unique][object] */ 


typedef IPointerInactive __RPC_FAR *LPPOINTERINACTIVE;

typedef /* [v1_enum] */ 
enum tagPOINTERINACTIVE
    {	POINTERINACTIVE_ACTIVATEONENTRY	= 1,
	POINTERINACTIVE_DEACTIVATEONLEAVE	= 2,
	POINTERINACTIVE_ACTIVATEONDRAG	= 4
    }	POINTERINACTIVE;


EXTERN_C const IID IID_IPointerInactive;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IPointerInactive : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetActivationPolicy( 
            /* [out] */ DWORD __RPC_FAR *pdwPolicy) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnInactiveMouseMove( 
            /* [in] */ LPCRECT pRectBounds,
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [in] */ DWORD grfKeyState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnInactiveSetCursor( 
            /* [in] */ LPCRECT pRectBounds,
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [in] */ DWORD dwMouseMsg,
            /* [in] */ BOOL fSetAlways) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPointerInactiveVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPointerInactive __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPointerInactive __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPointerInactive __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetActivationPolicy )( 
            IPointerInactive __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwPolicy);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnInactiveMouseMove )( 
            IPointerInactive __RPC_FAR * This,
            /* [in] */ LPCRECT pRectBounds,
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [in] */ DWORD grfKeyState);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnInactiveSetCursor )( 
            IPointerInactive __RPC_FAR * This,
            /* [in] */ LPCRECT pRectBounds,
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [in] */ DWORD dwMouseMsg,
            /* [in] */ BOOL fSetAlways);
        
        END_INTERFACE
    } IPointerInactiveVtbl;

    interface IPointerInactive
    {
        CONST_VTBL struct IPointerInactiveVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPointerInactive_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPointerInactive_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPointerInactive_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPointerInactive_GetActivationPolicy(This,pdwPolicy)	\
    (This)->lpVtbl -> GetActivationPolicy(This,pdwPolicy)

#define IPointerInactive_OnInactiveMouseMove(This,pRectBounds,x,y,grfKeyState)	\
    (This)->lpVtbl -> OnInactiveMouseMove(This,pRectBounds,x,y,grfKeyState)

#define IPointerInactive_OnInactiveSetCursor(This,pRectBounds,x,y,dwMouseMsg,fSetAlways)	\
    (This)->lpVtbl -> OnInactiveSetCursor(This,pRectBounds,x,y,dwMouseMsg,fSetAlways)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IPointerInactive_GetActivationPolicy_Proxy( 
    IPointerInactive __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwPolicy);


void __RPC_STUB IPointerInactive_GetActivationPolicy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPointerInactive_OnInactiveMouseMove_Proxy( 
    IPointerInactive __RPC_FAR * This,
    /* [in] */ LPCRECT pRectBounds,
    /* [in] */ LONG x,
    /* [in] */ LONG y,
    /* [in] */ DWORD grfKeyState);


void __RPC_STUB IPointerInactive_OnInactiveMouseMove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPointerInactive_OnInactiveSetCursor_Proxy( 
    IPointerInactive __RPC_FAR * This,
    /* [in] */ LPCRECT pRectBounds,
    /* [in] */ LONG x,
    /* [in] */ LONG y,
    /* [in] */ DWORD dwMouseMsg,
    /* [in] */ BOOL fSetAlways);


void __RPC_STUB IPointerInactive_OnInactiveSetCursor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPointerInactive_INTERFACE_DEFINED__ */


#ifndef __IObjectWithSite_INTERFACE_DEFINED__
#define __IObjectWithSite_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IObjectWithSite
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IObjectWithSite __RPC_FAR *LPOBJECTWITHSITE;


EXTERN_C const IID IID_IObjectWithSite;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IObjectWithSite : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetSite( 
            /* [in] */ IUnknown __RPC_FAR *pUnkSite) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSite( 
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvSite) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IObjectWithSiteVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IObjectWithSite __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IObjectWithSite __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IObjectWithSite __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSite )( 
            IObjectWithSite __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *pUnkSite);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSite )( 
            IObjectWithSite __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvSite);
        
        END_INTERFACE
    } IObjectWithSiteVtbl;

    interface IObjectWithSite
    {
        CONST_VTBL struct IObjectWithSiteVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IObjectWithSite_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IObjectWithSite_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IObjectWithSite_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IObjectWithSite_SetSite(This,pUnkSite)	\
    (This)->lpVtbl -> SetSite(This,pUnkSite)

#define IObjectWithSite_GetSite(This,riid,ppvSite)	\
    (This)->lpVtbl -> GetSite(This,riid,ppvSite)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IObjectWithSite_SetSite_Proxy( 
    IObjectWithSite __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *pUnkSite);


void __RPC_STUB IObjectWithSite_SetSite_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IObjectWithSite_GetSite_Proxy( 
    IObjectWithSite __RPC_FAR * This,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvSite);


void __RPC_STUB IObjectWithSite_GetSite_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IObjectWithSite_INTERFACE_DEFINED__ */


#ifndef __IErrorLog_INTERFACE_DEFINED__
#define __IErrorLog_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IErrorLog
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IErrorLog __RPC_FAR *LPERRORLOG;


EXTERN_C const IID IID_IErrorLog;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IErrorLog : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AddError( 
            /* [in] */ LPCOLESTR pszPropName,
            /* [in] */ EXCEPINFO __RPC_FAR *pExcepInfo) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IErrorLogVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IErrorLog __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IErrorLog __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IErrorLog __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddError )( 
            IErrorLog __RPC_FAR * This,
            /* [in] */ LPCOLESTR pszPropName,
            /* [in] */ EXCEPINFO __RPC_FAR *pExcepInfo);
        
        END_INTERFACE
    } IErrorLogVtbl;

    interface IErrorLog
    {
        CONST_VTBL struct IErrorLogVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IErrorLog_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IErrorLog_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IErrorLog_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IErrorLog_AddError(This,pszPropName,pExcepInfo)	\
    (This)->lpVtbl -> AddError(This,pszPropName,pExcepInfo)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IErrorLog_AddError_Proxy( 
    IErrorLog __RPC_FAR * This,
    /* [in] */ LPCOLESTR pszPropName,
    /* [in] */ EXCEPINFO __RPC_FAR *pExcepInfo);


void __RPC_STUB IErrorLog_AddError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IErrorLog_INTERFACE_DEFINED__ */


#ifndef __IPropertyBag_INTERFACE_DEFINED__
#define __IPropertyBag_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IPropertyBag
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IPropertyBag __RPC_FAR *LPPROPERTYBAG;


EXTERN_C const IID IID_IPropertyBag;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IPropertyBag : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Read( 
            /* [in] */ LPCOLESTR pszPropName,
            /* [out][in] */ VARIANT __RPC_FAR *pVar,
            /* [in] */ IErrorLog __RPC_FAR *pErrorLog) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Write( 
            /* [in] */ LPCOLESTR pszPropName,
            /* [in] */ VARIANT __RPC_FAR *pVar) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPropertyBagVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPropertyBag __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPropertyBag __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPropertyBag __RPC_FAR * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Read )( 
            IPropertyBag __RPC_FAR * This,
            /* [in] */ LPCOLESTR pszPropName,
            /* [out][in] */ VARIANT __RPC_FAR *pVar,
            /* [in] */ IErrorLog __RPC_FAR *pErrorLog);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Write )( 
            IPropertyBag __RPC_FAR * This,
            /* [in] */ LPCOLESTR pszPropName,
            /* [in] */ VARIANT __RPC_FAR *pVar);
        
        END_INTERFACE
    } IPropertyBagVtbl;

    interface IPropertyBag
    {
        CONST_VTBL struct IPropertyBagVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPropertyBag_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPropertyBag_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPropertyBag_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPropertyBag_Read(This,pszPropName,pVar,pErrorLog)	\
    (This)->lpVtbl -> Read(This,pszPropName,pVar,pErrorLog)

#define IPropertyBag_Write(This,pszPropName,pVar)	\
    (This)->lpVtbl -> Write(This,pszPropName,pVar)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT STDMETHODCALLTYPE IPropertyBag_RemoteRead_Proxy( 
    IPropertyBag __RPC_FAR * This,
    /* [in] */ LPCOLESTR pszPropName,
    /* [out] */ VARIANT __RPC_FAR *pVar,
    /* [in] */ IErrorLog __RPC_FAR *pErrorLog,
    /* [in] */ DWORD varType,
    /* [in] */ IUnknown __RPC_FAR *pUnkObj);


void __RPC_STUB IPropertyBag_RemoteRead_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPropertyBag_Write_Proxy( 
    IPropertyBag __RPC_FAR * This,
    /* [in] */ LPCOLESTR pszPropName,
    /* [in] */ VARIANT __RPC_FAR *pVar);


void __RPC_STUB IPropertyBag_Write_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPropertyBag_INTERFACE_DEFINED__ */


#ifndef __IPerPropertyBrowsing_INTERFACE_DEFINED__
#define __IPerPropertyBrowsing_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IPerPropertyBrowsing
 * at Sat Jul 13 21:56:58 1996
 * using MIDL 3.00.39
 ****************************************/
/* [unique][uuid][object] */ 


typedef IPerPropertyBrowsing __RPC_FAR *LPPERPROPERTYBROWSING;

typedef struct  tagCALPOLESTR
    {
    ULONG cElems;
    /* [size_is] */ LPOLESTR __RPC_FAR *pElems;
    }	CALPOLESTR;

typedef struct tagCALPOLESTR __RPC_FAR *LPCALPOLESTR;

typedef struct  tagCADWORD
    {
    ULONG cElems;
    /* [size_is] */ DWORD __RPC_FAR *pElems;
    }	CADWORD;

typedef struct tagCADWORD __RPC_FAR *LPCADWORD;


EXTERN_C const IID IID_IPerPropertyBrowsing;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IPerPropertyBrowsing : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetDisplayString( 
            /* [in] */ DISPID dispID,
            /* [out] */ BSTR __RPC_FAR *pBstr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MapPropertyToPage( 
            /* [in] */ DISPID dispID,
            /* [out] */ CLSID __RPC_FAR *pClsid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPredefinedStrings( 
            /* [in] */ DISPID dispID,
            /* [out] */ CALPOLESTR __RPC_FAR *pCaStringsOut,
            /* [out] */ CADWORD __RPC_FAR *pCaCookiesOut) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPredefinedValue( 
            /* [in] */ DISPID dispID,
            /* [in] */ DWORD dwCookie,
            /* [out] */ VARIANT __RPC_FAR *pVarOut) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPerPropertyBrowsingVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPerPropertyBrowsing __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPerPropertyBrowsing __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPerPropertyBrowsing __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDisplayString )( 
            IPerPropertyBrowsing __RPC_FAR * This,
            /* [in] */ DISPID dispID,
            /* [out] */ BSTR __RPC_FAR *pBstr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MapPropertyToPage )( 
            IPerPropertyBrowsing __RPC_FAR * This,
            /* [in] */ DISPID dispID,
            /* [out] */ CLSID __RPC_FAR *pClsid);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPredefinedStrings )( 
            IPerPropertyBrowsing __RPC_FAR * This,
            /* [in] */ DISPID dispID,
            /* [out] */ CALPOLESTR __RPC_FAR *pCaStringsOut,
            /* [out] */ CADWORD __RPC_FAR *pCaCookiesOut);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPredefinedValue )( 
            IPerPropertyBrowsing __RPC_FAR * This,
            /* [in] */ DISPID dispID,
            /* [in] */ DWORD dwCookie,
            /* [out] */ VARIANT __RPC_FAR *pVarOut);
        
        END_INTERFACE
    } IPerPropertyBrowsingVtbl;

    interface IPerPropertyBrowsing
    {
        CONST_VTBL struct IPerPropertyBrowsingVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPerPropertyBrowsing_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPerPropertyBrowsing_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPerPropertyBrowsing_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPerPropertyBrowsing_GetDisplayString(This,dispID,pBstr)	\
    (This)->lpVtbl -> GetDisplayString(This,dispID,pBstr)

#define IPerPropertyBrowsing_MapPropertyToPage(This,dispID,pClsid)	\
    (This)->lpVtbl -> MapPropertyToPage(This,dispID,pClsid)

#define IPerPropertyBrowsing_GetPredefinedStrings(This,dispID,pCaStringsOut,pCaCookiesOut)	\
    (This)->lpVtbl -> GetPredefinedStrings(This,dispID,pCaStringsOut,pCaCookiesOut)

#define IPerPropertyBrowsing_GetPredefinedValue(This,dispID,dwCookie,pVarOut)	\
    (This)->lpVtbl -> GetPredefinedValue(This,dispID,dwCookie,pVarOut)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IPerPropertyBrowsing_GetDisplayString_Proxy( 
    IPerPropertyBrowsing __RPC_FAR * This,
    /* [in] */ DISPID dispID,
    /* [out] */ BSTR __RPC_FAR *pBstr);


void __RPC_STUB IPerPropertyBrowsing_GetDisplayString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPerPropertyBrowsing_MapPropertyToPage_Proxy( 
    IPerPropertyBrowsing __RPC_FAR * This,
    /* [in] */ DISPID dispID,
    /* [out] */ CLSID __RPC_FAR *pClsid);


void __RPC_STUB IPerPropertyBrowsing_MapPropertyToPage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPerPropertyBrowsing_GetPredefinedStrings_Proxy( 
    IPerPropertyBrowsing __RPC_FAR * This,
    /* [in] */ DISPID dispID,
    /* [out] */ CALPOLESTR __RPC_FAR *pCaStringsOut,
    /* [out] */ CADWORD __RPC_FAR *pCaCookiesOut);


void __RPC_STUB IPerPropertyBrowsing_GetPredefinedStrings_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPerPropertyBrowsing_GetPredefinedValue_Proxy( 
    IPerPropertyBrowsing __RPC_FAR * This,
    /* [in] */ DISPID dispID,
    /* [in] */ DWORD dwCookie,
    /* [out] */ VARIANT __RPC_FAR *pVarOut);


void __RPC_STUB IPerPropertyBrowsing_GetPredefinedValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPerPropertyBrowsing_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */


void __RPC_USER UserHWND_from_local( HWND __RPC_FAR *, UserHWND __RPC_FAR * __RPC_FAR * );
void __RPC_USER UserHWND_to_local( UserHWND __RPC_FAR *, HWND __RPC_FAR * );
 void __RPC_USER UserHWND_free_inst( UserHWND __RPC_FAR * );
void __RPC_USER UserHWND_free_local( HWND __RPC_FAR * );

void __RPC_USER UserHACCEL_from_local( HACCEL __RPC_FAR *, UserHACCEL __RPC_FAR * __RPC_FAR * );
void __RPC_USER UserHACCEL_to_local( UserHACCEL __RPC_FAR *, HACCEL __RPC_FAR * );
 void __RPC_USER UserHACCEL_free_inst( UserHACCEL __RPC_FAR * );
void __RPC_USER UserHACCEL_free_local( HACCEL __RPC_FAR * );

void __RPC_USER UserHDC_from_local( HDC __RPC_FAR *, UserHDC __RPC_FAR * __RPC_FAR * );
void __RPC_USER UserHDC_to_local( UserHDC __RPC_FAR *, HDC __RPC_FAR * );
 void __RPC_USER UserHDC_free_inst( UserHDC __RPC_FAR * );
void __RPC_USER UserHDC_free_local( HDC __RPC_FAR * );

void __RPC_USER UserHFONT_from_local( HFONT __RPC_FAR *, UserHFONT __RPC_FAR * __RPC_FAR * );
void __RPC_USER UserHFONT_to_local( UserHFONT __RPC_FAR *, HFONT __RPC_FAR * );
 void __RPC_USER UserHFONT_free_inst( UserHFONT __RPC_FAR * );
void __RPC_USER UserHFONT_free_local( HFONT __RPC_FAR * );

void __RPC_USER UserMSG_from_local( MSG __RPC_FAR *, UserMSG __RPC_FAR * __RPC_FAR * );
void __RPC_USER UserMSG_to_local( UserMSG __RPC_FAR *, MSG __RPC_FAR * );
 void __RPC_USER UserMSG_free_inst( UserMSG __RPC_FAR * );
void __RPC_USER UserMSG_free_local( MSG __RPC_FAR * );

void __RPC_USER UserBSTR_from_local( BSTR __RPC_FAR *, UserBSTR __RPC_FAR * __RPC_FAR * );
void __RPC_USER UserBSTR_to_local( UserBSTR __RPC_FAR *, BSTR __RPC_FAR * );
 void __RPC_USER UserBSTR_free_inst( UserBSTR __RPC_FAR * );
void __RPC_USER UserBSTR_free_local( BSTR __RPC_FAR * );

void __RPC_USER UserVARIANT_from_local( VARIANT __RPC_FAR *, UserVARIANT __RPC_FAR * __RPC_FAR * );
void __RPC_USER UserVARIANT_to_local( UserVARIANT __RPC_FAR *, VARIANT __RPC_FAR * );
 void __RPC_USER UserVARIANT_free_inst( UserVARIANT __RPC_FAR * );
void __RPC_USER UserVARIANT_free_local( VARIANT __RPC_FAR * );

void __RPC_USER UserEXCEPINFO_from_local( EXCEPINFO __RPC_FAR *, UserEXCEPINFO __RPC_FAR * __RPC_FAR * );
void __RPC_USER UserEXCEPINFO_to_local( UserEXCEPINFO __RPC_FAR *, EXCEPINFO __RPC_FAR * );
 void __RPC_USER UserEXCEPINFO_free_inst( UserEXCEPINFO __RPC_FAR * );
void __RPC_USER UserEXCEPINFO_free_local( EXCEPINFO __RPC_FAR * );

/* [local] */ HRESULT STDMETHODCALLTYPE IEnumConnections_Next_Proxy( 
    IEnumConnections __RPC_FAR * This,
    /* [in] */ ULONG cConnections,
    /* [length_is][size_is][out] */ LPCONNECTDATA rgcd,
    /* [out] */ ULONG __RPC_FAR *pcFetched);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IEnumConnections_Next_Stub( 
    IEnumConnections __RPC_FAR * This,
    /* [in] */ ULONG cConnections,
    /* [length_is][size_is][out] */ LPCONNECTDATA rgcd,
    /* [out] */ ULONG __RPC_FAR *pcFetched);

/* [local] */ HRESULT STDMETHODCALLTYPE IEnumConnectionPoints_Next_Proxy( 
    IEnumConnectionPoints __RPC_FAR * This,
    /* [in] */ ULONG cConnections,
    /* [length_is][size_is][out] */ LPCONNECTIONPOINT __RPC_FAR *ppCP,
    /* [out] */ ULONG __RPC_FAR *pcFetched);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IEnumConnectionPoints_Next_Stub( 
    IEnumConnectionPoints __RPC_FAR * This,
    /* [in] */ ULONG cConnections,
    /* [length_is][size_is][out] */ LPCONNECTIONPOINT __RPC_FAR *ppCP,
    /* [out] */ ULONG __RPC_FAR *pcFetched);

/* [local] */ HRESULT STDMETHODCALLTYPE IClassFactory2_CreateInstanceLic_Proxy( 
    IClassFactory2 __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *pUnkOuter,
    /* [in] */ IUnknown __RPC_FAR *pUnkReserved,
    /* [in] */ REFIID riid,
    /* [in] */ BSTR bstrKey,
    /* [iid_is][out] */ PVOID __RPC_FAR *ppvObj);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IClassFactory2_CreateInstanceLic_Stub( 
    IClassFactory2 __RPC_FAR * This,
    /* [in] */ REFIID riid,
    /* [in] */ BSTR bstrKey,
    /* [iid_is][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppvObj);

/* [local] */ HRESULT STDMETHODCALLTYPE IPersistMemory_Load_Proxy( 
    IPersistMemory __RPC_FAR * This,
    /* [size_is][in] */ LPVOID pMem,
    /* [in] */ ULONG cbSize);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IPersistMemory_Load_Stub( 
    IPersistMemory __RPC_FAR * This,
    /* [size_is][in] */ BYTE __RPC_FAR *pMem,
    /* [in] */ ULONG cbSize);

/* [local] */ HRESULT STDMETHODCALLTYPE IPersistMemory_Save_Proxy( 
    IPersistMemory __RPC_FAR * This,
    /* [size_is][in] */ LPVOID pMem,
    /* [in] */ BOOL fClearDirty,
    /* [in] */ ULONG cbSize);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IPersistMemory_Save_Stub( 
    IPersistMemory __RPC_FAR * This,
    /* [size_is][in] */ BYTE __RPC_FAR *pMem,
    /* [in] */ BOOL fClearDirty,
    /* [in] */ ULONG cbSize);

/* [local] */ void STDMETHODCALLTYPE IAdviseSinkEx_OnViewStatusChange_Proxy( 
    IAdviseSinkEx __RPC_FAR * This,
    /* [in] */ DWORD dwViewStatus);


/* [async][call_as] */ void STDMETHODCALLTYPE IAdviseSinkEx_OnViewStatusChange_Stub( 
    IAdviseSinkEx __RPC_FAR * This,
    /* [in] */ DWORD dwViewStatus);

/* [local] */ HRESULT STDMETHODCALLTYPE IEnumOleUndoUnits_Next_Proxy( 
    IEnumOleUndoUnits __RPC_FAR * This,
    /* [in] */ ULONG cElt,
    /* [length_is][size_is][out] */ IOleUndoUnit __RPC_FAR *__RPC_FAR *rgElt,
    /* [out] */ ULONG __RPC_FAR *pcEltFetched);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IEnumOleUndoUnits_Next_Stub( 
    IEnumOleUndoUnits __RPC_FAR * This,
    /* [in] */ ULONG cElt,
    /* [length_is][size_is][out] */ IOleUndoUnit __RPC_FAR *__RPC_FAR *rgElt,
    /* [out] */ ULONG __RPC_FAR *pcEltFetched);

/* [local] */ HRESULT STDMETHODCALLTYPE IPropertyBag_Read_Proxy( 
    IPropertyBag __RPC_FAR * This,
    /* [in] */ LPCOLESTR pszPropName,
    /* [out][in] */ VARIANT __RPC_FAR *pVar,
    /* [in] */ IErrorLog __RPC_FAR *pErrorLog);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IPropertyBag_Read_Stub( 
    IPropertyBag __RPC_FAR * This,
    /* [in] */ LPCOLESTR pszPropName,
    /* [out] */ VARIANT __RPC_FAR *pVar,
    /* [in] */ IErrorLog __RPC_FAR *pErrorLog,
    /* [in] */ DWORD varType,
    /* [in] */ IUnknown __RPC_FAR *pUnkObj);



/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
