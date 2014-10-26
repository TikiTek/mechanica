

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0606 */
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

/* verify that the <rpcsal.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCSAL_H_VERSION__
#define __REQUIRED_RPCSAL_H_VERSION__ 100
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __d3d12sdklayers_h__
#define __d3d12sdklayers_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ID3D12Debug_FWD_DEFINED__
#define __ID3D12Debug_FWD_DEFINED__
typedef interface ID3D12Debug ID3D12Debug;

#endif 	/* __ID3D12Debug_FWD_DEFINED__ */


#ifndef __ID3D12DebugCommandQueue_FWD_DEFINED__
#define __ID3D12DebugCommandQueue_FWD_DEFINED__
typedef interface ID3D12DebugCommandQueue ID3D12DebugCommandQueue;

#endif 	/* __ID3D12DebugCommandQueue_FWD_DEFINED__ */


#ifndef __ID3D12DebugCommandList_FWD_DEFINED__
#define __ID3D12DebugCommandList_FWD_DEFINED__
typedef interface ID3D12DebugCommandList ID3D12DebugCommandList;

#endif 	/* __ID3D12DebugCommandList_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "d3d12.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ID3D12Debug_INTERFACE_DEFINED__
#define __ID3D12Debug_INTERFACE_DEFINED__

/* interface ID3D12Debug */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12Debug;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3febd6dd-4973-4787-8194-e45f9e28923e")
    ID3D12Debug : public IUnknown
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12DebugVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12Debug * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12Debug * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12Debug * This);
        
        END_INTERFACE
    } ID3D12DebugVtbl;

    interface ID3D12Debug
    {
        CONST_VTBL struct ID3D12DebugVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12Debug_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12Debug_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12Debug_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ID3D12Debug_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_d3d12sdklayers_0000_0001 */
/* [local] */ 

DEFINE_GUID(DXGI_DEBUG_D3D12, 0xcf59a98c, 0xa950, 0x4326, 0x91, 0xef, 0x9b, 0xba, 0xa1, 0x7b, 0xfd, 0x95);


extern RPC_IF_HANDLE __MIDL_itf_d3d12sdklayers_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d12sdklayers_0000_0001_v0_0_s_ifspec;

#ifndef __ID3D12DebugCommandQueue_INTERFACE_DEFINED__
#define __ID3D12DebugCommandQueue_INTERFACE_DEFINED__

/* interface ID3D12DebugCommandQueue */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12DebugCommandQueue;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("09e0bf36-54ac-484f-8847-4baeeab6053a")
    ID3D12DebugCommandQueue : public IUnknown
    {
    public:
        virtual BOOL STDMETHODCALLTYPE AssertResourceState( 
            /* [annotation] */ 
            _In_  ID3D12Resource *pResource,
            UINT Subresource,
            UINT State) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12DebugCommandQueueVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12DebugCommandQueue * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12DebugCommandQueue * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12DebugCommandQueue * This);
        
        BOOL ( STDMETHODCALLTYPE *AssertResourceState )( 
            ID3D12DebugCommandQueue * This,
            /* [annotation] */ 
            _In_  ID3D12Resource *pResource,
            UINT Subresource,
            UINT State);
        
        END_INTERFACE
    } ID3D12DebugCommandQueueVtbl;

    interface ID3D12DebugCommandQueue
    {
        CONST_VTBL struct ID3D12DebugCommandQueueVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12DebugCommandQueue_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12DebugCommandQueue_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12DebugCommandQueue_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12DebugCommandQueue_AssertResourceState(This,pResource,Subresource,State)	\
    ( (This)->lpVtbl -> AssertResourceState(This,pResource,Subresource,State) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ID3D12DebugCommandQueue_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_d3d12sdklayers_0000_0002 */
/* [local] */ 

typedef 
enum D3D12_DEBUG_FEATURE
    {
        D3D12_DEBUG_FEATURE_TREAT_BUNDLE_AS_DRAW	= 0x1,
        D3D12_DEBUG_FEATURE_TREAT_BUNDLE_AS_DISPATCH	= 0x2
    } 	D3D12_DEBUG_FEATURE;

DEFINE_ENUM_FLAG_OPERATORS( D3D12_DEBUG_FEATURE );


extern RPC_IF_HANDLE __MIDL_itf_d3d12sdklayers_0000_0002_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d12sdklayers_0000_0002_v0_0_s_ifspec;

#ifndef __ID3D12DebugCommandList_INTERFACE_DEFINED__
#define __ID3D12DebugCommandList_INTERFACE_DEFINED__

/* interface ID3D12DebugCommandList */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12DebugCommandList;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("09e0bf36-54ac-484f-8847-4baeeab6053f")
    ID3D12DebugCommandList : public IUnknown
    {
    public:
        virtual BOOL STDMETHODCALLTYPE AssertResourceState( 
            /* [annotation] */ 
            _In_  ID3D12Resource *pResource,
            UINT Subresource,
            UINT State) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetFeatureMask( 
            D3D12_DEBUG_FEATURE Mask) = 0;
        
        virtual D3D12_DEBUG_FEATURE STDMETHODCALLTYPE GetFeatureMask( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12DebugCommandListVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12DebugCommandList * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12DebugCommandList * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12DebugCommandList * This);
        
        BOOL ( STDMETHODCALLTYPE *AssertResourceState )( 
            ID3D12DebugCommandList * This,
            /* [annotation] */ 
            _In_  ID3D12Resource *pResource,
            UINT Subresource,
            UINT State);
        
        HRESULT ( STDMETHODCALLTYPE *SetFeatureMask )( 
            ID3D12DebugCommandList * This,
            D3D12_DEBUG_FEATURE Mask);
        
        D3D12_DEBUG_FEATURE ( STDMETHODCALLTYPE *GetFeatureMask )( 
            ID3D12DebugCommandList * This);
        
        END_INTERFACE
    } ID3D12DebugCommandListVtbl;

    interface ID3D12DebugCommandList
    {
        CONST_VTBL struct ID3D12DebugCommandListVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12DebugCommandList_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12DebugCommandList_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12DebugCommandList_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12DebugCommandList_AssertResourceState(This,pResource,Subresource,State)	\
    ( (This)->lpVtbl -> AssertResourceState(This,pResource,Subresource,State) ) 

#define ID3D12DebugCommandList_SetFeatureMask(This,Mask)	\
    ( (This)->lpVtbl -> SetFeatureMask(This,Mask) ) 

#define ID3D12DebugCommandList_GetFeatureMask(This)	\
    ( (This)->lpVtbl -> GetFeatureMask(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ID3D12DebugCommandList_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_d3d12sdklayers_0000_0003 */
/* [local] */ 

DEFINE_GUID(IID_ID3D12Debug,0x3febd6dd,0x4973,0x4787,0x81,0x94,0xe4,0x5f,0x9e,0x28,0x92,0x3e);
DEFINE_GUID(IID_ID3D12DebugCommandQueue,0x09e0bf36,0x54ac,0x484f,0x88,0x47,0x4b,0xae,0xea,0xb6,0x05,0x3a);
DEFINE_GUID(IID_ID3D12DebugCommandList,0x09e0bf36,0x54ac,0x484f,0x88,0x47,0x4b,0xae,0xea,0xb6,0x05,0x3f);


extern RPC_IF_HANDLE __MIDL_itf_d3d12sdklayers_0000_0003_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d12sdklayers_0000_0003_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


