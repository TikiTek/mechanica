/*-------------------------------------------------------------------------------------
 *
 * Copyright (c) Microsoft Corporation
 *
 *-------------------------------------------------------------------------------------*/


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

#ifndef __d3d12_h__
#define __d3d12_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ID3D12Object_FWD_DEFINED__
#define __ID3D12Object_FWD_DEFINED__
typedef interface ID3D12Object ID3D12Object;

#endif 	/* __ID3D12Object_FWD_DEFINED__ */


#ifndef __ID3D12DeviceChild_FWD_DEFINED__
#define __ID3D12DeviceChild_FWD_DEFINED__
typedef interface ID3D12DeviceChild ID3D12DeviceChild;

#endif 	/* __ID3D12DeviceChild_FWD_DEFINED__ */


#ifndef __ID3D12RootSignature_FWD_DEFINED__
#define __ID3D12RootSignature_FWD_DEFINED__
typedef interface ID3D12RootSignature ID3D12RootSignature;

#endif 	/* __ID3D12RootSignature_FWD_DEFINED__ */


#ifndef __ID3D12RootSignatureDeserializer_FWD_DEFINED__
#define __ID3D12RootSignatureDeserializer_FWD_DEFINED__
typedef interface ID3D12RootSignatureDeserializer ID3D12RootSignatureDeserializer;

#endif 	/* __ID3D12RootSignatureDeserializer_FWD_DEFINED__ */


#ifndef __ID3D12Pageable_FWD_DEFINED__
#define __ID3D12Pageable_FWD_DEFINED__
typedef interface ID3D12Pageable ID3D12Pageable;

#endif 	/* __ID3D12Pageable_FWD_DEFINED__ */


#ifndef __ID3D12Heap_FWD_DEFINED__
#define __ID3D12Heap_FWD_DEFINED__
typedef interface ID3D12Heap ID3D12Heap;

#endif 	/* __ID3D12Heap_FWD_DEFINED__ */


#ifndef __ID3D12Resource_FWD_DEFINED__
#define __ID3D12Resource_FWD_DEFINED__
typedef interface ID3D12Resource ID3D12Resource;

#endif 	/* __ID3D12Resource_FWD_DEFINED__ */


#ifndef __ID3D12CommandAllocator_FWD_DEFINED__
#define __ID3D12CommandAllocator_FWD_DEFINED__
typedef interface ID3D12CommandAllocator ID3D12CommandAllocator;

#endif 	/* __ID3D12CommandAllocator_FWD_DEFINED__ */


#ifndef __ID3D12Fence_FWD_DEFINED__
#define __ID3D12Fence_FWD_DEFINED__
typedef interface ID3D12Fence ID3D12Fence;

#endif 	/* __ID3D12Fence_FWD_DEFINED__ */


#ifndef __ID3D12PipelineState_FWD_DEFINED__
#define __ID3D12PipelineState_FWD_DEFINED__
typedef interface ID3D12PipelineState ID3D12PipelineState;

#endif 	/* __ID3D12PipelineState_FWD_DEFINED__ */


#ifndef __ID3D12DescriptorHeap_FWD_DEFINED__
#define __ID3D12DescriptorHeap_FWD_DEFINED__
typedef interface ID3D12DescriptorHeap ID3D12DescriptorHeap;

#endif 	/* __ID3D12DescriptorHeap_FWD_DEFINED__ */


#ifndef __ID3D12CommandList_FWD_DEFINED__
#define __ID3D12CommandList_FWD_DEFINED__
typedef interface ID3D12CommandList ID3D12CommandList;

#endif 	/* __ID3D12CommandList_FWD_DEFINED__ */


#ifndef __ID3D12CommandQueue_FWD_DEFINED__
#define __ID3D12CommandQueue_FWD_DEFINED__
typedef interface ID3D12CommandQueue ID3D12CommandQueue;

#endif 	/* __ID3D12CommandQueue_FWD_DEFINED__ */


#ifndef __ID3D12Asynchronous_FWD_DEFINED__
#define __ID3D12Asynchronous_FWD_DEFINED__
typedef interface ID3D12Asynchronous ID3D12Asynchronous;

#endif 	/* __ID3D12Asynchronous_FWD_DEFINED__ */


#ifndef __ID3D12Library_FWD_DEFINED__
#define __ID3D12Library_FWD_DEFINED__
typedef interface ID3D12Library ID3D12Library;

#endif 	/* __ID3D12Library_FWD_DEFINED__ */


#ifndef __ID3D12Callback_FWD_DEFINED__
#define __ID3D12Callback_FWD_DEFINED__
typedef interface ID3D12Callback ID3D12Callback;

#endif 	/* __ID3D12Callback_FWD_DEFINED__ */


#ifndef __ID3D12Device_FWD_DEFINED__
#define __ID3D12Device_FWD_DEFINED__
typedef interface ID3D12Device ID3D12Device;

#endif 	/* __ID3D12Device_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "d3d11_3.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_d3d12_0000_0000 */
/* [local] */ 


#define D3D12_BINDING 1
typedef 
enum D3D12_COMMAND_LIST_TYPE
    {
        D3D12_COMMAND_LIST_TYPE_DIRECT	= 0,
        D3D12_COMMAND_LIST_TYPE_BUNDLE	= 1,
        D3D12_COMMAND_LIST_TYPE_COMPUTE	= 2,
        D3D12_COMMAND_LIST_TYPE_COPY	= 3
    } 	D3D12_COMMAND_LIST_TYPE;

typedef 
enum D3D12_PRIMITIVE_TOPOLOGY_TYPE
    {
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED	= 0,
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT	= 1,
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE	= 2,
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE	= 3,
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH	= 4
    } 	D3D12_PRIMITIVE_TOPOLOGY_TYPE;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
struct CD3D12_DEFAULT {};
extern const DECLSPEC_SELECTANY CD3D12_DEFAULT D3D12_DEFAULT;
extern "C"{
#endif
typedef 
enum D3D12_INPUT_CLASSIFICATION
    {
        D3D12_INPUT_PER_VERTEX_DATA	= 0,
        D3D12_INPUT_PER_INSTANCE_DATA	= 1
    } 	D3D12_INPUT_CLASSIFICATION;

#define	D3D12_APPEND_ALIGNED_ELEMENT	( 0xffffffff )

typedef struct D3D12_INPUT_ELEMENT_DESC
    {
    LPCSTR SemanticName;
    UINT SemanticIndex;
    DXGI_FORMAT Format;
    UINT InputSlot;
    UINT AlignedByteOffset;
    D3D12_INPUT_CLASSIFICATION InputSlotClass;
    UINT InstanceDataStepRate;
    } 	D3D12_INPUT_ELEMENT_DESC;

typedef 
enum D3D12_FILL_MODE
    {
        D3D12_FILL_WIREFRAME	= 2,
        D3D12_FILL_SOLID	= 3
    } 	D3D12_FILL_MODE;

typedef D3D_PRIMITIVE_TOPOLOGY D3D12_PRIMITIVE_TOPOLOGY;

typedef D3D_PRIMITIVE D3D12_PRIMITIVE;

typedef 
enum D3D12_CULL_MODE
    {
        D3D12_CULL_NONE	= 1,
        D3D12_CULL_FRONT	= 2,
        D3D12_CULL_BACK	= 3
    } 	D3D12_CULL_MODE;

typedef struct D3D12_SO_DECLARATION_ENTRY
    {
    UINT Stream;
    LPCSTR SemanticName;
    UINT SemanticIndex;
    BYTE StartComponent;
    BYTE ComponentCount;
    BYTE OutputSlot;
    } 	D3D12_SO_DECLARATION_ENTRY;

typedef struct D3D12_VIEWPORT
    {
    FLOAT TopLeftX;
    FLOAT TopLeftY;
    FLOAT Width;
    FLOAT Height;
    FLOAT MinDepth;
    FLOAT MaxDepth;
    } 	D3D12_VIEWPORT;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
inline bool operator==( const D3D12_VIEWPORT& l, const D3D12_VIEWPORT& r )
{
    return l.TopLeftX == r.TopLeftX && l.TopLeftY == r.TopLeftY && l.Width == r.Width &&
        l.Height == r.Height && l.MinDepth == r.MinDepth && l.MaxDepth == r.MaxDepth;
}
inline bool operator!=( const D3D12_VIEWPORT& l, const D3D12_VIEWPORT& r )
{ return !( l == r ); }
extern "C"{
#endif
typedef RECT D3D12_RECT;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
struct CD3D12_RECT : public D3D12_RECT
{
    CD3D12_RECT()
    {}
    explicit CD3D12_RECT( const D3D12_RECT& o ) :
        D3D12_RECT( o )
    {}
    explicit CD3D12_RECT(
        LONG Left,
        LONG Top,
        LONG Right,
        LONG Bottom )
    {
        left = Left;
        top = Top;
        right = Right;
        bottom = Bottom;
    }
    ~CD3D12_RECT() {}
    operator const D3D12_RECT&() const { return *this; }
};
#if defined( D3D11_NO_HELPERS )
inline bool operator==( const D3D12_RECT& l, const D3D12_RECT& r )
{
    return l.left == r.left && l.top == r.top &&
        l.right == r.right && l.bottom == r.bottom;
}
inline bool operator!=( const D3D12_RECT& l, const D3D12_RECT& r )
{ return !( l == r ); }
#endif
extern "C"{
#endif
typedef struct D3D12_BOX
    {
    UINT left;
    UINT top;
    UINT front;
    UINT right;
    UINT bottom;
    UINT back;
    } 	D3D12_BOX;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
struct CD3D12_BOX : public D3D12_BOX
{
    CD3D12_BOX()
    {}
    explicit CD3D12_BOX( const D3D12_BOX& o ) :
        D3D12_BOX( o )
    {}
    explicit CD3D12_BOX(
        LONG Left,
        LONG Right )
    {
        left = Left;
        top = 0;
        front = 0;
        right = Right;
        bottom = 1;
        back = 1;
    }
    explicit CD3D12_BOX(
        LONG Left,
        LONG Top,
        LONG Right,
        LONG Bottom )
    {
        left = Left;
        top = Top;
        front = 0;
        right = Right;
        bottom = Bottom;
        back = 1;
    }
    explicit CD3D12_BOX(
        LONG Left,
        LONG Top,
        LONG Front,
        LONG Right,
        LONG Bottom,
        LONG Back )
    {
        left = Left;
        top = Top;
        front = Front;
        right = Right;
        bottom = Bottom;
        back = Back;
    }
    ~CD3D12_BOX() {}
    operator const D3D12_BOX&() const { return *this; }
};
inline bool operator==( const D3D12_BOX& l, const D3D12_BOX& r )
{
    return l.left == r.left && l.top == r.top && l.front == r.front &&
        l.right == r.right && l.bottom == r.bottom && l.back == r.back;
}
inline bool operator!=( const D3D12_BOX& l, const D3D12_BOX& r )
{ return !( l == r ); }
extern "C"{
#endif
typedef 
enum D3D12_COMPARISON_FUNC
    {
        D3D12_COMPARISON_NEVER	= 1,
        D3D12_COMPARISON_LESS	= 2,
        D3D12_COMPARISON_EQUAL	= 3,
        D3D12_COMPARISON_LESS_EQUAL	= 4,
        D3D12_COMPARISON_GREATER	= 5,
        D3D12_COMPARISON_NOT_EQUAL	= 6,
        D3D12_COMPARISON_GREATER_EQUAL	= 7,
        D3D12_COMPARISON_ALWAYS	= 8
    } 	D3D12_COMPARISON_FUNC;

typedef 
enum D3D12_DEPTH_WRITE_MASK
    {
        D3D12_DEPTH_WRITE_MASK_ZERO	= 0,
        D3D12_DEPTH_WRITE_MASK_ALL	= 1
    } 	D3D12_DEPTH_WRITE_MASK;

typedef 
enum D3D12_STENCIL_OP
    {
        D3D12_STENCIL_OP_KEEP	= 1,
        D3D12_STENCIL_OP_ZERO	= 2,
        D3D12_STENCIL_OP_REPLACE	= 3,
        D3D12_STENCIL_OP_INCR_SAT	= 4,
        D3D12_STENCIL_OP_DECR_SAT	= 5,
        D3D12_STENCIL_OP_INVERT	= 6,
        D3D12_STENCIL_OP_INCR	= 7,
        D3D12_STENCIL_OP_DECR	= 8
    } 	D3D12_STENCIL_OP;

typedef struct D3D12_DEPTH_STENCILOP_DESC
    {
    D3D12_STENCIL_OP StencilFailOp;
    D3D12_STENCIL_OP StencilDepthFailOp;
    D3D12_STENCIL_OP StencilPassOp;
    D3D12_COMPARISON_FUNC StencilFunc;
    } 	D3D12_DEPTH_STENCILOP_DESC;

typedef struct D3D12_DEPTH_STENCIL_DESC
    {
    BOOL DepthEnable;
    D3D12_DEPTH_WRITE_MASK DepthWriteMask;
    D3D12_COMPARISON_FUNC DepthFunc;
    BOOL StencilEnable;
    UINT8 StencilReadMask;
    UINT8 StencilWriteMask;
    D3D12_DEPTH_STENCILOP_DESC FrontFace;
    D3D12_DEPTH_STENCILOP_DESC BackFace;
    } 	D3D12_DEPTH_STENCIL_DESC;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
struct CD3D12_DEPTH_STENCIL_DESC : public D3D12_DEPTH_STENCIL_DESC
{
    CD3D12_DEPTH_STENCIL_DESC()
    {}
    explicit CD3D12_DEPTH_STENCIL_DESC( const D3D12_DEPTH_STENCIL_DESC& o ) :
        D3D12_DEPTH_STENCIL_DESC( o )
    {}
    explicit CD3D12_DEPTH_STENCIL_DESC( CD3D12_DEFAULT )
    {
        DepthEnable = TRUE;
        DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        DepthFunc = D3D12_COMPARISON_LESS;
        StencilEnable = FALSE;
        StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
        StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
        const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp =
        { D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_ALWAYS };
        FrontFace = defaultStencilOp;
        BackFace = defaultStencilOp;
    }
    explicit CD3D12_DEPTH_STENCIL_DESC(
        BOOL depthEnable,
        D3D12_DEPTH_WRITE_MASK depthWriteMask,
        D3D12_COMPARISON_FUNC depthFunc,
        BOOL stencilEnable,
        UINT8 stencilReadMask,
        UINT8 stencilWriteMask,
        D3D12_STENCIL_OP frontStencilFailOp,
        D3D12_STENCIL_OP frontStencilDepthFailOp,
        D3D12_STENCIL_OP frontStencilPassOp,
        D3D12_COMPARISON_FUNC frontStencilFunc,
        D3D12_STENCIL_OP backStencilFailOp,
        D3D12_STENCIL_OP backStencilDepthFailOp,
        D3D12_STENCIL_OP backStencilPassOp,
        D3D12_COMPARISON_FUNC backStencilFunc )
    {
        DepthEnable = depthEnable;
        DepthWriteMask = depthWriteMask;
        DepthFunc = depthFunc;
        StencilEnable = stencilEnable;
        StencilReadMask = stencilReadMask;
        StencilWriteMask = stencilWriteMask;
        FrontFace.StencilFailOp = frontStencilFailOp;
        FrontFace.StencilDepthFailOp = frontStencilDepthFailOp;
        FrontFace.StencilPassOp = frontStencilPassOp;
        FrontFace.StencilFunc = frontStencilFunc;
        BackFace.StencilFailOp = backStencilFailOp;
        BackFace.StencilDepthFailOp = backStencilDepthFailOp;
        BackFace.StencilPassOp = backStencilPassOp;
        BackFace.StencilFunc = backStencilFunc;
    }
    ~CD3D12_DEPTH_STENCIL_DESC() {}
    operator const D3D12_DEPTH_STENCIL_DESC&() const { return *this; }
};
extern "C"{
#endif
typedef 
enum D3D12_BLEND
    {
        D3D12_BLEND_ZERO	= 1,
        D3D12_BLEND_ONE	= 2,
        D3D12_BLEND_SRC_COLOR	= 3,
        D3D12_BLEND_INV_SRC_COLOR	= 4,
        D3D12_BLEND_SRC_ALPHA	= 5,
        D3D12_BLEND_INV_SRC_ALPHA	= 6,
        D3D12_BLEND_DEST_ALPHA	= 7,
        D3D12_BLEND_INV_DEST_ALPHA	= 8,
        D3D12_BLEND_DEST_COLOR	= 9,
        D3D12_BLEND_INV_DEST_COLOR	= 10,
        D3D12_BLEND_SRC_ALPHA_SAT	= 11,
        D3D12_BLEND_BLEND_FACTOR	= 14,
        D3D12_BLEND_INV_BLEND_FACTOR	= 15,
        D3D12_BLEND_SRC1_COLOR	= 16,
        D3D12_BLEND_INV_SRC1_COLOR	= 17,
        D3D12_BLEND_SRC1_ALPHA	= 18,
        D3D12_BLEND_INV_SRC1_ALPHA	= 19
    } 	D3D12_BLEND;

typedef 
enum D3D12_BLEND_OP
    {
        D3D12_BLEND_OP_ADD	= 1,
        D3D12_BLEND_OP_SUBTRACT	= 2,
        D3D12_BLEND_OP_REV_SUBTRACT	= 3,
        D3D12_BLEND_OP_MIN	= 4,
        D3D12_BLEND_OP_MAX	= 5
    } 	D3D12_BLEND_OP;

typedef 
enum D3D12_COLOR_WRITE_ENABLE
    {
        D3D12_COLOR_WRITE_ENABLE_RED	= 1,
        D3D12_COLOR_WRITE_ENABLE_GREEN	= 2,
        D3D12_COLOR_WRITE_ENABLE_BLUE	= 4,
        D3D12_COLOR_WRITE_ENABLE_ALPHA	= 8,
        D3D12_COLOR_WRITE_ENABLE_ALL	= ( ( ( D3D12_COLOR_WRITE_ENABLE_RED | D3D12_COLOR_WRITE_ENABLE_GREEN )  | D3D12_COLOR_WRITE_ENABLE_BLUE )  | D3D12_COLOR_WRITE_ENABLE_ALPHA ) 
    } 	D3D12_COLOR_WRITE_ENABLE;

typedef 
enum D3D12_LOGIC_OP
    {
        D3D12_LOGIC_OP_CLEAR	= 0,
        D3D12_LOGIC_OP_SET	= ( D3D12_LOGIC_OP_CLEAR + 1 ) ,
        D3D12_LOGIC_OP_COPY	= ( D3D12_LOGIC_OP_SET + 1 ) ,
        D3D12_LOGIC_OP_COPY_INVERTED	= ( D3D12_LOGIC_OP_COPY + 1 ) ,
        D3D12_LOGIC_OP_NOOP	= ( D3D12_LOGIC_OP_COPY_INVERTED + 1 ) ,
        D3D12_LOGIC_OP_INVERT	= ( D3D12_LOGIC_OP_NOOP + 1 ) ,
        D3D12_LOGIC_OP_AND	= ( D3D12_LOGIC_OP_INVERT + 1 ) ,
        D3D12_LOGIC_OP_NAND	= ( D3D12_LOGIC_OP_AND + 1 ) ,
        D3D12_LOGIC_OP_OR	= ( D3D12_LOGIC_OP_NAND + 1 ) ,
        D3D12_LOGIC_OP_NOR	= ( D3D12_LOGIC_OP_OR + 1 ) ,
        D3D12_LOGIC_OP_XOR	= ( D3D12_LOGIC_OP_NOR + 1 ) ,
        D3D12_LOGIC_OP_EQUIV	= ( D3D12_LOGIC_OP_XOR + 1 ) ,
        D3D12_LOGIC_OP_AND_REVERSE	= ( D3D12_LOGIC_OP_EQUIV + 1 ) ,
        D3D12_LOGIC_OP_AND_INVERTED	= ( D3D12_LOGIC_OP_AND_REVERSE + 1 ) ,
        D3D12_LOGIC_OP_OR_REVERSE	= ( D3D12_LOGIC_OP_AND_INVERTED + 1 ) ,
        D3D12_LOGIC_OP_OR_INVERTED	= ( D3D12_LOGIC_OP_OR_REVERSE + 1 ) 
    } 	D3D12_LOGIC_OP;

typedef struct D3D12_RENDER_TARGET_BLEND_DESC
    {
    BOOL BlendEnable;
    BOOL LogicOpEnable;
    D3D12_BLEND SrcBlend;
    D3D12_BLEND DestBlend;
    D3D12_BLEND_OP BlendOp;
    D3D12_BLEND SrcBlendAlpha;
    D3D12_BLEND DestBlendAlpha;
    D3D12_BLEND_OP BlendOpAlpha;
    D3D12_LOGIC_OP LogicOp;
    UINT8 RenderTargetWriteMask;
    } 	D3D12_RENDER_TARGET_BLEND_DESC;

typedef struct D3D12_BLEND_DESC
    {
    BOOL AlphaToCoverageEnable;
    BOOL IndependentBlendEnable;
    D3D12_RENDER_TARGET_BLEND_DESC RenderTarget[ 8 ];
    } 	D3D12_BLEND_DESC;

/* Note, the array size for RenderTarget[] above is D3D11_SIMULTANEOUS_RENDERTARGET_COUNT. 
   IDL processing/generation of this header replaces the define; this comment is merely explaining what happened. */
#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
struct CD3D12_BLEND_DESC : public D3D12_BLEND_DESC
{
    CD3D12_BLEND_DESC()
    {}
    explicit CD3D12_BLEND_DESC( const D3D12_BLEND_DESC& o ) :
        D3D12_BLEND_DESC( o )
    {}
    explicit CD3D12_BLEND_DESC( CD3D12_DEFAULT )
    {
        AlphaToCoverageEnable = FALSE;
        IndependentBlendEnable = FALSE;
        const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
        {
            FALSE,FALSE,
            D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
            D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
            D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE_ALL,
        };
        for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
            RenderTarget[ i ] = defaultRenderTargetBlendDesc;
    }
    ~CD3D12_BLEND_DESC() {}
    operator const D3D12_BLEND_DESC&() const { return *this; }
};
extern "C"{
#endif
typedef 
enum D3D12_CONSERVATIVE_RASTERIZATION_MODE
    {
        D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF	= 0,
        D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON	= 1
    } 	D3D12_CONSERVATIVE_RASTERIZATION_MODE;

typedef struct D3D12_RASTERIZER_DESC
    {
    D3D12_FILL_MODE FillMode;
    D3D12_CULL_MODE CullMode;
    BOOL FrontCounterClockwise;
    INT DepthBias;
    FLOAT DepthBiasClamp;
    FLOAT SlopeScaledDepthBias;
    BOOL DepthClipEnable;
    BOOL MultisampleEnable;
    BOOL AntialiasedLineEnable;
    UINT ForcedSampleCount;
    D3D12_CONSERVATIVE_RASTERIZATION_MODE ConservativeRaster;
    } 	D3D12_RASTERIZER_DESC;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
struct CD3D12_RASTERIZER_DESC : public D3D12_RASTERIZER_DESC
{
    CD3D12_RASTERIZER_DESC()
    {}
    explicit CD3D12_RASTERIZER_DESC( const D3D12_RASTERIZER_DESC& o ) :
        D3D12_RASTERIZER_DESC( o )
    {}
    explicit CD3D12_RASTERIZER_DESC( CD3D12_DEFAULT )
    {
        FillMode = D3D12_FILL_SOLID;
        CullMode = D3D12_CULL_BACK;
        FrontCounterClockwise = FALSE;
        DepthBias = D3D11_DEFAULT_DEPTH_BIAS;
        DepthBiasClamp = D3D11_DEFAULT_DEPTH_BIAS_CLAMP;
        SlopeScaledDepthBias = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        DepthClipEnable = TRUE;
        MultisampleEnable = FALSE;
        AntialiasedLineEnable = FALSE;
        ForcedSampleCount = 0;
        ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
    }
    explicit CD3D12_RASTERIZER_DESC(
        D3D12_FILL_MODE fillMode,
        D3D12_CULL_MODE cullMode,
        BOOL frontCounterClockwise,
        INT depthBias,
        FLOAT depthBiasClamp,
        FLOAT slopeScaledDepthBias,
        BOOL depthClipEnable,
        BOOL multisampleEnable,
        BOOL antialiasedLineEnable, 
        UINT forcedSampleCount, 
        D3D12_CONSERVATIVE_RASTERIZATION_MODE conservativeRaster)
    {
        FillMode = fillMode;
        CullMode = cullMode;
        FrontCounterClockwise = frontCounterClockwise;
        DepthBias = depthBias;
        DepthBiasClamp = depthBiasClamp;
        SlopeScaledDepthBias = slopeScaledDepthBias;
        DepthClipEnable = depthClipEnable;
        MultisampleEnable = multisampleEnable;
        AntialiasedLineEnable = antialiasedLineEnable;
        ForcedSampleCount = forcedSampleCount;
        ConservativeRaster = conservativeRaster;
    }
    ~CD3D12_RASTERIZER_DESC() {}
    operator const D3D12_RASTERIZER_DESC&() const { return *this; }
};
extern "C"{
#endif


extern RPC_IF_HANDLE __MIDL_itf_d3d12_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d12_0000_0000_v0_0_s_ifspec;

#ifndef __ID3D12Object_INTERFACE_DEFINED__
#define __ID3D12Object_INTERFACE_DEFINED__

/* interface ID3D12Object */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12Object;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c4fec28f-7966-4e95-9f94-f431cb56c3b8")
    ID3D12Object : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetPrivateData( 
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _Inout_  UINT *pDataSize,
            /* [annotation] */ 
            _Out_writes_bytes_opt_( *pDataSize )  void *pData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetPrivateData( 
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_  UINT DataSize,
            /* [annotation] */ 
            _In_reads_bytes_opt_( DataSize )  const void *pData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface( 
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_opt_  const IUnknown *pData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetName( 
            /* [annotation] */ 
            _In_z_  LPCWSTR pName) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12ObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12Object * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12Object * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12Object * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetPrivateData )( 
            ID3D12Object * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _Inout_  UINT *pDataSize,
            /* [annotation] */ 
            _Out_writes_bytes_opt_( *pDataSize )  void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateData )( 
            ID3D12Object * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_  UINT DataSize,
            /* [annotation] */ 
            _In_reads_bytes_opt_( DataSize )  const void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )( 
            ID3D12Object * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_opt_  const IUnknown *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetName )( 
            ID3D12Object * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR pName);
        
        END_INTERFACE
    } ID3D12ObjectVtbl;

    interface ID3D12Object
    {
        CONST_VTBL struct ID3D12ObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12Object_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12Object_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12Object_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12Object_GetPrivateData(This,guid,pDataSize,pData)	\
    ( (This)->lpVtbl -> GetPrivateData(This,guid,pDataSize,pData) ) 

#define ID3D12Object_SetPrivateData(This,guid,DataSize,pData)	\
    ( (This)->lpVtbl -> SetPrivateData(This,guid,DataSize,pData) ) 

#define ID3D12Object_SetPrivateDataInterface(This,guid,pData)	\
    ( (This)->lpVtbl -> SetPrivateDataInterface(This,guid,pData) ) 

#define ID3D12Object_SetName(This,pName)	\
    ( (This)->lpVtbl -> SetName(This,pName) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ID3D12Object_INTERFACE_DEFINED__ */


#ifndef __ID3D12DeviceChild_INTERFACE_DEFINED__
#define __ID3D12DeviceChild_INTERFACE_DEFINED__

/* interface ID3D12DeviceChild */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12DeviceChild;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("905db94b-a00c-4140-9df5-2b64ca9ea357")
    ID3D12DeviceChild : public ID3D12Object
    {
    public:
        virtual void STDMETHODCALLTYPE GetDevice( 
            /* [annotation] */ 
            _Out_  ID3D12Device **ppDevice) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12DeviceChildVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12DeviceChild * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12DeviceChild * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12DeviceChild * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetPrivateData )( 
            ID3D12DeviceChild * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _Inout_  UINT *pDataSize,
            /* [annotation] */ 
            _Out_writes_bytes_opt_( *pDataSize )  void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateData )( 
            ID3D12DeviceChild * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_  UINT DataSize,
            /* [annotation] */ 
            _In_reads_bytes_opt_( DataSize )  const void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )( 
            ID3D12DeviceChild * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_opt_  const IUnknown *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetName )( 
            ID3D12DeviceChild * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR pName);
        
        void ( STDMETHODCALLTYPE *GetDevice )( 
            ID3D12DeviceChild * This,
            /* [annotation] */ 
            _Out_  ID3D12Device **ppDevice);
        
        END_INTERFACE
    } ID3D12DeviceChildVtbl;

    interface ID3D12DeviceChild
    {
        CONST_VTBL struct ID3D12DeviceChildVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12DeviceChild_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12DeviceChild_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12DeviceChild_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12DeviceChild_GetPrivateData(This,guid,pDataSize,pData)	\
    ( (This)->lpVtbl -> GetPrivateData(This,guid,pDataSize,pData) ) 

#define ID3D12DeviceChild_SetPrivateData(This,guid,DataSize,pData)	\
    ( (This)->lpVtbl -> SetPrivateData(This,guid,DataSize,pData) ) 

#define ID3D12DeviceChild_SetPrivateDataInterface(This,guid,pData)	\
    ( (This)->lpVtbl -> SetPrivateDataInterface(This,guid,pData) ) 

#define ID3D12DeviceChild_SetName(This,pName)	\
    ( (This)->lpVtbl -> SetName(This,pName) ) 


#define ID3D12DeviceChild_GetDevice(This,ppDevice)	\
    ( (This)->lpVtbl -> GetDevice(This,ppDevice) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ID3D12DeviceChild_INTERFACE_DEFINED__ */


#ifndef __ID3D12RootSignature_INTERFACE_DEFINED__
#define __ID3D12RootSignature_INTERFACE_DEFINED__

/* interface ID3D12RootSignature */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12RootSignature;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c54a6b66-72df-4ee8-8be5-a946a1429214")
    ID3D12RootSignature : public ID3D12DeviceChild
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12RootSignatureVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12RootSignature * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12RootSignature * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12RootSignature * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetPrivateData )( 
            ID3D12RootSignature * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _Inout_  UINT *pDataSize,
            /* [annotation] */ 
            _Out_writes_bytes_opt_( *pDataSize )  void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateData )( 
            ID3D12RootSignature * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_  UINT DataSize,
            /* [annotation] */ 
            _In_reads_bytes_opt_( DataSize )  const void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )( 
            ID3D12RootSignature * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_opt_  const IUnknown *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetName )( 
            ID3D12RootSignature * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR pName);
        
        void ( STDMETHODCALLTYPE *GetDevice )( 
            ID3D12RootSignature * This,
            /* [annotation] */ 
            _Out_  ID3D12Device **ppDevice);
        
        END_INTERFACE
    } ID3D12RootSignatureVtbl;

    interface ID3D12RootSignature
    {
        CONST_VTBL struct ID3D12RootSignatureVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12RootSignature_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12RootSignature_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12RootSignature_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12RootSignature_GetPrivateData(This,guid,pDataSize,pData)	\
    ( (This)->lpVtbl -> GetPrivateData(This,guid,pDataSize,pData) ) 

#define ID3D12RootSignature_SetPrivateData(This,guid,DataSize,pData)	\
    ( (This)->lpVtbl -> SetPrivateData(This,guid,DataSize,pData) ) 

#define ID3D12RootSignature_SetPrivateDataInterface(This,guid,pData)	\
    ( (This)->lpVtbl -> SetPrivateDataInterface(This,guid,pData) ) 

#define ID3D12RootSignature_SetName(This,pName)	\
    ( (This)->lpVtbl -> SetName(This,pName) ) 


#define ID3D12RootSignature_GetDevice(This,ppDevice)	\
    ( (This)->lpVtbl -> GetDevice(This,ppDevice) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ID3D12RootSignature_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_d3d12_0000_0001 */
/* [local] */ 

typedef struct D3D12_SHADER_BYTECODE
    {
    /* [annotation] */ 
    __ecount(BytecodeLength)  const BYTE *pShaderBytecode;
    SIZE_T BytecodeLength;
    } 	D3D12_SHADER_BYTECODE;

typedef struct D3D12_STREAM_OUTPUT_DESC
    {
    /* [annotation] */ 
    __ecount(NumEntries)  const D3D12_SO_DECLARATION_ENTRY *pSODeclaration;
    UINT NumEntries;
    /* [annotation] */ 
    __ecount(NumStrides)  const UINT *pBufferStrides;
    UINT NumStrides;
    UINT RasterizedStream;
    } 	D3D12_STREAM_OUTPUT_DESC;

typedef struct D3D12_INPUT_LAYOUT_DESC
    {
    /* [annotation] */ 
    __ecount(NumElements)  const D3D12_INPUT_ELEMENT_DESC *pInputElementDescs;
    UINT NumElements;
    } 	D3D12_INPUT_LAYOUT_DESC;

typedef struct D3D12_GRAPHICS_PIPELINE_STATE_DESC
    {
    ID3D12RootSignature *pRootSignature;
    D3D12_SHADER_BYTECODE VS;
    D3D12_SHADER_BYTECODE PS;
    D3D12_SHADER_BYTECODE DS;
    D3D12_SHADER_BYTECODE HS;
    D3D12_SHADER_BYTECODE GS;
    D3D12_STREAM_OUTPUT_DESC StreamOutput;
    D3D12_BLEND_DESC BlendState;
    UINT SampleMask;
    D3D12_RASTERIZER_DESC RasterizerState;
    D3D12_DEPTH_STENCIL_DESC DepthStencilState;
    D3D12_INPUT_LAYOUT_DESC InputLayout;
    D3D12_PRIMITIVE_TOPOLOGY_TYPE PrimitiveTopologyType;
    UINT NumRenderTargets;
    DXGI_FORMAT RTVFormats[ 8 ];
    DXGI_FORMAT DSVFormat;
    DXGI_SAMPLE_DESC SampleDesc;
    } 	D3D12_GRAPHICS_PIPELINE_STATE_DESC;

typedef struct D3D12_COMPUTE_PIPELINE_STATE_DESC
    {
    ID3D12RootSignature *pRootSignature;
    D3D12_SHADER_BYTECODE CS;
    } 	D3D12_COMPUTE_PIPELINE_STATE_DESC;

typedef 
enum D3D12_FEATURE
    {
        D3D12_FEATURE_D3D12_OPTIONS	= 0,
        D3D12_FEATURE_ARCHITECTURE	= ( D3D12_FEATURE_D3D12_OPTIONS + 1 ) ,
        D3D12_FEATURE_FEATURE_LEVELS	= ( D3D12_FEATURE_ARCHITECTURE + 1 ) ,
        D3D12_FEATURE_FORMAT_SUPPORT	= ( D3D12_FEATURE_FEATURE_LEVELS + 1 ) ,
        D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS	= ( D3D12_FEATURE_FORMAT_SUPPORT + 1 ) 
    } 	D3D12_FEATURE;

typedef 
enum D3D12_SHADER_MIN_PRECISION_SUPPORT
    {
        D3D12_SHADER_MIN_PRECISION_NONE	= 0,
        D3D12_SHADER_MIN_PRECISION_10_BIT	= 0x1,
        D3D12_SHADER_MIN_PRECISION_16_BIT	= 0x2
    } 	D3D12_SHADER_MIN_PRECISION_SUPPORT;

DEFINE_ENUM_FLAG_OPERATORS( D3D12_SHADER_MIN_PRECISION_SUPPORT );
typedef 
enum D3D12_TILED_RESOURCES_TIER
    {
        D3D12_TILED_RESOURCES_NOT_SUPPORTED	= 0,
        D3D12_TILED_RESOURCES_TIER_1	= 1,
        D3D12_TILED_RESOURCES_TIER_2	= 2
    } 	D3D12_TILED_RESOURCES_TIER;

typedef 
enum D3D12_RESOURCE_BINDING_TIER
    {
        D3D12_RESOURCE_BINDING_TIER_1	= 1,
        D3D12_RESOURCE_BINDING_TIER_2	= 2,
        D3D12_RESOURCE_BINDING_TIER_3	= 3
    } 	D3D12_RESOURCE_BINDING_TIER;

typedef 
enum D3D12_CONSERVATIVE_RASTERIZATION_TIER
    {
        D3D12_CONSERVATIVE_RASTERIZATION_NOT_SUPPORTED	= 0,
        D3D12_CONSERVATIVE_RASTERIZATION_TIER_1	= 1,
        D3D12_CONSERVATIVE_RASTERIZATION_TIER_2	= 2,
        D3D12_CONSERVATIVE_RASTERIZATION_TIER_3	= 3
    } 	D3D12_CONSERVATIVE_RASTERIZATION_TIER;

typedef 
enum D3D12_FORMAT_SUPPORT1
    {
        D3D12_FORMAT_SUPPORT1_NONE	= 0,
        D3D12_FORMAT_SUPPORT1_BUFFER	= 0x1,
        D3D12_FORMAT_SUPPORT1_IA_VERTEX_BUFFER	= 0x2,
        D3D12_FORMAT_SUPPORT1_IA_INDEX_BUFFER	= 0x4,
        D3D12_FORMAT_SUPPORT1_SO_BUFFER	= 0x8,
        D3D12_FORMAT_SUPPORT1_TEXTURE1D	= 0x10,
        D3D12_FORMAT_SUPPORT1_TEXTURE2D	= 0x20,
        D3D12_FORMAT_SUPPORT1_TEXTURE3D	= 0x40,
        D3D12_FORMAT_SUPPORT1_TEXTURECUBE	= 0x80,
        D3D12_FORMAT_SUPPORT1_SHADER_LOAD	= 0x100,
        D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE	= 0x200,
        D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE_COMPARISON	= 0x400,
        D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE_MONO_TEXT	= 0x800,
        D3D12_FORMAT_SUPPORT1_MIP	= 0x1000,
        D3D12_FORMAT_SUPPORT1_RENDER_TARGET	= 0x4000,
        D3D12_FORMAT_SUPPORT1_BLENDABLE	= 0x8000,
        D3D12_FORMAT_SUPPORT1_DEPTH_STENCIL	= 0x10000,
        D3D12_FORMAT_SUPPORT1_MULTISAMPLE_RESOLVE	= 0x40000,
        D3D12_FORMAT_SUPPORT1_DISPLAY	= 0x80000,
        D3D12_FORMAT_SUPPORT1_CAST_WITHIN_BIT_LAYOUT	= 0x100000,
        D3D12_FORMAT_SUPPORT1_MULTISAMPLE_RENDERTARGET	= 0x200000,
        D3D12_FORMAT_SUPPORT1_MULTISAMPLE_LOAD	= 0x400000,
        D3D12_FORMAT_SUPPORT1_SHADER_GATHER	= 0x800000,
        D3D12_FORMAT_SUPPORT1_BACK_BUFFER_CAST	= 0x1000000,
        D3D12_FORMAT_SUPPORT1_TYPED_UNORDERED_ACCESS_VIEW	= 0x2000000,
        D3D12_FORMAT_SUPPORT1_SHADER_GATHER_COMPARISON	= 0x4000000,
        D3D12_FORMAT_SUPPORT1_DECODER_OUTPUT	= 0x8000000,
        D3D12_FORMAT_SUPPORT1_VIDEO_PROCESSOR_OUTPUT	= 0x10000000,
        D3D12_FORMAT_SUPPORT1_VIDEO_PROCESSOR_INPUT	= 0x20000000,
        D3D12_FORMAT_SUPPORT1_VIDEO_ENCODER	= 0x40000000
    } 	D3D12_FORMAT_SUPPORT1;

DEFINE_ENUM_FLAG_OPERATORS( D3D12_FORMAT_SUPPORT1 );
typedef 
enum D3D12_FORMAT_SUPPORT2
    {
        D3D12_FORMAT_SUPPORT2_NONE	= 0,
        D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_ADD	= 0x1,
        D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_BITWISE_OPS	= 0x2,
        D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_COMPARE_STORE_OR_COMPARE_EXCHANGE	= 0x4,
        D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_EXCHANGE	= 0x8,
        D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_SIGNED_MIN_OR_MAX	= 0x10,
        D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_UNSIGNED_MIN_OR_MAX	= 0x20,
        D3D12_FORMAT_SUPPORT2_UAV_TYPED_LOAD	= 0x40,
        D3D12_FORMAT_SUPPORT2_UAV_TYPED_STORE	= 0x80,
        D3D12_FORMAT_SUPPORT2_OUTPUT_MERGER_LOGIC_OP	= 0x100,
        D3D12_FORMAT_SUPPORT2_TILED	= 0x200,
        D3D12_FORMAT_SUPPORT2_SHAREABLE	= 0x400
    } 	D3D12_FORMAT_SUPPORT2;

DEFINE_ENUM_FLAG_OPERATORS( D3D12_FORMAT_SUPPORT2 );
typedef 
enum D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG
    {
        D3D12_MULTISAMPLE_QUALITY_LEVELS_NONE	= 0,
        D3D12_MULTISAMPLE_QUALITY_LEVELS_TILED_RESOURCE	= 0x1
    } 	D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG;

DEFINE_ENUM_FLAG_OPERATORS( D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG );
typedef struct D3D12_FEATURE_DATA_D3D12_OPTIONS
    {
    /* [annotation] */ 
    _Out_  BOOL DoublePrecisionFloatShaderOps;
    /* [annotation] */ 
    _Out_  BOOL OutputMergerLogicOp;
    /* [annotation] */ 
    _Out_  D3D12_SHADER_MIN_PRECISION_SUPPORT MinPrecisionSupport;
    /* [annotation] */ 
    _Out_  D3D12_TILED_RESOURCES_TIER TiledResourcesTier;
    /* [annotation] */ 
    _Out_  D3D12_RESOURCE_BINDING_TIER ResourceBindingTier;
    /* [annotation] */ 
    _Out_  BOOL PSSpecifiedStencilRefSupported;
    /* [annotation] */ 
    _Out_  BOOL TypedUAVLoadAdditionalFormats;
    /* [annotation] */ 
    _Out_  BOOL ROVsSupported;
    /* [annotation] */ 
    _Out_  D3D12_CONSERVATIVE_RASTERIZATION_TIER ConservativeRasterizationTier;
    /* [annotation] */ 
    _Out_  UINT MaxGPUVirtualAddressBitsPerResource;
    } 	D3D12_FEATURE_DATA_D3D12_OPTIONS;

typedef struct D3D12_FEATURE_DATA_ARCHITECTURE
    {
    /* [annotation] */ 
    _Out_  BOOL TileBasedRenderer;
    /* [annotation] */ 
    _Out_  BOOL UMA;
    /* [annotation] */ 
    _Out_  BOOL CacheCoherentUMA;
    } 	D3D12_FEATURE_DATA_ARCHITECTURE;

typedef struct D3D12_FEATURE_DATA_FEATURE_LEVELS
    {
    /* [annotation] */ 
    _In_  UINT NumFeatureLevels;
    /* [annotation] */ 
    _In_reads_(NumFeatureLevels)  const D3D_FEATURE_LEVEL *pFeatureLevelsRequested;
    /* [annotation] */ 
    _Out_  D3D_FEATURE_LEVEL MaxSupportedFeatureLevel;
    } 	D3D12_FEATURE_DATA_FEATURE_LEVELS;

typedef struct D3D12_FEATURE_DATA_FORMAT_SUPPORT
    {
    /* [annotation] */ 
    _In_  DXGI_FORMAT Format;
    /* [annotation] */ 
    _Out_  D3D12_FORMAT_SUPPORT1 Support1;
    /* [annotation] */ 
    _Out_  D3D12_FORMAT_SUPPORT2 Support2;
    } 	D3D12_FEATURE_DATA_FORMAT_SUPPORT;

typedef struct D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS
    {
    /* [annotation] */ 
    _In_  DXGI_FORMAT Format;
    /* [annotation] */ 
    _In_  UINT SampleCount;
    /* [annotation] */ 
    _In_  D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG Flags;
    /* [annotation] */ 
    _Out_  UINT NumQualityLevels;
    } 	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS;

typedef struct D3D12_RESOURCE_ALLOCATION_INFO
    {
    UINT64 Size;
    UINT64 Alignment;
    } 	D3D12_RESOURCE_ALLOCATION_INFO;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
struct CD3D12_RESOURCE_ALLOCATION_INFO : public D3D12_RESOURCE_ALLOCATION_INFO
{
    CD3D12_RESOURCE_ALLOCATION_INFO()
    {}
    CD3D12_RESOURCE_ALLOCATION_INFO( 
        UINT64 size, 
        UINT64 alignment )
    {
        Size = size;
        Alignment = alignment;
    }
    operator const D3D12_RESOURCE_ALLOCATION_INFO&() const { return *this; }
};
extern "C"{
#endif
typedef 
enum D3D12_HEAP_TYPE
    {
        D3D12_HEAP_TYPE_DEFAULT	= 1,
        D3D12_HEAP_TYPE_UPLOAD	= 2,
        D3D12_HEAP_TYPE_READBACK	= 3,
        D3D12_HEAP_TYPE_CUSTOM	= 4
    } 	D3D12_HEAP_TYPE;

typedef 
enum D3D12_CPU_PAGE_PROPERTIES
    {
        D3D12_CPU_PAGE_UNKNOWN	= 0,
        D3D12_CPU_PAGE_NOT_AVAILABLE	= 1,
        D3D12_CPU_PAGE_WRITE_COMBINE	= 2,
        D3D12_CPU_PAGE_WRITE_BACK	= 3
    } 	D3D12_CPU_PAGE_PROPERTIES;

typedef 
enum D3D12_MEMORY_POOL
    {
        D3D12_MEMORY_POOL_UNKNOWN	= 0,
        D3D12_MEMORY_POOL_L0	= 1,
        D3D12_MEMORY_POOL_L1	= 2
    } 	D3D12_MEMORY_POOL;

typedef struct D3D12_HEAP_PROPERTIES
    {
    D3D12_HEAP_TYPE Type;
    D3D12_CPU_PAGE_PROPERTIES CPUPageProperties;
    D3D12_MEMORY_POOL MemoryPoolPreference;
    } 	D3D12_HEAP_PROPERTIES;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
struct CD3D12_HEAP_PROPERTIES : public D3D12_HEAP_PROPERTIES
{
    CD3D12_HEAP_PROPERTIES()
    {}
    CD3D12_HEAP_PROPERTIES( 
        D3D12_CPU_PAGE_PROPERTIES cpuPageProperties, 
        D3D12_MEMORY_POOL memoryPoolPreference )
    {
        Type = D3D12_HEAP_TYPE_CUSTOM;
        CPUPageProperties = cpuPageProperties;
        MemoryPoolPreference = memoryPoolPreference;
    }
    explicit CD3D12_HEAP_PROPERTIES( D3D12_HEAP_TYPE type )
    {
        Type = type;
        CPUPageProperties = D3D12_CPU_PAGE_UNKNOWN;
        MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    }
#if defined(__d3d11_h__) || defined(__d3d11p_h__)
    static D3D12_HEAP_TYPE ConvertD3D11Usage( D3D11_USAGE Usage, UINT CPUAccessFlags )
    {
        switch (Usage)
        {
        case D3D11_USAGE_DEFAULT:
            return (CPUAccessFlags ? D3D12_HEAP_TYPE_CUSTOM : D3D12_HEAP_TYPE_DEFAULT);
        case D3D11_USAGE_IMMUTABLE:
            return (CPUAccessFlags ? D3D12_HEAP_TYPE( 0 ) : D3D12_HEAP_TYPE_DEFAULT);
        case D3D11_USAGE_DYNAMIC:
            return (CPUAccessFlags == D3D11_CPU_ACCESS_WRITE ? D3D12_HEAP_TYPE_UPLOAD : D3D12_HEAP_TYPE( 0 ));
        case D3D11_USAGE_STAGING:
            return (CPUAccessFlags == D3D11_CPU_ACCESS_WRITE ? D3D12_HEAP_TYPE_UPLOAD :
                (CPUAccessFlags ? D3D12_HEAP_TYPE_READBACK : D3D12_HEAP_TYPE( 0 )));
        default: return D3D12_HEAP_TYPE( 0 );
        }
    }
    explicit CD3D12_HEAP_PROPERTIES( const D3D11_BUFFER_DESC& o )
    {
        *this = CD3D12_HEAP_PROPERTIES( ConvertD3D11Usage( o.Usage, o.CPUAccessFlags ) );
    }
    explicit CD3D12_HEAP_PROPERTIES( const D3D11_TEXTURE1D_DESC& o )
    {
        *this = CD3D12_HEAP_PROPERTIES( ConvertD3D11Usage( o.Usage, o.CPUAccessFlags ) );
    }
    explicit CD3D12_HEAP_PROPERTIES( const D3D11_TEXTURE2D_DESC& o )
    {
        *this = CD3D12_HEAP_PROPERTIES( ConvertD3D11Usage( o.Usage, o.CPUAccessFlags ) );
    }
    explicit CD3D12_HEAP_PROPERTIES( const D3D11_TEXTURE3D_DESC& o )
    {
        *this = CD3D12_HEAP_PROPERTIES( ConvertD3D11Usage( o.Usage, o.CPUAccessFlags ) );
    }
#endif
    operator const D3D12_HEAP_PROPERTIES&() const { return *this; }
    bool IsCPUAccessible() const
    {
        return Type == D3D12_HEAP_TYPE_UPLOAD || Type == D3D12_HEAP_TYPE_READBACK || (Type == D3D12_HEAP_TYPE_CUSTOM &&
            (CPUPageProperties == D3D12_CPU_PAGE_WRITE_COMBINE || CPUPageProperties == D3D12_CPU_PAGE_WRITE_BACK));
    }
};
inline bool operator==( const D3D12_HEAP_PROPERTIES& l, const D3D12_HEAP_PROPERTIES& r )
{
    return l.Type == r.Type && l.CPUPageProperties == r.CPUPageProperties && 
        l.MemoryPoolPreference == r.MemoryPoolPreference;
}
inline bool operator!=( const D3D12_HEAP_PROPERTIES& l, const D3D12_HEAP_PROPERTIES& r )
{ return !( l == r ); }
extern "C"{
#endif
typedef 
enum D3D12_HEAP_MISC_FLAG
    {
        D3D12_HEAP_MISC_NONE	= 0,
        D3D12_HEAP_MISC_SHARED	= 0x1,
        D3D12_HEAP_MISC_NO_TEXTURES	= 0x2,
        D3D12_HEAP_MISC_NO_BUFFERS	= 0x4
    } 	D3D12_HEAP_MISC_FLAG;

DEFINE_ENUM_FLAG_OPERATORS( D3D12_HEAP_MISC_FLAG );
typedef struct D3D12_HEAP_DESC
    {
    UINT64 Size;
    D3D12_HEAP_PROPERTIES Properties;
    UINT64 Alignment;
    D3D12_HEAP_MISC_FLAG MiscFlags;
    } 	D3D12_HEAP_DESC;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
struct CD3D12_HEAP_DESC : public D3D12_HEAP_DESC
{
    CD3D12_HEAP_DESC()
    {}
    CD3D12_HEAP_DESC( 
        UINT64 size, 
        D3D12_HEAP_PROPERTIES properties, 
        UINT64 alignment = 0, 
        D3D12_HEAP_MISC_FLAG miscFlags = D3D12_HEAP_MISC_NONE )
    {
        Size = size;
        Properties = properties;
        Alignment = alignment;
        MiscFlags = miscFlags;
    }
    CD3D12_HEAP_DESC( 
        UINT64 size, 
        D3D12_HEAP_TYPE type, 
        UINT64 alignment = 0, 
        D3D12_HEAP_MISC_FLAG miscFlags = D3D12_HEAP_MISC_NONE )
    {
        Size = size;
        Properties = CD3D12_HEAP_PROPERTIES( type );
        Alignment = alignment;
        MiscFlags = miscFlags;
    }
    CD3D12_HEAP_DESC( 
        UINT64 size, 
        D3D12_CPU_PAGE_PROPERTIES cpuPageProperties, 
        D3D12_MEMORY_POOL memoryPoolPreference, 
        UINT64 alignment = 0, 
        D3D12_HEAP_MISC_FLAG miscFlags = D3D12_HEAP_MISC_NONE )
    {
        Size = size;
        Properties = CD3D12_HEAP_PROPERTIES( cpuPageProperties, memoryPoolPreference );
        Alignment = alignment;
        MiscFlags = miscFlags;
    }
    CD3D12_HEAP_DESC( 
        const D3D12_RESOURCE_ALLOCATION_INFO& resAllocInfo,
        D3D12_HEAP_PROPERTIES properties, 
        D3D12_HEAP_MISC_FLAG miscFlags = D3D12_HEAP_MISC_NONE )
    {
        Size = resAllocInfo.Size;
        Properties = properties;
        Alignment = resAllocInfo.Alignment;
        MiscFlags = miscFlags;
    }
    CD3D12_HEAP_DESC( 
        const D3D12_RESOURCE_ALLOCATION_INFO& resAllocInfo,
        D3D12_HEAP_TYPE type, 
        D3D12_HEAP_MISC_FLAG miscFlags = D3D12_HEAP_MISC_NONE )
    {
        Size = resAllocInfo.Size;
        Properties = CD3D12_HEAP_PROPERTIES( type );
        Alignment = resAllocInfo.Alignment;
        MiscFlags = miscFlags;
    }
    CD3D12_HEAP_DESC( 
        const D3D12_RESOURCE_ALLOCATION_INFO& resAllocInfo,
        D3D12_CPU_PAGE_PROPERTIES cpuPageProperties, 
        D3D12_MEMORY_POOL memoryPoolPreference, 
        D3D12_HEAP_MISC_FLAG miscFlags = D3D12_HEAP_MISC_NONE )
    {
        Size = resAllocInfo.Size;
        Properties = CD3D12_HEAP_PROPERTIES( cpuPageProperties, memoryPoolPreference );
        Alignment = resAllocInfo.Alignment;
        MiscFlags = miscFlags;
    }
#if defined(__d3d11_h__) || defined(__d3d11p_h__)
    static D3D12_HEAP_MISC_FLAG ConvertD3D11MiscFlags( UINT miscFlags )
    {
        D3D12_HEAP_MISC_FLAG MiscFlags = D3D12_HEAP_MISC_NONE;
        if ((miscFlags & (D3D11_RESOURCE_MISC_SHARED_NTHANDLE | D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX)) == 
            D3D11_RESOURCE_MISC_SHARED_NTHANDLE)
        {
            MiscFlags |= D3D12_HEAP_MISC_SHARED;
        }
        return MiscFlags;
    }
    explicit CD3D12_HEAP_DESC( const D3D11_BUFFER_DESC& o )
    {
        *this = CD3D12_HEAP_DESC( 0, CD3D12_HEAP_PROPERTIES( o ), 0, ConvertD3D11MiscFlags( o.MiscFlags ) );
    }
    explicit CD3D12_HEAP_DESC( const D3D11_TEXTURE1D_DESC& o )
    {
        *this = CD3D12_HEAP_DESC( 0, CD3D12_HEAP_PROPERTIES( o ), 0, ConvertD3D11MiscFlags( o.MiscFlags ) );
    }
    explicit CD3D12_HEAP_DESC( const D3D11_TEXTURE2D_DESC& o )
    {
        *this = CD3D12_HEAP_DESC( 0, CD3D12_HEAP_PROPERTIES( o ), 0, ConvertD3D11MiscFlags( o.MiscFlags ) );
    }
    explicit CD3D12_HEAP_DESC( const D3D11_TEXTURE3D_DESC& o )
    {
        *this = CD3D12_HEAP_DESC( 0, CD3D12_HEAP_PROPERTIES( o ), 0, ConvertD3D11MiscFlags( o.MiscFlags ) );
    }
#endif
    operator const D3D12_HEAP_DESC&() const { return *this; }
    bool IsCPUAccessible() const
    { return static_cast< const CD3D12_HEAP_PROPERTIES* >( &Properties )->IsCPUAccessible(); }
};
extern "C"{
#endif
typedef 
enum D3D12_RESOURCE_DIMENSION
    {
        D3D12_RESOURCE_DIMENSION_UNKNOWN	= 0,
        D3D12_RESOURCE_DIMENSION_BUFFER	= 1,
        D3D12_RESOURCE_DIMENSION_TEXTURE_1D	= 2,
        D3D12_RESOURCE_DIMENSION_TEXTURE_2D	= 3,
        D3D12_RESOURCE_DIMENSION_TEXTURE_3D	= 4
    } 	D3D12_RESOURCE_DIMENSION;

typedef 
enum D3D12_TEXTURE_LAYOUT
    {
        D3D12_TEXTURE_LAYOUT_UNKNOWN	= 0,
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR	= 1,
        D3D12_TEXTURE_LAYOUT_64KB_TILE_UNDEFINED_SWIZZLE	= 2
    } 	D3D12_TEXTURE_LAYOUT;

typedef 
enum D3D12_RESOURCE_MISC_FLAG
    {
        D3D12_RESOURCE_MISC_NONE	= 0,
        D3D12_RESOURCE_MISC_RENDER_TARGET	= 0x1,
        D3D12_RESOURCE_MISC_DEPTH_STENCIL	= 0x2,
        D3D12_RESOURCE_MISC_NO_SHADER_RESOURCE	= 0x4,
        D3D12_RESOURCE_MISC_NO_UNORDERED_ACCESS	= 0x8,
        D3D12_RESOURCE_MISC_NO_STREAM_OUTPUT	= 0x10,
        D3D12_RESOURCE_MISC_NO_TILE_MAPPING	= 0x20
    } 	D3D12_RESOURCE_MISC_FLAG;

DEFINE_ENUM_FLAG_OPERATORS( D3D12_RESOURCE_MISC_FLAG );
typedef struct D3D12_RESOURCE_DESC
    {
    D3D12_RESOURCE_DIMENSION Dimension;
    UINT64 Alignment;
    UINT64 Width;
    UINT Height;
    UINT16 DepthOrArraySize;
    UINT16 MipLevels;
    DXGI_FORMAT Format;
    DXGI_SAMPLE_DESC SampleDesc;
    D3D12_TEXTURE_LAYOUT Layout;
    D3D12_RESOURCE_MISC_FLAG MiscFlags;
    } 	D3D12_RESOURCE_DESC;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
inline bool D3D12IsLayoutOpaque( D3D12_TEXTURE_LAYOUT Layout )
{ return Layout == D3D12_TEXTURE_LAYOUT_UNKNOWN || Layout == D3D12_TEXTURE_LAYOUT_64KB_TILE_UNDEFINED_SWIZZLE; }
struct CD3D12_RESOURCE_DESC : public D3D12_RESOURCE_DESC
{
    CD3D12_RESOURCE_DESC()
    {}
    explicit CD3D12_RESOURCE_DESC( const D3D12_RESOURCE_DESC& o ) :
        D3D12_RESOURCE_DESC( o )
    {}
    CD3D12_RESOURCE_DESC( 
        D3D12_RESOURCE_DIMENSION dimension,
        UINT64 alignment,
        UINT64 width,
        UINT height,
        UINT16 depthOrArraySize,
        UINT16 mipLevels,
        DXGI_FORMAT format,
        UINT sampleCount,
        UINT sampleQuality,
        D3D12_TEXTURE_LAYOUT layout,
        D3D12_RESOURCE_MISC_FLAG miscFlags )
    {
        Dimension = dimension;
        Alignment = alignment;
        Width = width;
        Height = height;
        DepthOrArraySize = depthOrArraySize;
        MipLevels = mipLevels;
        Format = format;
        SampleDesc.Count = sampleCount;
        SampleDesc.Quality = sampleQuality;
        Layout = layout;
        MiscFlags = miscFlags;
    }
    static CD3D12_RESOURCE_DESC Buffer( 
        const D3D12_RESOURCE_ALLOCATION_INFO& resAllocInfo,
        D3D12_RESOURCE_MISC_FLAG miscFlags = D3D12_RESOURCE_MISC_NONE )
    {
        return CD3D12_RESOURCE_DESC( D3D12_RESOURCE_DIMENSION_BUFFER, resAllocInfo.Alignment, resAllocInfo.Size, 
            1, 1, 1, DXGI_FORMAT_UNKNOWN, 1, 0, D3D12_TEXTURE_LAYOUT_ROW_MAJOR, miscFlags );
    }
    static CD3D12_RESOURCE_DESC Buffer( 
        UINT64 width,
        D3D12_RESOURCE_MISC_FLAG miscFlags = D3D12_RESOURCE_MISC_NONE,
        UINT64 alignment = 0 )
    {
        return CD3D12_RESOURCE_DESC( D3D12_RESOURCE_DIMENSION_BUFFER, alignment, width, 1, 1, 1, 
            DXGI_FORMAT_UNKNOWN, 1, 0, D3D12_TEXTURE_LAYOUT_ROW_MAJOR, miscFlags );
    }
    static CD3D12_RESOURCE_DESC Tex1D( 
        DXGI_FORMAT format,
        UINT64 width,
        UINT16 arraySize = 1,
        UINT16 mipLevels = 0,
        D3D12_RESOURCE_MISC_FLAG miscFlags = D3D12_RESOURCE_MISC_NO_STREAM_OUTPUT,
        D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
        UINT64 alignment = 0 )
    {
        return CD3D12_RESOURCE_DESC( D3D12_RESOURCE_DIMENSION_TEXTURE_1D, alignment, width, 1, arraySize, 
            mipLevels, format, 1, 0, layout, miscFlags );
    }
    static CD3D12_RESOURCE_DESC Tex2D( 
        DXGI_FORMAT format,
        UINT64 width,
        UINT height,
        UINT16 arraySize = 1,
        UINT16 mipLevels = 0,
        UINT sampleCount = 1,
        UINT sampleQuality = 0,
        D3D12_RESOURCE_MISC_FLAG miscFlags = D3D12_RESOURCE_MISC_NO_STREAM_OUTPUT,
        D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
        UINT64 alignment = 0 )
    {
        return CD3D12_RESOURCE_DESC( D3D12_RESOURCE_DIMENSION_TEXTURE_2D, alignment, width, height, arraySize, 
            mipLevels, format, sampleCount, sampleQuality, layout, miscFlags );
    }
    static CD3D12_RESOURCE_DESC Tex3D( 
        DXGI_FORMAT format,
        UINT64 width,
        UINT height,
        UINT16 depth,
        UINT16 mipLevels = 0,
        D3D12_RESOURCE_MISC_FLAG miscFlags = D3D12_RESOURCE_MISC_NO_STREAM_OUTPUT,
        D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
        UINT64 alignment = 0 )
    {
        return CD3D12_RESOURCE_DESC( D3D12_RESOURCE_DIMENSION_TEXTURE_3D, alignment, width, height, depth, 
            mipLevels, format, 1, 0, layout, miscFlags );
    }
    UINT16 Depth() const
    { return (Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE_3D ? DepthOrArraySize : 1); }
    UINT16 ArraySize() const
    { return (Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE_3D ? DepthOrArraySize : 1); }
    UINT Subresources() const
    { return MipLevels * ArraySize(); }
#if defined(__d3d11_h__) || defined(__d3d11p_h__)
    void ConvertD3D11Flags( UINT bindFlags )
    {
        if (!(bindFlags & D3D11_BIND_SHADER_RESOURCE)) MiscFlags |= D3D12_RESOURCE_MISC_NO_SHADER_RESOURCE;
        if (!(bindFlags & D3D11_BIND_STREAM_OUTPUT)) MiscFlags |= D3D12_RESOURCE_MISC_NO_STREAM_OUTPUT;
        if (bindFlags & D3D11_BIND_RENDER_TARGET) MiscFlags |= D3D12_RESOURCE_MISC_RENDER_TARGET;
        if (bindFlags & D3D11_BIND_DEPTH_STENCIL) MiscFlags |= D3D12_RESOURCE_MISC_DEPTH_STENCIL;
        if (!(bindFlags & D3D11_BIND_UNORDERED_ACCESS)) MiscFlags |= D3D12_RESOURCE_MISC_NO_UNORDERED_ACCESS;
    }
    explicit CD3D12_RESOURCE_DESC( const D3D11_BUFFER_DESC& o )
    {
        *this = Buffer( o.ByteWidth, D3D12_RESOURCE_MISC_NONE );
        ConvertD3D11Flags( o.BindFlags );
    }
    explicit CD3D12_RESOURCE_DESC( const D3D11_TEXTURE1D_DESC& o )
    {
        *this = Tex1D( o.Format, o.Width, UINT16( o.ArraySize ), UINT16( o.MipLevels ) );
        ConvertD3D11Flags( o.BindFlags );
        if (o.MiscFlags & D3D11_RESOURCE_MISC_TILED) Layout = D3D12_TEXTURE_LAYOUT_64KB_TILE_UNDEFINED_SWIZZLE;
    }
    explicit CD3D12_RESOURCE_DESC( const D3D11_TEXTURE2D_DESC& o )
    {
        *this = Tex2D( o.Format, o.Width, o.Height, UINT16( o.ArraySize ), UINT16( o.MipLevels ), 
            o.SampleDesc.Count, o.SampleDesc.Quality );
        ConvertD3D11Flags( o.BindFlags );
        if (o.MiscFlags & D3D11_RESOURCE_MISC_TILED) Layout = D3D12_TEXTURE_LAYOUT_64KB_TILE_UNDEFINED_SWIZZLE;
    }
    explicit CD3D12_RESOURCE_DESC( const D3D11_TEXTURE3D_DESC& o )
    {
        *this = Tex3D( o.Format, o.Width, o.Height, UINT16( o.Depth ), UINT16( o.MipLevels ) );
        ConvertD3D11Flags( o.BindFlags );
        if (o.MiscFlags & D3D11_RESOURCE_MISC_TILED) Layout = D3D12_TEXTURE_LAYOUT_64KB_TILE_UNDEFINED_SWIZZLE;
    }
#endif
    operator const D3D12_RESOURCE_DESC&() const { return *this; }
};
extern "C"{
#endif
typedef struct D3D11_RESOURCE_DESC
    {
    D3D11_RESOURCE_DIMENSION Dimension;
    UINT Width;
    UINT Height;
    UINT Depth;
    UINT MipLevels;
    UINT ArraySize;
    DXGI_FORMAT Format;
    DXGI_SAMPLE_DESC SampleDesc;
    D3D11_USAGE Usage;
    UINT BindFlags;
    UINT CPUAccessFlags;
    UINT MiscFlags;
    UINT StructureByteStride;
    } 	D3D11_RESOURCE_DESC;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
struct CD3D11_RESOURCE_DESC : public D3D11_RESOURCE_DESC
{
    CD3D11_RESOURCE_DESC()
    {}
    explicit CD3D11_RESOURCE_DESC( const D3D11_RESOURCE_DESC& o ) :
        D3D11_RESOURCE_DESC( o )
    {}
    void ConvertD3D12Info( D3D12_TEXTURE_LAYOUT layout, D3D12_RESOURCE_MISC_FLAG miscFlags )
    {
        if (miscFlags & D3D12_RESOURCE_MISC_RENDER_TARGET) BindFlags |= D3D11_BIND_RENDER_TARGET;
        if (miscFlags & D3D12_RESOURCE_MISC_DEPTH_STENCIL) BindFlags |= D3D11_BIND_DEPTH_STENCIL;
        if (!(miscFlags & D3D12_RESOURCE_MISC_NO_SHADER_RESOURCE)) BindFlags |= D3D11_BIND_SHADER_RESOURCE;
        if (!(miscFlags & D3D12_RESOURCE_MISC_NO_UNORDERED_ACCESS)) BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
        if (!(miscFlags & D3D12_RESOURCE_MISC_NO_STREAM_OUTPUT)) BindFlags |= D3D11_BIND_STREAM_OUTPUT;
        if (layout == D3D12_TEXTURE_LAYOUT_64KB_TILE_UNDEFINED_SWIZZLE) MiscFlags |= D3D11_RESOURCE_MISC_TILED;
    }
    CD3D11_RESOURCE_DESC( const D3D12_RESOURCE_DESC& o )
    {
        switch (o.Dimension)
        {
        case D3D12_RESOURCE_DIMENSION_BUFFER: Dimension = D3D11_RESOURCE_DIMENSION_BUFFER; break;
        case D3D12_RESOURCE_DIMENSION_TEXTURE_1D: Dimension = D3D11_RESOURCE_DIMENSION_TEXTURE1D; break;
        case D3D12_RESOURCE_DIMENSION_TEXTURE_2D: Dimension = D3D11_RESOURCE_DIMENSION_TEXTURE2D; break;
        case D3D12_RESOURCE_DIMENSION_TEXTURE_3D: Dimension = D3D11_RESOURCE_DIMENSION_TEXTURE3D; break;
        default: Dimension = D3D11_RESOURCE_DIMENSION_UNKNOWN; break;
        }
        Width = UINT( o.Width );
        Height = o.Height;
        Depth = (Dimension == D3D11_RESOURCE_DIMENSION_TEXTURE3D ? o.DepthOrArraySize : 1);
        MipLevels = o.MipLevels;
        ArraySize = (Dimension == D3D11_RESOURCE_DIMENSION_TEXTURE3D ? 1 : o.DepthOrArraySize);
        Format = o.Format;
        SampleDesc = o.SampleDesc;
        Usage = D3D11_USAGE_DEFAULT;
        BindFlags = 0;
        CPUAccessFlags = 0;
        MiscFlags = 0;
        StructureByteStride = 0;
        ConvertD3D12Info( o.Layout, o.MiscFlags );
    }
    CD3D11_RESOURCE_DESC( const D3D11_BUFFER_DESC& o )
    {
        Dimension = D3D11_RESOURCE_DIMENSION_BUFFER;
        Width = o.ByteWidth;
        Height = 1;
        Depth = 1;
        MipLevels = 1;
        ArraySize = 1;
        Format = DXGI_FORMAT_UNKNOWN;
        SampleDesc.Count = 1;
        SampleDesc.Quality = 0;
        Usage = o.Usage;
        BindFlags = o.BindFlags;
        CPUAccessFlags = o.CPUAccessFlags;
        MiscFlags = o.MiscFlags;
        StructureByteStride = o.StructureByteStride;
    }
    CD3D11_RESOURCE_DESC( const CD3D11_BUFFER_DESC& o )
    {
        Dimension = D3D11_RESOURCE_DIMENSION_BUFFER;
        Width = o.ByteWidth;
        Height = 1;
        Depth = 1;
        MipLevels = 1;
        ArraySize = 1;
        Format = DXGI_FORMAT_UNKNOWN;
        SampleDesc.Count = 1;
        SampleDesc.Quality = 0;
        Usage = o.Usage;
        BindFlags = o.BindFlags;
        CPUAccessFlags = o.CPUAccessFlags;
        MiscFlags = o.MiscFlags;
        StructureByteStride = o.StructureByteStride;
    }
    CD3D11_RESOURCE_DESC( const D3D11_TEXTURE1D_DESC& o )
    {
        Dimension = D3D11_RESOURCE_DIMENSION_TEXTURE1D;
        Width = o.Width;
        Height = 1;
        Depth = 1;
        MipLevels = o.MipLevels;
        ArraySize = o.ArraySize;
        Format = o.Format;
        SampleDesc.Count = 1;
        SampleDesc.Quality = 0;
        Usage = o.Usage;
        BindFlags = o.BindFlags;
        CPUAccessFlags = o.CPUAccessFlags;
        MiscFlags = o.MiscFlags;
        StructureByteStride = 0;
    }
    CD3D11_RESOURCE_DESC( const CD3D11_TEXTURE1D_DESC& o )
    {
        Dimension = D3D11_RESOURCE_DIMENSION_TEXTURE1D;
        Width = o.Width;
        Height = 1;
        Depth = 1;
        MipLevels = o.MipLevels;
        ArraySize = o.ArraySize;
        Format = o.Format;
        SampleDesc.Count = 1;
        SampleDesc.Quality = 0;
        Usage = o.Usage;
        BindFlags = o.BindFlags;
        CPUAccessFlags = o.CPUAccessFlags;
        MiscFlags = o.MiscFlags;
        StructureByteStride = 0;
    }
    CD3D11_RESOURCE_DESC( const D3D11_TEXTURE2D_DESC& o )
    {
        Dimension = D3D11_RESOURCE_DIMENSION_TEXTURE2D;
        Width = o.Width;
        Height = o.Height;
        Depth = 1;
        MipLevels = o.MipLevels;
        ArraySize = o.ArraySize;
        Format = o.Format;
        SampleDesc = o.SampleDesc;
        Usage = o.Usage;
        BindFlags = o.BindFlags;
        CPUAccessFlags = o.CPUAccessFlags;
        MiscFlags = o.MiscFlags;
        StructureByteStride = 0;
    }
    CD3D11_RESOURCE_DESC( const CD3D11_TEXTURE2D_DESC& o )
    {
        Dimension = D3D11_RESOURCE_DIMENSION_TEXTURE2D;
        Width = o.Width;
        Height = o.Height;
        Depth = 1;
        MipLevels = o.MipLevels;
        ArraySize = o.ArraySize;
        Format = o.Format;
        SampleDesc = o.SampleDesc;
        Usage = o.Usage;
        BindFlags = o.BindFlags;
        CPUAccessFlags = o.CPUAccessFlags;
        MiscFlags = o.MiscFlags;
        StructureByteStride = 0;
    }
    CD3D11_RESOURCE_DESC( const D3D11_TEXTURE3D_DESC& o )
    {
        Dimension = D3D11_RESOURCE_DIMENSION_TEXTURE3D;
        Width = o.Width;
        Height = o.Height;
        Depth = o.Depth;
        MipLevels = o.MipLevels;
        ArraySize = 1;
        Format = o.Format;
        SampleDesc.Count = 1;
        SampleDesc.Quality = 0;
        Usage = o.Usage;
        BindFlags = o.BindFlags;
        CPUAccessFlags = o.CPUAccessFlags;
        MiscFlags = o.MiscFlags;
        StructureByteStride = 0;
    }
    CD3D11_RESOURCE_DESC( const CD3D11_TEXTURE3D_DESC& o )
    {
        Dimension = D3D11_RESOURCE_DIMENSION_TEXTURE3D;
        Width = o.Width;
        Height = o.Height;
        Depth = o.Depth;
        MipLevels = o.MipLevels;
        ArraySize = 1;
        Format = o.Format;
        SampleDesc.Count = 1;
        SampleDesc.Quality = 0;
        Usage = o.Usage;
        BindFlags = o.BindFlags;
        CPUAccessFlags = o.CPUAccessFlags;
        MiscFlags = o.MiscFlags;
        StructureByteStride = 0;
    }
    ~CD3D11_RESOURCE_DESC() {}
    operator const D3D11_RESOURCE_DESC&() const { return *this; }
    operator CD3D12_RESOURCE_DESC() const
    {
        CD3D12_RESOURCE_DESC RetDesc
            ( D3D12_RESOURCE_DIMENSION_BUFFER
            , 0
            , Width
            , Height
            , UINT16( Dimension == D3D11_RESOURCE_DIMENSION_TEXTURE3D ? Depth : ArraySize )
            , UINT16( MipLevels )
            , Format
            , SampleDesc.Count
            , SampleDesc.Quality
            , D3D12_TEXTURE_LAYOUT_UNKNOWN
            , D3D12_RESOURCE_MISC_NONE
            );
        switch (Dimension)
        {
        case D3D11_RESOURCE_DIMENSION_TEXTURE1D: RetDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE_1D; break;
        case D3D11_RESOURCE_DIMENSION_TEXTURE2D: RetDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE_2D; break;
        case D3D11_RESOURCE_DIMENSION_TEXTURE3D: RetDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE_3D; break;
        default: break;
        }
        RetDesc.ConvertD3D11Flags( BindFlags );
        return RetDesc;
    }
    operator D3D12_RESOURCE_DESC() const { return D3D12_RESOURCE_DESC( this->operator CD3D12_RESOURCE_DESC() ); }
};
extern "C"{
#endif
typedef 
enum D3D12_MAP
    {
        D3D12_MAP_READ	= 1,
        D3D12_MAP_WRITE	= 2,
        D3D12_MAP_READ_WRITE	= 3
    } 	D3D12_MAP;

typedef struct D3D12_RANGE
    {
    SIZE_T Begin;
    SIZE_T End;
    } 	D3D12_RANGE;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
struct CD3D12_RANGE : public D3D12_RANGE
{
    CD3D12_RANGE()
    {}
    CD3D12_RANGE( 
        SIZE_T begin, 
        SIZE_T end )
    {
        Begin = begin;
        End = end;
    }
    operator const D3D12_RANGE&() const { return *this; }
};
extern "C"{
#endif
typedef struct D3D12_SUBRESOURCE_INFO
    {
    UINT64 Offset;
    UINT RowPitch;
    UINT DepthPitch;
    } 	D3D12_SUBRESOURCE_INFO;

typedef struct D3D12_TILED_RESOURCE_COORDINATE
    {
    UINT X;
    UINT Y;
    UINT Z;
    UINT Subresource;
    } 	D3D12_TILED_RESOURCE_COORDINATE;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
struct CD3D12_TILED_RESOURCE_COORDINATE : public D3D12_TILED_RESOURCE_COORDINATE
{
    CD3D12_TILED_RESOURCE_COORDINATE()
    {}
    CD3D12_TILED_RESOURCE_COORDINATE( 
        UINT x, 
        UINT y, 
        UINT z, 
        UINT subresource ) 
    {
        X = x;
        Y = y;
        Z = z;
        Subresource = subresource;
    }
    operator const D3D12_TILED_RESOURCE_COORDINATE&() const { return *this; }
};
extern "C"{
#endif
typedef struct D3D12_TILE_REGION_SIZE
    {
    UINT NumTiles;
    BOOL UseBox;
    UINT Width;
    UINT16 Height;
    UINT16 Depth;
    } 	D3D12_TILE_REGION_SIZE;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
struct CD3D12_TILE_REGION_SIZE : public D3D12_TILE_REGION_SIZE
{
    CD3D12_TILE_REGION_SIZE()
    {}
    CD3D12_TILE_REGION_SIZE( 
        UINT numTiles, 
        BOOL useBox, 
        UINT width, 
        UINT16 height, 
        UINT16 depth ) 
    {
        NumTiles = numTiles;
        UseBox = useBox;
        Width = width;
        Height = height;
        Depth = depth;
    }
    operator const D3D12_TILE_REGION_SIZE&() const { return *this; }
};
extern "C"{
#endif
typedef 
enum D3D12_TILE_RANGE_FLAG
    {
        D3D12_TILE_RANGE_NONE	= 0,
        D3D12_TILE_RANGE_NULL	= 0x1,
        D3D12_TILE_RANGE_SKIP	= 0x2,
        D3D12_TILE_RANGE_REUSE_SINGLE_TILE	= 0x4
    } 	D3D12_TILE_RANGE_FLAG;

DEFINE_ENUM_FLAG_OPERATORS( D3D12_TILE_RANGE_FLAG );
typedef struct D3D12_SUBRESOURCE_TILING
    {
    UINT WidthInTiles;
    UINT16 HeightInTiles;
    UINT16 DepthInTiles;
    UINT StartTileIndexInOverallResource;
    } 	D3D12_SUBRESOURCE_TILING;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
struct CD3D12_SUBRESOURCE_TILING : public D3D12_SUBRESOURCE_TILING
{
    CD3D12_SUBRESOURCE_TILING()
    {}
    CD3D12_SUBRESOURCE_TILING( 
        UINT widthInTiles, 
        UINT16 heightInTiles, 
        UINT16 depthInTiles, 
        UINT startTileIndexInOverallResource ) 
    {
        WidthInTiles = widthInTiles;
        HeightInTiles = heightInTiles;
        DepthInTiles = depthInTiles;
        StartTileIndexInOverallResource = startTileIndexInOverallResource;
    }
    operator const D3D12_SUBRESOURCE_TILING&() const { return *this; }
};
extern "C"{
#endif
#define	D3D12_PACKED_TILE	( 0xffffffff )

typedef struct D3D12_TILE_SHAPE
    {
    UINT WidthInTexels;
    UINT HeightInTexels;
    UINT DepthInTexels;
    } 	D3D12_TILE_SHAPE;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
struct CD3D12_TILE_SHAPE : public D3D12_TILE_SHAPE
{
    CD3D12_TILE_SHAPE()
    {}
    CD3D12_TILE_SHAPE( 
        UINT widthInTexels, 
        UINT heightInTexels, 
        UINT depthInTexels ) 
    {
        WidthInTexels = widthInTexels;
        HeightInTexels = heightInTexels;
        DepthInTexels = depthInTexels;
    }
    operator const D3D12_TILE_SHAPE&() const { return *this; }
};
extern "C"{
#endif
typedef struct D3D12_PACKED_MIP_DESC
    {
    UINT8 NumStandardMips;
    UINT8 NumPackedMips;
    UINT NumTilesForPackedMips;
    UINT StartTileIndexInOverallResource;
    } 	D3D12_PACKED_MIP_DESC;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
struct CD3D12_PACKED_MIP_DESC : public D3D12_PACKED_MIP_DESC
{
    CD3D12_PACKED_MIP_DESC()
    {}
    CD3D12_PACKED_MIP_DESC( 
        UINT8 numStandardMips, 
        UINT8 numPackedMips, 
        UINT numTilesForPackedMips, 
        UINT startTileIndexInOverallResource ) 
    {
        NumStandardMips = numStandardMips;
        NumPackedMips = numPackedMips;
        NumTilesForPackedMips = numTilesForPackedMips;
        StartTileIndexInOverallResource = startTileIndexInOverallResource;
    }
    operator const D3D12_PACKED_MIP_DESC&() const { return *this; }
};
extern "C"{
#endif
typedef 
enum D3D12_TILE_MAPPING_FLAG
    {
        D3D12_TILE_MAPPING_NONE	= 0,
        D3D12_TILE_MAPPING_NO_HAZARD	= 0x1
    } 	D3D12_TILE_MAPPING_FLAG;

DEFINE_ENUM_FLAG_OPERATORS( D3D12_TILE_MAPPING_FLAG );
typedef 
enum D3D12_TILE_COPY_FLAG
    {
        D3D12_TILE_COPY_NONE	= 0,
        D3D12_TILE_COPY_NO_HAZARD	= 0x1,
        D3D12_TILE_COPY_LINEAR_BUFFER_TO_SWIZZLED_TILED_RESOURCE	= 0x2,
        D3D12_TILE_COPY_SWIZZLED_TILED_RESOURCE_TO_LINEAR_BUFFER	= 0x4
    } 	D3D12_TILE_COPY_FLAG;

DEFINE_ENUM_FLAG_OPERATORS( D3D12_TILE_COPY_FLAG );
typedef 
enum D3D12_RESOURCE_USAGE
    {
        D3D12_RESOURCE_USAGE_NO_ACCESS	= 0x1,
        D3D12_RESOURCE_USAGE_DEFAULT_READ	= 0x2,
        D3D12_RESOURCE_USAGE_RENDER_TARGET	= 0x4,
        D3D12_RESOURCE_USAGE_UNORDERED_ACCESS	= 0x8,
        D3D12_RESOURCE_USAGE_DEPTH	= 0x10,
        D3D12_RESOURCE_USAGE_NON_PIXEL_SHADER_RESOURCE	= 0x20,
        D3D12_RESOURCE_USAGE_PIXEL_SHADER_RESOURCE	= 0x40,
        D3D12_RESOURCE_USAGE_STREAM_OUT	= 0x80,
        D3D12_RESOURCE_USAGE_INDIRECT_ARGUMENT	= 0x100,
        D3D12_RESOURCE_USAGE_COPY_DEST	= 0x400,
        D3D12_RESOURCE_USAGE_COPY_SOURCE	= 0x800,
        D3D12_RESOURCE_USAGE_RESOLVE_DEST	= 0x1000,
        D3D12_RESOURCE_USAGE_RESOLVE_SOURCE	= 0x2000,
        D3D12_RESOURCE_USAGE_GENERIC_READ	= ( ( ( ( D3D12_RESOURCE_USAGE_DEFAULT_READ | D3D12_RESOURCE_USAGE_COPY_SOURCE )  | D3D12_RESOURCE_USAGE_NON_PIXEL_SHADER_RESOURCE )  | D3D12_RESOURCE_USAGE_PIXEL_SHADER_RESOURCE )  | D3D12_RESOURCE_USAGE_INDIRECT_ARGUMENT ) ,
        D3D12_RESOURCE_USAGE_PRESENT	= D3D12_RESOURCE_USAGE_COPY_SOURCE,
        D3D12_RESOURCE_USAGE_INITIAL	= 0
    } 	D3D12_RESOURCE_USAGE;

typedef 
enum D3D12_RESOURCE_BARRIER_TYPE
    {
        D3D12_RESOURCE_BARRIER_TYPE_TRANSITION	= 0,
        D3D12_RESOURCE_BARRIER_TYPE_ALIASING	= ( D3D12_RESOURCE_BARRIER_TYPE_TRANSITION + 1 ) ,
        D3D12_RESOURCE_BARRIER_TYPE_UAV	= ( D3D12_RESOURCE_BARRIER_TYPE_ALIASING + 1 ) 
    } 	D3D12_RESOURCE_BARRIER_TYPE;

#define	D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES	( 0xffffffff )


typedef struct D3D12_RESOURCE_TRANSITION_BARRIER_DESC
    {
    ID3D12Resource *pResource;
    UINT Subresource;
    UINT StateBefore;
    UINT StateAfter;
    } 	D3D12_RESOURCE_TRANSITION_BARRIER_DESC;

typedef struct D3D12_RESOURCE_ALIASING_BARRIER_DESC
    {
    ID3D12Resource *pResourceBefore;
    ID3D12Resource *pResourceAfter;
    } 	D3D12_RESOURCE_ALIASING_BARRIER_DESC;

typedef struct D3D12_RESOURCE_UAV_BARRIER_DESC
    {
    ID3D12Resource *pResource;
    } 	D3D12_RESOURCE_UAV_BARRIER_DESC;

typedef struct D3D12_RESOURCE_BARRIER_DESC
    {
    D3D12_RESOURCE_BARRIER_TYPE Type;
    union 
        {
        D3D12_RESOURCE_TRANSITION_BARRIER_DESC Transition;
        D3D12_RESOURCE_ALIASING_BARRIER_DESC Aliasing;
        D3D12_RESOURCE_UAV_BARRIER_DESC UAV;
        } 	;
    } 	D3D12_RESOURCE_BARRIER_DESC;

#define	D3D12_CONSTANT_BUFFER_OFFSET_ALIGNMENT	( 256 )

#define	D3D12_TEXTURE_OFFSET_ALIGNMENT	( 512 )

#define	D3D12_TEXTURE_PITCH_ALIGNMENT	( 128 )

#define	D3D12_RESOURCE_MAPPING_ALIGNMENT	( 65536 )

#define	D3D12_MSAA_RESOURCE_MAPPING_ALIGNMENT	( 4194304 )

typedef struct D3D12_PITCHED_SUBRESOURCE_DESC
    {
    DXGI_FORMAT Format;
    UINT Width;
    UINT Height;
    UINT Depth;
    UINT RowPitch;
    } 	D3D12_PITCHED_SUBRESOURCE_DESC;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
struct CD3D12_PITCHED_SUBRESOURCE_DESC : public D3D12_PITCHED_SUBRESOURCE_DESC
{
    CD3D12_PITCHED_SUBRESOURCE_DESC()
    {}
    CD3D12_PITCHED_SUBRESOURCE_DESC( 
        DXGI_FORMAT format, 
        UINT width, 
        UINT height, 
        UINT depth, 
        UINT rowPitch ) 
    {
        Format = format;
        Width = width;
        Height = height;
        Depth = depth;
        RowPitch = rowPitch;
    }
    explicit CD3D12_PITCHED_SUBRESOURCE_DESC( 
        const D3D12_RESOURCE_DESC& resDesc, 
        UINT rowPitch ) 
    {
        Format = resDesc.Format;
        Width = UINT( resDesc.Width );
        Height = resDesc.Height;
        Depth = (resDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE_3D ? resDesc.DepthOrArraySize : 1);
        RowPitch = rowPitch;
    }
    operator const D3D12_PITCHED_SUBRESOURCE_DESC&() const { return *this; }
};
extern "C"{
#endif
typedef struct D3D12_PLACED_PITCHED_SUBRESOURCE_DESC
    {
    UINT64 Offset;
    D3D12_PITCHED_SUBRESOURCE_DESC Placement;
    } 	D3D12_PLACED_PITCHED_SUBRESOURCE_DESC;

typedef 
enum D3D12_SUBRESOURCE_VIEW_TYPE
    {
        D3D12_SUBRESOURCE_VIEW_SELECT_SUBRESOURCE	= 0,
        D3D12_SUBRESOURCE_VIEW_PLACED_PITCHED_SUBRESOURCE	= 1
    } 	D3D12_SUBRESOURCE_VIEW_TYPE;

typedef struct D3D12_SELECT_SUBRESOURCE
    {
    UINT Subresource;
    } 	D3D12_SELECT_SUBRESOURCE;

typedef 
enum D3D12_COPY_FLAGS
    {
        D3D12_COPY_NONE	= 0,
        D3D12_COPY_NO_OVERWRITE	= 0x1,
        D3D12_COPY_DISCARD	= 0x2
    } 	D3D12_COPY_FLAGS;

DEFINE_ENUM_FLAG_OPERATORS( D3D12_COPY_FLAGS );
typedef struct D3D12_INDEX_BUFFER_VIEW_DESC
    {
    DXGI_FORMAT Format;
    UINT64 OffsetInBytes;
    UINT64 SizeInBytes;
    } 	D3D12_INDEX_BUFFER_VIEW_DESC;

typedef struct D3D12_STREAM_OUTPUT_VIEW_DESC
    {
    UINT64 OffsetInBytes;
    UINT64 SizeInBytes;
    ID3D11Query *pSOCurrentOffset;
    } 	D3D12_STREAM_OUTPUT_VIEW_DESC;

typedef struct D3D12_VERTEX_BUFFER_VIEW_DESC
    {
    UINT StrideInBytes;
    UINT64 OffsetInBytes;
    UINT64 SizeInBytes;
    } 	D3D12_VERTEX_BUFFER_VIEW_DESC;

typedef 
enum D3D12_BUFFER_SRV_FLAG
    {
        D3D12_BUFFER_SRV_FLAG_RAW	= 0x1
    } 	D3D12_BUFFER_SRV_FLAG;

typedef struct D3D12_BUFFER_SRV
    {
    UINT64 FirstElement;
    UINT NumElements;
    UINT StructureByteStride;
    UINT Flags;
    } 	D3D12_BUFFER_SRV;

typedef struct D3D12_TEX1D_SRV
    {
    UINT MostDetailedMip;
    UINT MipLevels;
    FLOAT ResourceMinLODClamp;
    } 	D3D12_TEX1D_SRV;

typedef struct D3D12_TEX1D_ARRAY_SRV
    {
    UINT MostDetailedMip;
    UINT MipLevels;
    UINT FirstArraySlice;
    UINT ArraySize;
    FLOAT ResourceMinLODClamp;
    } 	D3D12_TEX1D_ARRAY_SRV;

typedef struct D3D12_TEX2D_SRV
    {
    UINT MostDetailedMip;
    UINT MipLevels;
    FLOAT ResourceMinLODClamp;
    } 	D3D12_TEX2D_SRV;

typedef struct D3D12_TEX2D_ARRAY_SRV
    {
    UINT MostDetailedMip;
    UINT MipLevels;
    UINT FirstArraySlice;
    UINT ArraySize;
    FLOAT ResourceMinLODClamp;
    } 	D3D12_TEX2D_ARRAY_SRV;

typedef struct D3D12_TEX3D_SRV
    {
    UINT MostDetailedMip;
    UINT MipLevels;
    FLOAT ResourceMinLODClamp;
    } 	D3D12_TEX3D_SRV;

typedef struct D3D12_TEXCUBE_SRV
    {
    UINT MostDetailedMip;
    UINT MipLevels;
    FLOAT ResourceMinLODClamp;
    } 	D3D12_TEXCUBE_SRV;

typedef struct D3D12_TEXCUBE_ARRAY_SRV
    {
    UINT MostDetailedMip;
    UINT MipLevels;
    UINT First2DArrayFace;
    UINT NumCubes;
    FLOAT ResourceMinLODClamp;
    } 	D3D12_TEXCUBE_ARRAY_SRV;

typedef struct D3D12_TEX2DMS_SRV
    {
    UINT UnusedField_NothingToDefine;
    } 	D3D12_TEX2DMS_SRV;

typedef struct D3D12_TEX2DMS_ARRAY_SRV
    {
    UINT FirstArraySlice;
    UINT ArraySize;
    } 	D3D12_TEX2DMS_ARRAY_SRV;

typedef 
enum D3D12_SRV_DIMENSION
    {
        D3D12_SRV_DIMENSION_UNKNOWN	= 0,
        D3D12_SRV_DIMENSION_BUFFER	= 1,
        D3D12_SRV_DIMENSION_TEXTURE1D	= 2,
        D3D12_SRV_DIMENSION_TEXTURE1DARRAY	= 3,
        D3D12_SRV_DIMENSION_TEXTURE2D	= 4,
        D3D12_SRV_DIMENSION_TEXTURE2DARRAY	= 5,
        D3D12_SRV_DIMENSION_TEXTURE2DMS	= 6,
        D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY	= 7,
        D3D12_SRV_DIMENSION_TEXTURE3D	= 8,
        D3D12_SRV_DIMENSION_TEXTURECUBE	= 9,
        D3D12_SRV_DIMENSION_TEXTURECUBEARRAY	= 10
    } 	D3D12_SRV_DIMENSION;

typedef struct D3D12_SHADER_RESOURCE_VIEW_DESC
    {
    DXGI_FORMAT Format;
    D3D12_SRV_DIMENSION ViewDimension;
    union 
        {
        D3D12_BUFFER_SRV Buffer;
        D3D12_TEX1D_SRV Texture1D;
        D3D12_TEX1D_ARRAY_SRV Texture1DArray;
        D3D12_TEX2D_SRV Texture2D;
        D3D12_TEX2D_ARRAY_SRV Texture2DArray;
        D3D12_TEX2DMS_SRV Texture2DMS;
        D3D12_TEX2DMS_ARRAY_SRV Texture2DMSArray;
        D3D12_TEX3D_SRV Texture3D;
        D3D12_TEXCUBE_SRV TextureCube;
        D3D12_TEXCUBE_ARRAY_SRV TextureCubeArray;
        } 	;
    } 	D3D12_SHADER_RESOURCE_VIEW_DESC;

typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC
    {
    UINT64 OffsetInBytes;
    UINT SizeInBytes;
    } 	D3D12_CONSTANT_BUFFER_VIEW_DESC;

typedef 
enum D3D12_FILTER
    {
        D3D12_FILTER_MIN_MAG_MIP_POINT	= 0,
        D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR	= 0x1,
        D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT	= 0x4,
        D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR	= 0x5,
        D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT	= 0x10,
        D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR	= 0x11,
        D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT	= 0x14,
        D3D12_FILTER_MIN_MAG_MIP_LINEAR	= 0x15,
        D3D12_FILTER_ANISOTROPIC	= 0x55,
        D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT	= 0x80,
        D3D12_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR	= 0x81,
        D3D12_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT	= 0x84,
        D3D12_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR	= 0x85,
        D3D12_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT	= 0x90,
        D3D12_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR	= 0x91,
        D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT	= 0x94,
        D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR	= 0x95,
        D3D12_FILTER_COMPARISON_ANISOTROPIC	= 0xd5,
        D3D12_FILTER_MINIMUM_MIN_MAG_MIP_POINT	= 0x100,
        D3D12_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR	= 0x101,
        D3D12_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT	= 0x104,
        D3D12_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR	= 0x105,
        D3D12_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT	= 0x110,
        D3D12_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR	= 0x111,
        D3D12_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT	= 0x114,
        D3D12_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR	= 0x115,
        D3D12_FILTER_MINIMUM_ANISOTROPIC	= 0x155,
        D3D12_FILTER_MAXIMUM_MIN_MAG_MIP_POINT	= 0x180,
        D3D12_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR	= 0x181,
        D3D12_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT	= 0x184,
        D3D12_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR	= 0x185,
        D3D12_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT	= 0x190,
        D3D12_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR	= 0x191,
        D3D12_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT	= 0x194,
        D3D12_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR	= 0x195,
        D3D12_FILTER_MAXIMUM_ANISOTROPIC	= 0x1d5
    } 	D3D12_FILTER;

typedef 
enum D3D12_FILTER_TYPE
    {
        D3D12_FILTER_TYPE_POINT	= 0,
        D3D12_FILTER_TYPE_LINEAR	= 1
    } 	D3D12_FILTER_TYPE;

typedef 
enum D3D12_FILTER_REDUCTION_TYPE
    {
        D3D12_FILTER_REDUCTION_TYPE_STANDARD	= 0,
        D3D12_FILTER_REDUCTION_TYPE_COMPARISON	= 1,
        D3D12_FILTER_REDUCTION_TYPE_MINIMUM	= 2,
        D3D12_FILTER_REDUCTION_TYPE_MAXIMUM	= 3
    } 	D3D12_FILTER_REDUCTION_TYPE;

#define	D3D12_FILTER_REDUCTION_TYPE_MASK	( 0x3 )

#define	D3D12_FILTER_REDUCTION_TYPE_SHIFT	( 7 )

#define	D3D12_FILTER_TYPE_MASK	( 0x3 )

#define	D3D12_MIN_FILTER_SHIFT	( 4 )

#define	D3D12_MAG_FILTER_SHIFT	( 2 )

#define	D3D12_MIP_FILTER_SHIFT	( 0 )

#define D3D12_ENCODE_BASIC_FILTER( min, mag, mip, reduction )                                                     \
                                   ( ( D3D12_FILTER ) (                                                           \
                                   ( ( ( min ) & D3D12_FILTER_TYPE_MASK ) << D3D12_MIN_FILTER_SHIFT ) |           \
                                   ( ( ( mag ) & D3D12_FILTER_TYPE_MASK ) << D3D12_MAG_FILTER_SHIFT ) |           \
                                   ( ( ( mip ) & D3D12_FILTER_TYPE_MASK ) << D3D12_MIP_FILTER_SHIFT ) |           \
                                   ( ( ( reduction ) & D3D12_FILTER_REDUCTION_TYPE_MASK ) << D3D12_FILTER_REDUCTION_TYPE_SHIFT ) ) ) 
#define D3D12_ENCODE_ANISOTROPIC_FILTER( reduction )                                                  \
                                         ( ( D3D12_FILTER ) (                                         \
                                         D3D12_ANISOTROPIC_FILTERING_BIT |                            \
                                         D3D12_ENCODE_BASIC_FILTER( D3D12_FILTER_TYPE_LINEAR,         \
                                                                    D3D12_FILTER_TYPE_LINEAR,         \
                                                                    D3D12_FILTER_TYPE_LINEAR,         \
                                                                    reduction ) ) )                     
#define D3D12_DECODE_MIN_FILTER( D3D12Filter )                                                              \
                                 ( ( D3D12_FILTER_TYPE )                                                    \
                                 ( ( ( D3D12Filter ) >> D3D12_MIN_FILTER_SHIFT ) & D3D12_FILTER_TYPE_MASK ) ) 
#define D3D12_DECODE_MAG_FILTER( D3D12Filter )                                                              \
                                 ( ( D3D12_FILTER_TYPE )                                                    \
                                 ( ( ( D3D12Filter ) >> D3D12_MAG_FILTER_SHIFT ) & D3D12_FILTER_TYPE_MASK ) ) 
#define D3D12_DECODE_MIP_FILTER( D3D12Filter )                                                              \
                                 ( ( D3D12_FILTER_TYPE )                                                    \
                                 ( ( ( D3D12Filter ) >> D3D12_MIP_FILTER_SHIFT ) & D3D12_FILTER_TYPE_MASK ) ) 
#define D3D12_DECODE_FILTER_REDUCTION( D3D12Filter )                                                        \
                                 ( ( D3D12_FILTER_REDUCTION_TYPE )                                                      \
                                 ( ( ( D3D12Filter ) >> D3D12_FILTER_REDUCTION_TYPE_SHIFT ) & D3D12_FILTER_REDUCTION_TYPE_MASK ) ) 
#define D3D12_DECODE_IS_COMPARISON_FILTER( D3D12Filter )                                                    \
                                 ( D3D12_DECODE_FILTER_REDUCTION( D3D12Filter ) == D3D12_FILTER_REDUCTION_TYPE_COMPARISON ) 
#define D3D12_DECODE_IS_ANISOTROPIC_FILTER( D3D12Filter )                                               \
                            ( ( ( D3D12Filter ) & D3D12_ANISOTROPIC_FILTERING_BIT ) &&                  \
                            ( D3D12_FILTER_TYPE_LINEAR == D3D12_DECODE_MIN_FILTER( D3D12Filter ) ) &&   \
                            ( D3D12_FILTER_TYPE_LINEAR == D3D12_DECODE_MAG_FILTER( D3D12Filter ) ) &&   \
                            ( D3D12_FILTER_TYPE_LINEAR == D3D12_DECODE_MIP_FILTER( D3D12Filter ) ) )      
typedef 
enum D3D12_TEXTURE_ADDRESS_MODE
    {
        D3D12_TEXTURE_ADDRESS_WRAP	= 1,
        D3D12_TEXTURE_ADDRESS_MIRROR	= 2,
        D3D12_TEXTURE_ADDRESS_CLAMP	= 3,
        D3D12_TEXTURE_ADDRESS_BORDER	= 4,
        D3D12_TEXTURE_ADDRESS_MIRROR_ONCE	= 5
    } 	D3D12_TEXTURE_ADDRESS_MODE;

typedef struct D3D12_SAMPLER_DESC
    {
    D3D12_FILTER Filter;
    D3D12_TEXTURE_ADDRESS_MODE AddressU;
    D3D12_TEXTURE_ADDRESS_MODE AddressV;
    D3D12_TEXTURE_ADDRESS_MODE AddressW;
    FLOAT MipLODBias;
    UINT MaxAnisotropy;
    D3D12_COMPARISON_FUNC ComparisonFunc;
    FLOAT BorderColor[ 4 ];
    FLOAT MinLOD;
    FLOAT MaxLOD;
    } 	D3D12_SAMPLER_DESC;

typedef 
enum D3D12_BUFFER_UAV_FLAG
    {
        D3D12_BUFFER_UAV_FLAG_RAW	= 0x1,
        D3D12_BUFFER_UAV_FLAG_APPEND	= 0x2,
        D3D12_BUFFER_UAV_FLAG_COUNTER	= 0x4
    } 	D3D12_BUFFER_UAV_FLAG;

typedef struct D3D12_BUFFER_UAV
    {
    UINT64 FirstElement;
    UINT NumElements;
    UINT StructureByteStride;
    ID3D11Query *pUAVCounter;
    UINT Flags;
    } 	D3D12_BUFFER_UAV;

typedef struct D3D12_TEX1D_UAV
    {
    UINT MipSlice;
    } 	D3D12_TEX1D_UAV;

typedef struct D3D12_TEX1D_ARRAY_UAV
    {
    UINT MipSlice;
    UINT FirstArraySlice;
    UINT ArraySize;
    } 	D3D12_TEX1D_ARRAY_UAV;

typedef struct D3D12_TEX2D_UAV
    {
    UINT MipSlice;
    } 	D3D12_TEX2D_UAV;

typedef struct D3D12_TEX2D_ARRAY_UAV
    {
    UINT MipSlice;
    UINT FirstArraySlice;
    UINT ArraySize;
    } 	D3D12_TEX2D_ARRAY_UAV;

typedef struct D3D12_TEX3D_UAV
    {
    UINT MipSlice;
    UINT FirstWSlice;
    UINT WSize;
    } 	D3D12_TEX3D_UAV;

typedef 
enum D3D12_UAV_DIMENSION
    {
        D3D12_UAV_DIMENSION_UNKNOWN	= 0,
        D3D12_UAV_DIMENSION_BUFFER	= 1,
        D3D12_UAV_DIMENSION_TEXTURE1D	= 2,
        D3D12_UAV_DIMENSION_TEXTURE1DARRAY	= 3,
        D3D12_UAV_DIMENSION_TEXTURE2D	= 4,
        D3D12_UAV_DIMENSION_TEXTURE2DARRAY	= 5,
        D3D12_UAV_DIMENSION_TEXTURE3D	= 8
    } 	D3D12_UAV_DIMENSION;

typedef struct D3D12_UNORDERED_ACCESS_VIEW_DESC
    {
    DXGI_FORMAT Format;
    D3D12_UAV_DIMENSION ViewDimension;
    union 
        {
        D3D12_BUFFER_UAV Buffer;
        D3D12_TEX1D_UAV Texture1D;
        D3D12_TEX1D_ARRAY_UAV Texture1DArray;
        D3D12_TEX2D_UAV Texture2D;
        D3D12_TEX2D_ARRAY_UAV Texture2DArray;
        D3D12_TEX3D_UAV Texture3D;
        } 	;
    } 	D3D12_UNORDERED_ACCESS_VIEW_DESC;

typedef struct D3D12_BUFFER_RTV
    {
    UINT64 FirstElement;
    UINT NumElements;
    } 	D3D12_BUFFER_RTV;

typedef struct D3D12_TEX1D_RTV
    {
    UINT MipSlice;
    } 	D3D12_TEX1D_RTV;

typedef struct D3D12_TEX1D_ARRAY_RTV
    {
    UINT MipSlice;
    UINT FirstArraySlice;
    UINT ArraySize;
    } 	D3D12_TEX1D_ARRAY_RTV;

typedef struct D3D12_TEX2D_RTV
    {
    UINT MipSlice;
    } 	D3D12_TEX2D_RTV;

typedef struct D3D12_TEX2DMS_RTV
    {
    UINT UnusedField_NothingToDefine;
    } 	D3D12_TEX2DMS_RTV;

typedef struct D3D12_TEX2D_ARRAY_RTV
    {
    UINT MipSlice;
    UINT FirstArraySlice;
    UINT ArraySize;
    } 	D3D12_TEX2D_ARRAY_RTV;

typedef struct D3D12_TEX2DMS_ARRAY_RTV
    {
    UINT FirstArraySlice;
    UINT ArraySize;
    } 	D3D12_TEX2DMS_ARRAY_RTV;

typedef struct D3D12_TEX3D_RTV
    {
    UINT MipSlice;
    UINT FirstWSlice;
    UINT WSize;
    } 	D3D12_TEX3D_RTV;

typedef 
enum D3D12_RTV_DIMENSION
    {
        D3D12_RTV_DIMENSION_UNKNOWN	= 0,
        D3D12_RTV_DIMENSION_BUFFER	= 1,
        D3D12_RTV_DIMENSION_TEXTURE1D	= 2,
        D3D12_RTV_DIMENSION_TEXTURE1DARRAY	= 3,
        D3D12_RTV_DIMENSION_TEXTURE2D	= 4,
        D3D12_RTV_DIMENSION_TEXTURE2DARRAY	= 5,
        D3D12_RTV_DIMENSION_TEXTURE2DMS	= 6,
        D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY	= 7,
        D3D12_RTV_DIMENSION_TEXTURE3D	= 8
    } 	D3D12_RTV_DIMENSION;

typedef struct D3D12_RENDER_TARGET_VIEW_DESC
    {
    DXGI_FORMAT Format;
    D3D12_RTV_DIMENSION ViewDimension;
    union 
        {
        D3D12_BUFFER_RTV Buffer;
        D3D12_TEX1D_RTV Texture1D;
        D3D12_TEX1D_ARRAY_RTV Texture1DArray;
        D3D12_TEX2D_RTV Texture2D;
        D3D12_TEX2D_ARRAY_RTV Texture2DArray;
        D3D12_TEX2DMS_RTV Texture2DMS;
        D3D12_TEX2DMS_ARRAY_RTV Texture2DMSArray;
        D3D12_TEX3D_RTV Texture3D;
        } 	;
    } 	D3D12_RENDER_TARGET_VIEW_DESC;

typedef struct D3D12_TEX1D_DSV
    {
    UINT MipSlice;
    } 	D3D12_TEX1D_DSV;

typedef struct D3D12_TEX1D_ARRAY_DSV
    {
    UINT MipSlice;
    UINT FirstArraySlice;
    UINT ArraySize;
    } 	D3D12_TEX1D_ARRAY_DSV;

typedef struct D3D12_TEX2D_DSV
    {
    UINT MipSlice;
    } 	D3D12_TEX2D_DSV;

typedef struct D3D12_TEX2D_ARRAY_DSV
    {
    UINT MipSlice;
    UINT FirstArraySlice;
    UINT ArraySize;
    } 	D3D12_TEX2D_ARRAY_DSV;

typedef struct D3D12_TEX2DMS_DSV
    {
    UINT UnusedField_NothingToDefine;
    } 	D3D12_TEX2DMS_DSV;

typedef struct D3D12_TEX2DMS_ARRAY_DSV
    {
    UINT FirstArraySlice;
    UINT ArraySize;
    } 	D3D12_TEX2DMS_ARRAY_DSV;

typedef 
enum D3D12_DSV_FLAG
    {
        D3D12_DSV_READ_ONLY_DEPTH	= 0x1L,
        D3D12_DSV_READ_ONLY_STENCIL	= 0x2L
    } 	D3D12_DSV_FLAG;

typedef 
enum D3D12_DSV_DIMENSION
    {
        D3D12_DSV_DIMENSION_UNKNOWN	= 0,
        D3D12_DSV_DIMENSION_TEXTURE1D	= 1,
        D3D12_DSV_DIMENSION_TEXTURE1DARRAY	= 2,
        D3D12_DSV_DIMENSION_TEXTURE2D	= 3,
        D3D12_DSV_DIMENSION_TEXTURE2DARRAY	= 4,
        D3D12_DSV_DIMENSION_TEXTURE2DMS	= 5,
        D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY	= 6
    } 	D3D12_DSV_DIMENSION;

typedef struct D3D12_DEPTH_STENCIL_VIEW_DESC
    {
    DXGI_FORMAT Format;
    D3D12_DSV_DIMENSION ViewDimension;
    UINT Flags;
    union 
        {
        D3D12_TEX1D_DSV Texture1D;
        D3D12_TEX1D_ARRAY_DSV Texture1DArray;
        D3D12_TEX2D_DSV Texture2D;
        D3D12_TEX2D_ARRAY_DSV Texture2DArray;
        D3D12_TEX2DMS_DSV Texture2DMS;
        D3D12_TEX2DMS_ARRAY_DSV Texture2DMSArray;
        } 	;
    } 	D3D12_DEPTH_STENCIL_VIEW_DESC;

typedef 
enum D3D12_FENCE_MISC_FLAG
    {
        D3D12_FENCE_MISC_NONE	= 0,
        D3D12_FENCE_MISC_SHARED	= 0x1
    } 	D3D12_FENCE_MISC_FLAG;

DEFINE_ENUM_FLAG_OPERATORS( D3D12_FENCE_MISC_FLAG );
typedef struct D3D12_VIDEO_MEMORY_INFO
    {
    UINT64 Budget;
    UINT64 CurrentUsage;
    UINT64 AvailableForReservation;
    UINT64 CurrentReservation;
    } 	D3D12_VIDEO_MEMORY_INFO;


typedef void ( *PFN_D3D12_BUDGET_CHANGED )( 
    ID3D12Callback *pCallback,
    PVOID pContext,
    UINT64 Budget);

typedef 
enum D3D12_CREATE_DEVICE_FLAG
    {
        D3D12_CREATE_DEVICE_NONE	= 0,
        D3D12_CREATE_DEVICE_DEBUG	= 0x1,
        D3D12_CREATE_DEVICE_SHADER_DEBUGGING	= 0x2,
        D3D12_CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY	= 0x4
    } 	D3D12_CREATE_DEVICE_FLAG;

DEFINE_ENUM_FLAG_OPERATORS( D3D12_CREATE_DEVICE_FLAG );
typedef 
enum D3D12_DESCRIPTOR_HEAP_TYPE
    {
        D3D12_CBV_SRV_UAV_DESCRIPTOR_HEAP	= 0,
        D3D12_SAMPLER_DESCRIPTOR_HEAP	= ( D3D12_CBV_SRV_UAV_DESCRIPTOR_HEAP + 1 ) ,
        D3D12_IBV_DESCRIPTOR_HEAP	= ( D3D12_SAMPLER_DESCRIPTOR_HEAP + 1 ) ,
        D3D12_VBV_DESCRIPTOR_HEAP	= ( D3D12_IBV_DESCRIPTOR_HEAP + 1 ) ,
        D3D12_SOV_DESCRIPTOR_HEAP	= ( D3D12_VBV_DESCRIPTOR_HEAP + 1 ) ,
        D3D12_RTV_DESCRIPTOR_HEAP	= ( D3D12_SOV_DESCRIPTOR_HEAP + 1 ) ,
        D3D12_DSV_DESCRIPTOR_HEAP	= ( D3D12_RTV_DESCRIPTOR_HEAP + 1 ) ,
        D3D12_NUM_DESCRIPTOR_HEAP_TYPES	= ( D3D12_DSV_DESCRIPTOR_HEAP + 1 ) 
    } 	D3D12_DESCRIPTOR_HEAP_TYPE;

typedef 
enum D3D12_DESCRIPTOR_HEAP_FLAGS
    {
        D3D12_DESCRIPTOR_HEAP_SHADER_VISIBLE	= 0x1
    } 	D3D12_DESCRIPTOR_HEAP_FLAGS;

typedef struct D3D12_DESCRIPTOR_HEAP_DESC
    {
    D3D12_DESCRIPTOR_HEAP_TYPE Type;
    UINT NumDescriptors;
    UINT Flags;
    } 	D3D12_DESCRIPTOR_HEAP_DESC;

typedef 
enum D3D12_DESCRIPTOR_RANGE_TYPE
    {
        D3D12_DESCRIPTOR_RANGE_SRV	= 0,
        D3D12_DESCRIPTOR_RANGE_UAV	= ( D3D12_DESCRIPTOR_RANGE_SRV + 1 ) ,
        D3D12_DESCRIPTOR_RANGE_CBV	= ( D3D12_DESCRIPTOR_RANGE_UAV + 1 ) ,
        D3D12_DESCRIPTOR_RANGE_SAMPLER	= ( D3D12_DESCRIPTOR_RANGE_CBV + 1 ) 
    } 	D3D12_DESCRIPTOR_RANGE_TYPE;

#define	D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND	( -1 )

#if defined( __cplusplus )
}
typedef struct D3D12_DESCRIPTOR_RANGE
{
    D3D12_DESCRIPTOR_RANGE_TYPE RangeType;
    UINT NumDescriptors;
    UINT BaseShaderRegister;
    UINT RegisterSpace; 
    UINT OffsetInDescriptorsFromTableStart;

    void Init(
        D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
        UINT numDescriptors,
        UINT baseShaderRegister,
        UINT registerSpace = 0,
        UINT offsetInDescriptorsFromTableStart =
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND)
    {
        RangeType = rangeType;
        NumDescriptors = numDescriptors;
        BaseShaderRegister = baseShaderRegister;
        RegisterSpace = registerSpace;
        OffsetInDescriptorsFromTableStart = offsetInDescriptorsFromTableStart;
    }
} D3D12_DESCRIPTOR_RANGE;
extern "C"{
#else
typedef struct D3D12_DESCRIPTOR_RANGE
    {
    D3D12_DESCRIPTOR_RANGE_TYPE RangeType;
    UINT NumDescriptors;
    UINT BaseShaderRegister;
    UINT RegisterSpace;
    UINT OffsetInDescriptorsFromTableStart;
    } 	D3D12_DESCRIPTOR_RANGE;

#endif
#if defined( __cplusplus )
}
typedef struct D3D12_ROOT_DESCRIPTOR_TABLE
{
    UINT NumDescriptorRanges;
    const D3D12_DESCRIPTOR_RANGE* pDescriptorRanges;

    void Init(
        UINT numDescriptorRanges,
        const D3D12_DESCRIPTOR_RANGE* _pDescriptorRanges)
    {
        NumDescriptorRanges = numDescriptorRanges;
        pDescriptorRanges = _pDescriptorRanges;
    }
} D3D12_ROOT_DESCRIPTOR_TABLE;
extern "C"{
#else
typedef struct D3D12_ROOT_DESCRIPTOR_TABLE
    {
    UINT NumDescriptorRanges;
    const D3D12_DESCRIPTOR_RANGE *pDescriptorRanges;
    } 	D3D12_ROOT_DESCRIPTOR_TABLE;

#endif
#if defined( __cplusplus )
}
typedef struct D3D12_ROOT_CONSTANTS
{
    UINT ShaderRegister;
    UINT RegisterSpace;
    UINT Num32BitValues;

    void Init(
        UINT num32BitValues,
        UINT shaderRegister,
        UINT registerSpace = 0)
    {
        Num32BitValues = num32BitValues;
        ShaderRegister = shaderRegister;
        RegisterSpace = registerSpace;
    }
} D3D12_ROOT_CONSTANTS;
extern "C"{
#else
typedef struct D3D12_ROOT_CONSTANTS
    {
    UINT ShaderRegister;
    UINT RegisterSpace;
    UINT Num32BitValues;
    } 	D3D12_ROOT_CONSTANTS;

#endif
#if defined( __cplusplus )
}
typedef struct D3D12_ROOT_DESCRIPTOR
{
    UINT ShaderRegister;
    UINT RegisterSpace;

    void Init(
        UINT shaderRegister,
        UINT registerSpace = 0)
    {
        ShaderRegister = shaderRegister;
        RegisterSpace = registerSpace;
    }
} D3D12_ROOT_DESCRIPTOR;
extern "C"{
#else
typedef struct D3D12_ROOT_DESCRIPTOR
    {
    UINT ShaderRegister;
    UINT RegisterSpace;
    } 	D3D12_ROOT_DESCRIPTOR;

#endif
typedef 
enum D3D12_SHADER_VISIBILITY
    {
        D3D12_SHADER_VISIBILITY_ALL	= 0,
        D3D12_SHADER_VISIBILITY_VERTEX	= 1,
        D3D12_SHADER_VISIBILITY_HULL	= 2,
        D3D12_SHADER_VISIBILITY_DOMAIN	= 3,
        D3D12_SHADER_VISIBILITY_GEOMETRY	= 4,
        D3D12_SHADER_VISIBILITY_PIXEL	= 5
    } 	D3D12_SHADER_VISIBILITY;

typedef 
enum D3D12_ROOT_PARAMETER_TYPE
    {
        D3D12_ROOT_PARAMETER_DESCRIPTOR_TABLE	= 0,
        D3D12_ROOT_PARAMETER_32BIT_CONSTANTS	= ( D3D12_ROOT_PARAMETER_DESCRIPTOR_TABLE + 1 ) ,
        D3D12_ROOT_PARAMETER_CBV	= ( D3D12_ROOT_PARAMETER_32BIT_CONSTANTS + 1 ) ,
        D3D12_ROOT_PARAMETER_SRV	= ( D3D12_ROOT_PARAMETER_CBV + 1 ) ,
        D3D12_ROOT_PARAMETER_UAV	= ( D3D12_ROOT_PARAMETER_SRV + 1 ) 
    } 	D3D12_ROOT_PARAMETER_TYPE;

#if defined( __cplusplus )
}
typedef struct D3D12_ROOT_PARAMETER
{
    D3D12_ROOT_PARAMETER_TYPE ParameterType;
    union
    {
        D3D12_ROOT_DESCRIPTOR_TABLE    DescriptorTable;
        D3D12_ROOT_CONSTANTS           Constants;
        D3D12_ROOT_DESCRIPTOR          Descriptor;
    };
    D3D12_SHADER_VISIBILITY ShaderVisibility;

    void InitAsDescriptorTable(
        UINT numDescriptorRanges,
        const D3D12_DESCRIPTOR_RANGE* pDescriptorRanges,
        D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
    {
        ParameterType = D3D12_ROOT_PARAMETER_DESCRIPTOR_TABLE;
        ShaderVisibility = visibility;
        DescriptorTable.Init(numDescriptorRanges, pDescriptorRanges);
    }

    void InitAsConstants(
        UINT num32BitValues,
        UINT shaderRegister,
        UINT registerSpace = 0,
        D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
    {
        ParameterType = D3D12_ROOT_PARAMETER_32BIT_CONSTANTS;
        ShaderVisibility = visibility;
        Constants.Init(num32BitValues, shaderRegister, registerSpace);
    }

    void InitAsConstantBufferView(
        UINT shaderRegister,
        UINT registerSpace = 0,
        D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
    {
        ParameterType = D3D12_ROOT_PARAMETER_CBV;
        ShaderVisibility = visibility;
        Descriptor.Init(shaderRegister, registerSpace);
    }

    void InitAsShaderResourceView(
        UINT shaderRegister,
        UINT registerSpace = 0,
        D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
    {
        ParameterType = D3D12_ROOT_PARAMETER_SRV;
        ShaderVisibility = visibility;
        Descriptor.Init(shaderRegister, registerSpace);
    }

    void InitAsUnorderedAccessView(
        UINT shaderRegister,
        UINT registerSpace = 0,
        D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
    {
        ParameterType = D3D12_ROOT_PARAMETER_UAV;
        ShaderVisibility = visibility;
        Descriptor.Init(shaderRegister, registerSpace);
    }
} D3D12_ROOT_PARAMETER;
extern "C"{
#else
typedef struct D3D12_ROOT_PARAMETER
    {
    D3D12_ROOT_PARAMETER_TYPE ParameterType;
    union 
        {
        D3D12_ROOT_DESCRIPTOR_TABLE DescriptorTable;
        D3D12_ROOT_CONSTANTS Constants;
        D3D12_ROOT_DESCRIPTOR Descriptor;
        } 	;
    D3D12_SHADER_VISIBILITY ShaderVisibility;
    } 	D3D12_ROOT_PARAMETER;

#endif
typedef 
enum D3D12_ROOT_SIGNATURE_FLAGS
    {
        D3D12_ROOT_SIGNATURE_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT	= 0x1,
        D3D12_ROOT_SIGNATURE_DENY_VERTEX_SHADER_ROOT_ACCESS	= 0x2,
        D3D12_ROOT_SIGNATURE_DENY_HULL_SHADER_ROOT_ACCESS	= 0x4,
        D3D12_ROOT_SIGNATURE_DENY_DOMAIN_SHADER_ROOT_ACCESS	= 0x8,
        D3D12_ROOT_SIGNATURE_DENY_GEOMETRY_SHADER_ROOT_ACCESS	= 0x10,
        D3D12_ROOT_SIGNATURE_DENY_PIXEL_SHADER_ROOT_ACCESS	= 0x20
    } 	D3D12_ROOT_SIGNATURE_FLAGS;

#if defined( __cplusplus )
}
typedef struct D3D12_ROOT_SIGNATURE
{
    UINT NumParameters;
    const D3D12_ROOT_PARAMETER* pParameters;
    UINT Flags;

    // Initialize struct
    void Init(
        UINT numParameters,
        const D3D12_ROOT_PARAMETER* _pParameters,
        UINT flags = 0)
    {
        NumParameters = numParameters;
        pParameters = _pParameters;
        Flags = flags;
    }

    D3D12_ROOT_SIGNATURE() { Init(0,NULL,0);}

    D3D12_ROOT_SIGNATURE(
        UINT numParameters,
        const D3D12_ROOT_PARAMETER* _pParameters,
        UINT flags = 0)
    {
        Init(numParameters, _pParameters, flags);
    }
} D3D12_ROOT_SIGNATURE;
extern "C"{
#else
typedef struct D3D12_ROOT_SIGNATURE
    {
    UINT NumParameters;
    const D3D12_ROOT_PARAMETER *pParameters;
    UINT Flags;
    } 	D3D12_ROOT_SIGNATURE;

#endif
typedef 
enum D3D_ROOT_SIGNATURE_VERSION
    {
        D3D_ROOT_SIGNATURE_V1	= 0x1
    } 	D3D_ROOT_SIGNATURE_VERSION;



extern RPC_IF_HANDLE __MIDL_itf_d3d12_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d12_0000_0001_v0_0_s_ifspec;

#ifndef __ID3D12RootSignatureDeserializer_INTERFACE_DEFINED__
#define __ID3D12RootSignatureDeserializer_INTERFACE_DEFINED__

/* interface ID3D12RootSignatureDeserializer */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12RootSignatureDeserializer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("34AB647B-3CC8-46AC-841B-C0965645C046")
    ID3D12RootSignatureDeserializer : public IUnknown
    {
    public:
        virtual const D3D12_ROOT_SIGNATURE *STDMETHODCALLTYPE GetRootSignature( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12RootSignatureDeserializerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12RootSignatureDeserializer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12RootSignatureDeserializer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12RootSignatureDeserializer * This);
        
        const D3D12_ROOT_SIGNATURE *( STDMETHODCALLTYPE *GetRootSignature )( 
            ID3D12RootSignatureDeserializer * This);
        
        END_INTERFACE
    } ID3D12RootSignatureDeserializerVtbl;

    interface ID3D12RootSignatureDeserializer
    {
        CONST_VTBL struct ID3D12RootSignatureDeserializerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12RootSignatureDeserializer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12RootSignatureDeserializer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12RootSignatureDeserializer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12RootSignatureDeserializer_GetRootSignature(This)	\
    ( (This)->lpVtbl -> GetRootSignature(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ID3D12RootSignatureDeserializer_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_d3d12_0000_0002 */
/* [local] */ 

typedef HRESULT (WINAPI* PFN_D3D12_SERIALIZE_ROOT_SIGNATURE)(
                            _In_ const D3D12_ROOT_SIGNATURE* pRootSignature,
                            _In_ D3D_ROOT_SIGNATURE_VERSION Version,
                            _Out_ ID3DBlob** ppBlob,
                            _Always_(_Outptr_opt_result_maybenull_) ID3DBlob** ppErrorBlob);

HRESULT WINAPI D3D12SerializeRootSignature(
                            _In_ const D3D12_ROOT_SIGNATURE* pRootSignature,
                            _In_ D3D_ROOT_SIGNATURE_VERSION Version,
                            _Out_ ID3DBlob** ppBlob,
                            _Always_(_Outptr_opt_result_maybenull_) ID3DBlob** ppErrorBlob);

typedef HRESULT (WINAPI* PFN_D3D12_CREATE_ROOT_SIGNATURE_DESERIALIZER)(
                                     _In_reads_bytes_(SrcDataSizeInBytes) LPCVOID pSrcData,
                                     _In_ SIZE_T SrcDataSizeInBytes,
                                     _In_ REFIID pRootSignatureDeserializerInterface,
                                     _Out_ void** ppRootSignatureDeserializer);

HRESULT WINAPI D3D12CreateRootSignatureDeserializer(
                                     _In_reads_bytes_(SrcDataSizeInBytes) LPCVOID pSrcData,
                                     _In_ SIZE_T SrcDataSizeInBytes,
                                     _In_ REFIID pRootSignatureDeserializerInterface,
                                     _Out_ void** ppRootSignatureDeserializer);
#if defined( __cplusplus )
}
typedef struct D3D12_CPU_DESCRIPTOR_HANDLE
{
    SIZE_T ptr;

    D3D12_CPU_DESCRIPTOR_HANDLE MakeOffsetted(INT offsetInDecriptors, UINT descriptorIncrementSize) const 
    { 
        D3D12_CPU_DESCRIPTOR_HANDLE offsetted;
        offsetted.ptr = ptr + offsetInDecriptors * descriptorIncrementSize;
        return offsetted;
    }
    D3D12_CPU_DESCRIPTOR_HANDLE MakeOffsetted(INT offsetScaledByIncrementSize) const 
    { 
        D3D12_CPU_DESCRIPTOR_HANDLE offsetted;
        offsetted.ptr = ptr + offsetScaledByIncrementSize;
        return offsetted;
    }
    D3D12_CPU_DESCRIPTOR_HANDLE& Offset(INT offsetInDecriptors, UINT descriptorIncrementSize)
    { 
        ptr += offsetInDecriptors * descriptorIncrementSize;
        return *this;
    }
    D3D12_CPU_DESCRIPTOR_HANDLE& Offset(INT offsetScaledByIncrementSize) 
    { 
        ptr += offsetScaledByIncrementSize;
        return *this;
    }

} D3D12_CPU_DESCRIPTOR_HANDLE;
extern "C"{
#else
typedef struct D3D12_CPU_DESCRIPTOR_HANDLE
    {
    SIZE_T ptr;
    } 	D3D12_CPU_DESCRIPTOR_HANDLE;

#endif
#if defined( __cplusplus )
}
typedef struct D3D12_GPU_DESCRIPTOR_HANDLE
{
    UINT64 ptr;

    D3D12_GPU_DESCRIPTOR_HANDLE MakeOffsetted(INT offsetInDecriptors, UINT descriptorIncrementSize) const 
    { 
        D3D12_GPU_DESCRIPTOR_HANDLE offsetted;
        offsetted.ptr = ptr + offsetInDecriptors * descriptorIncrementSize;
        return offsetted;
    }
    D3D12_GPU_DESCRIPTOR_HANDLE MakeOffsetted(INT offsetScaledByIncrementSize) const 
    { 
        D3D12_GPU_DESCRIPTOR_HANDLE offsetted;
        offsetted.ptr = ptr + offsetScaledByIncrementSize;
        return offsetted;
    }
    D3D12_GPU_DESCRIPTOR_HANDLE& Offset(INT offsetInDecriptors, UINT descriptorIncrementSize)
    { 
        ptr += offsetInDecriptors * descriptorIncrementSize;
        return *this;
    }
    D3D12_GPU_DESCRIPTOR_HANDLE& Offset(INT offsetScaledByIncrementSize) 
    { 
        ptr += offsetScaledByIncrementSize;
        return *this;
    }

} D3D12_GPU_DESCRIPTOR_HANDLE;
extern "C"{
#else
typedef struct D3D12_GPU_DESCRIPTOR_HANDLE
    {
    UINT64 ptr;
    } 	D3D12_GPU_DESCRIPTOR_HANDLE;

#endif
// If rects are supplied in D3D12_DISCARD_RESOURCE_DESC, below, the resource 
// must have 2D subresources with all specified subresources the same dimension.
typedef struct D3D12_DISCARD_RESOURCE_DESC
    {
    /* [annotation] */ 
    _In_reads_(NumRects)  const D3D12_RECT *pRects;
    UINT NumRects;
    UINT FirstSubresource;
    UINT NumSubresources;
    } 	D3D12_DISCARD_RESOURCE_DESC;


///////////////////////////////////////////////////////////////////////////
// D3D12CreateDevice
// ------------------
//
// pAdapter
//      If NULL, D3D12CreateDevice will choose the primary adapter and
//      create a new instance from a temporarily created IDXGIFactory.
//      If non-NULL, D3D12CreateDevice will use the provided adapter.
// DriverType
//      Specifies the driver type to be created: hardware or WARP
// Flags
//      Any of those documented for D3D12CreateDeviceAndSwapChain.
// MinimumFeatureLevel
//      The minimum feature level required for successful device creation.
// SDKVersion
//      SDK version. Use the D3D12_SDK_VERSION macro.
// riid
//      The interface IID of the device to be returned. Expected: ID3D12Device.
// ppDevice
//      Pointer to returned interface. May be NULL.
// pFeatureLevel
//      Pointer to returned feature level. May be NULL.
//
// Return Values
//  Any of those documented for 
//          CreateDXGIFactory1
//          IDXGIFactory::EnumAdapters
//          D3D12CreateDevice
//
///////////////////////////////////////////////////////////////////////////
typedef HRESULT (WINAPI* PFN_D3D12_CREATE_DEVICE)( _In_opt_ IDXGIAdapter*, 
    D3D_DRIVER_TYPE, D3D12_CREATE_DEVICE_FLAG, 
    D3D_FEATURE_LEVEL, 
    UINT, _In_ REFIID, _COM_Outptr_opt_ void** );

HRESULT WINAPI D3D12CreateDevice(
    _In_opt_ IDXGIAdapter* pAdapter,
    D3D_DRIVER_TYPE DriverType,
    D3D12_CREATE_DEVICE_FLAG Flags,
    D3D_FEATURE_LEVEL MinimumFeatureLevel,
    UINT SDKVersion,
    _In_ REFIID riid, // Expected: ID3D12Device
    _COM_Outptr_opt_ void** ppDevice );


///////////////////////////////////////////////////////////////////////////
// D3D11CreateDeviceForD3D12
// ------------------
//
// pDevice
//      Specifies a pre-existing D3D12 device to use for D3D11 interop.
//      May not be NULL.
// Flags
//      Any of those documented for D3D11CreateDeviceAndSwapChain.
// pFeatureLevels
//      Array of any of the following:
//          D3D_FEATURE_LEVEL_11_1
//          D3D_FEATURE_LEVEL_11_0
//          D3D_FEATURE_LEVEL_10_1
//          D3D_FEATURE_LEVEL_10_0
//          D3D_FEATURE_LEVEL_9_3
//          D3D_FEATURE_LEVEL_9_2
//          D3D_FEATURE_LEVEL_9_1
//       The first feature level which is less than or equal to the
//       D3D12 device's feature level will be used to perform D3D11 validation.
//       Creation will fail if no acceptable feature levels are provided.
//       Providing NULL will default to the D3D12 device's feature level.
// FeatureLevels
//      Size of feature levels array.
// SDKVersion
//      SDK version. Use the D3D11_SDK_VERSION macro.
// ppDevice
//      Pointer to returned interface. May be NULL.
// ppImmediateContext
//      Pointer to returned interface. May be NULL.
// pChosenFeatureLevel
//      Pointer to returned feature level. May be NULL.
//
// Return Values
//  Any of those documented for 
//          D3D11CreateDevice
//
///////////////////////////////////////////////////////////////////////////
typedef HRESULT (WINAPI* PFN_D3D11_CREATE_DEVICE_FOR_D3D12)( _In_ IUnknown*, UINT, 
    _In_reads_opt_( FeatureLevels ) CONST D3D_FEATURE_LEVEL*, 
    UINT FeatureLevels, UINT, _COM_Outptr_opt_ ID3D11Device**, _COM_Outptr_opt_ ID3D11DeviceContext**, 
    _Out_opt_ D3D_FEATURE_LEVEL* );

HRESULT WINAPI D3D11CreateDeviceForD3D12(
    _In_ IUnknown* pDevice,
    UINT Flags,
    _In_reads_opt_( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    _COM_Outptr_opt_ ID3D11Device** ppDevice,
    _COM_Outptr_opt_ ID3D11DeviceContext** ppImmediateContext,
    _Out_opt_ D3D_FEATURE_LEVEL* pChosenFeatureLevel );

#pragma region Desktop Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
///////////////////////////////////////////////////////////////////////////
// D3D12CreateDeviceAndSwapChain
// ------------------------------
//
// pAdapter
//      If NULL, D3D12CreateDevice will choose the primary adapter and
//      create a new instance from a temporarily created IDXGIFactory.
//      If non-NULL, D3D12CreateDevice will use the provided adapter.
// DriverType
//      Specifies the driver type to be created: hardware or WARP
// Flags
//      Any of those documented for D3D12CreateDevice.
// MinimumFeatureLevels
//      Any of the following:
//          D3D_FEATURE_LEVEL_11_1
//          D3D_FEATURE_LEVEL_11_0
//          D3D_FEATURE_LEVEL_10_1
//          D3D_FEATURE_LEVEL_10_0
//          D3D_FEATURE_LEVEL_9_3
//          D3D_FEATURE_LEVEL_9_2
//          D3D_FEATURE_LEVEL_9_1
//      The D3D12 runtime will always choose the best feature level that
//      is supported by the hardware. Use this parameter to indicate a
//      minimum feature level required for successful device creation.
// SDKVersion
//      SDK version. Use the D3D12_SDK_VERSION macro.
// pSwapChainDesc
//      Swap chain description, may be NULL.
// ppSwapChain
//      Pointer to returned interface. May be NULL.
// ppDevice
//      Pointer to returned interface. May be NULL.
// pFeatureLevel
//      Pointer to returned feature level. May be NULL.
//
// Return Values
//  Any of those documented for 
//          CreateDXGIFactory1
//          IDXGIFactory::EnumAdapters
//          IDXGIAdapter::RegisterDriver
//          D3D12CreateDevice
//          IDXGIFactory::CreateSwapChain
//
///////////////////////////////////////////////////////////////////////////
typedef HRESULT (WINAPI* PFN_D3D12_CREATE_DEVICE_AND_SWAP_CHAIN)( _In_opt_ IDXGIAdapter*, 
    D3D_DRIVER_TYPE, HMODULE, UINT, 
    D3D_FEATURE_LEVEL, UINT, _In_opt_ CONST DXGI_SWAP_CHAIN_DESC*, 
    _In_ REFIID, _COM_Outptr_opt_ void**, 
    _In_ REFIID, _COM_Outptr_opt_ void** );

HRESULT WINAPI D3D12CreateDeviceAndSwapChain(
    _In_opt_ IDXGIAdapter* pAdapter,
    D3D_DRIVER_TYPE DriverType,
    D3D12_CREATE_DEVICE_FLAG Flags,
    D3D_FEATURE_LEVEL MinimumFeatureLevel,
    UINT SDKVersion,
    _In_opt_ CONST DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
    _In_ REFIID riidSwapchain, // Expected: IDXGISwapChain
    _COM_Outptr_opt_ void** ppSwapChain,
    _In_ REFIID riidDevice, // Expected: ID3D12Device
    _COM_Outptr_opt_ void** ppDevice );

#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) */
#pragma endregion
#define	D3D12_SDK_VERSION	( 12 )




extern RPC_IF_HANDLE __MIDL_itf_d3d12_0000_0002_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d12_0000_0002_v0_0_s_ifspec;

#ifndef __ID3D12Pageable_INTERFACE_DEFINED__
#define __ID3D12Pageable_INTERFACE_DEFINED__

/* interface ID3D12Pageable */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12Pageable;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("63ee58fb-1268-4835-86da-f008ce62f0d6")
    ID3D12Pageable : public ID3D12DeviceChild
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12PageableVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12Pageable * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12Pageable * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12Pageable * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetPrivateData )( 
            ID3D12Pageable * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _Inout_  UINT *pDataSize,
            /* [annotation] */ 
            _Out_writes_bytes_opt_( *pDataSize )  void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateData )( 
            ID3D12Pageable * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_  UINT DataSize,
            /* [annotation] */ 
            _In_reads_bytes_opt_( DataSize )  const void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )( 
            ID3D12Pageable * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_opt_  const IUnknown *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetName )( 
            ID3D12Pageable * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR pName);
        
        void ( STDMETHODCALLTYPE *GetDevice )( 
            ID3D12Pageable * This,
            /* [annotation] */ 
            _Out_  ID3D12Device **ppDevice);
        
        END_INTERFACE
    } ID3D12PageableVtbl;

    interface ID3D12Pageable
    {
        CONST_VTBL struct ID3D12PageableVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12Pageable_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12Pageable_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12Pageable_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12Pageable_GetPrivateData(This,guid,pDataSize,pData)	\
    ( (This)->lpVtbl -> GetPrivateData(This,guid,pDataSize,pData) ) 

#define ID3D12Pageable_SetPrivateData(This,guid,DataSize,pData)	\
    ( (This)->lpVtbl -> SetPrivateData(This,guid,DataSize,pData) ) 

#define ID3D12Pageable_SetPrivateDataInterface(This,guid,pData)	\
    ( (This)->lpVtbl -> SetPrivateDataInterface(This,guid,pData) ) 

#define ID3D12Pageable_SetName(This,pName)	\
    ( (This)->lpVtbl -> SetName(This,pName) ) 


#define ID3D12Pageable_GetDevice(This,ppDevice)	\
    ( (This)->lpVtbl -> GetDevice(This,ppDevice) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ID3D12Pageable_INTERFACE_DEFINED__ */


#ifndef __ID3D12Heap_INTERFACE_DEFINED__
#define __ID3D12Heap_INTERFACE_DEFINED__

/* interface ID3D12Heap */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12Heap;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("f98f3ba4-e608-4cf6-9e29-a14e50d4929d")
    ID3D12Heap : public ID3D12Pageable
    {
    public:
        virtual D3D12_HEAP_DESC STDMETHODCALLTYPE GetDesc( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12HeapVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12Heap * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12Heap * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12Heap * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetPrivateData )( 
            ID3D12Heap * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _Inout_  UINT *pDataSize,
            /* [annotation] */ 
            _Out_writes_bytes_opt_( *pDataSize )  void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateData )( 
            ID3D12Heap * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_  UINT DataSize,
            /* [annotation] */ 
            _In_reads_bytes_opt_( DataSize )  const void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )( 
            ID3D12Heap * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_opt_  const IUnknown *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetName )( 
            ID3D12Heap * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR pName);
        
        void ( STDMETHODCALLTYPE *GetDevice )( 
            ID3D12Heap * This,
            /* [annotation] */ 
            _Out_  ID3D12Device **ppDevice);
        
        D3D12_HEAP_DESC ( STDMETHODCALLTYPE *GetDesc )( 
            ID3D12Heap * This);
        
        END_INTERFACE
    } ID3D12HeapVtbl;

    interface ID3D12Heap
    {
        CONST_VTBL struct ID3D12HeapVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12Heap_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12Heap_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12Heap_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12Heap_GetPrivateData(This,guid,pDataSize,pData)	\
    ( (This)->lpVtbl -> GetPrivateData(This,guid,pDataSize,pData) ) 

#define ID3D12Heap_SetPrivateData(This,guid,DataSize,pData)	\
    ( (This)->lpVtbl -> SetPrivateData(This,guid,DataSize,pData) ) 

#define ID3D12Heap_SetPrivateDataInterface(This,guid,pData)	\
    ( (This)->lpVtbl -> SetPrivateDataInterface(This,guid,pData) ) 

#define ID3D12Heap_SetName(This,pName)	\
    ( (This)->lpVtbl -> SetName(This,pName) ) 


#define ID3D12Heap_GetDevice(This,ppDevice)	\
    ( (This)->lpVtbl -> GetDevice(This,ppDevice) ) 



#define ID3D12Heap_GetDesc(This)	\
    ( (This)->lpVtbl -> GetDesc(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



D3D12_HEAP_DESC STDMETHODCALLTYPE ID3D12Heap_GetDesc_Proxy( 
    ID3D12Heap * This);


void __RPC_STUB ID3D12Heap_GetDesc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ID3D12Heap_INTERFACE_DEFINED__ */


#ifndef __ID3D12Resource_INTERFACE_DEFINED__
#define __ID3D12Resource_INTERFACE_DEFINED__

/* interface ID3D12Resource */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12Resource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("696442be-a72e-4059-bc79-5b5c98040fad")
    ID3D12Resource : public ID3D12Pageable
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Map( 
            /* [annotation] */ 
            _In_opt_  const D3D12_RANGE *pReadRange,
            /* [annotation] */ 
            _Outptr_  void **ppData) = 0;
        
        virtual void STDMETHODCALLTYPE Unmap( 
            /* [annotation] */ 
            _In_opt_  const D3D12_RANGE *pWrittenRange) = 0;
        
        virtual D3D12_RESOURCE_DESC STDMETHODCALLTYPE GetDesc( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12ResourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12Resource * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12Resource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12Resource * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetPrivateData )( 
            ID3D12Resource * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _Inout_  UINT *pDataSize,
            /* [annotation] */ 
            _Out_writes_bytes_opt_( *pDataSize )  void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateData )( 
            ID3D12Resource * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_  UINT DataSize,
            /* [annotation] */ 
            _In_reads_bytes_opt_( DataSize )  const void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )( 
            ID3D12Resource * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_opt_  const IUnknown *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetName )( 
            ID3D12Resource * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR pName);
        
        void ( STDMETHODCALLTYPE *GetDevice )( 
            ID3D12Resource * This,
            /* [annotation] */ 
            _Out_  ID3D12Device **ppDevice);
        
        HRESULT ( STDMETHODCALLTYPE *Map )( 
            ID3D12Resource * This,
            /* [annotation] */ 
            _In_opt_  const D3D12_RANGE *pReadRange,
            /* [annotation] */ 
            _Outptr_  void **ppData);
        
        void ( STDMETHODCALLTYPE *Unmap )( 
            ID3D12Resource * This,
            /* [annotation] */ 
            _In_opt_  const D3D12_RANGE *pWrittenRange);
        
        D3D12_RESOURCE_DESC ( STDMETHODCALLTYPE *GetDesc )( 
            ID3D12Resource * This);
        
        END_INTERFACE
    } ID3D12ResourceVtbl;

    interface ID3D12Resource
    {
        CONST_VTBL struct ID3D12ResourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12Resource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12Resource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12Resource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12Resource_GetPrivateData(This,guid,pDataSize,pData)	\
    ( (This)->lpVtbl -> GetPrivateData(This,guid,pDataSize,pData) ) 

#define ID3D12Resource_SetPrivateData(This,guid,DataSize,pData)	\
    ( (This)->lpVtbl -> SetPrivateData(This,guid,DataSize,pData) ) 

#define ID3D12Resource_SetPrivateDataInterface(This,guid,pData)	\
    ( (This)->lpVtbl -> SetPrivateDataInterface(This,guid,pData) ) 

#define ID3D12Resource_SetName(This,pName)	\
    ( (This)->lpVtbl -> SetName(This,pName) ) 


#define ID3D12Resource_GetDevice(This,ppDevice)	\
    ( (This)->lpVtbl -> GetDevice(This,ppDevice) ) 



#define ID3D12Resource_Map(This,pReadRange,ppData)	\
    ( (This)->lpVtbl -> Map(This,pReadRange,ppData) ) 

#define ID3D12Resource_Unmap(This,pWrittenRange)	\
    ( (This)->lpVtbl -> Unmap(This,pWrittenRange) ) 

#define ID3D12Resource_GetDesc(This)	\
    ( (This)->lpVtbl -> GetDesc(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



D3D12_RESOURCE_DESC STDMETHODCALLTYPE ID3D12Resource_GetDesc_Proxy( 
    ID3D12Resource * This);


void __RPC_STUB ID3D12Resource_GetDesc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ID3D12Resource_INTERFACE_DEFINED__ */


#ifndef __ID3D12CommandAllocator_INTERFACE_DEFINED__
#define __ID3D12CommandAllocator_INTERFACE_DEFINED__

/* interface ID3D12CommandAllocator */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12CommandAllocator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6102dee4-af59-4b09-b999-b44d73f09b24")
    ID3D12CommandAllocator : public ID3D12Pageable
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12CommandAllocatorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12CommandAllocator * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12CommandAllocator * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12CommandAllocator * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetPrivateData )( 
            ID3D12CommandAllocator * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _Inout_  UINT *pDataSize,
            /* [annotation] */ 
            _Out_writes_bytes_opt_( *pDataSize )  void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateData )( 
            ID3D12CommandAllocator * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_  UINT DataSize,
            /* [annotation] */ 
            _In_reads_bytes_opt_( DataSize )  const void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )( 
            ID3D12CommandAllocator * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_opt_  const IUnknown *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetName )( 
            ID3D12CommandAllocator * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR pName);
        
        void ( STDMETHODCALLTYPE *GetDevice )( 
            ID3D12CommandAllocator * This,
            /* [annotation] */ 
            _Out_  ID3D12Device **ppDevice);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            ID3D12CommandAllocator * This);
        
        END_INTERFACE
    } ID3D12CommandAllocatorVtbl;

    interface ID3D12CommandAllocator
    {
        CONST_VTBL struct ID3D12CommandAllocatorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12CommandAllocator_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12CommandAllocator_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12CommandAllocator_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12CommandAllocator_GetPrivateData(This,guid,pDataSize,pData)	\
    ( (This)->lpVtbl -> GetPrivateData(This,guid,pDataSize,pData) ) 

#define ID3D12CommandAllocator_SetPrivateData(This,guid,DataSize,pData)	\
    ( (This)->lpVtbl -> SetPrivateData(This,guid,DataSize,pData) ) 

#define ID3D12CommandAllocator_SetPrivateDataInterface(This,guid,pData)	\
    ( (This)->lpVtbl -> SetPrivateDataInterface(This,guid,pData) ) 

#define ID3D12CommandAllocator_SetName(This,pName)	\
    ( (This)->lpVtbl -> SetName(This,pName) ) 


#define ID3D12CommandAllocator_GetDevice(This,ppDevice)	\
    ( (This)->lpVtbl -> GetDevice(This,ppDevice) ) 



#define ID3D12CommandAllocator_Reset(This)	\
    ( (This)->lpVtbl -> Reset(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ID3D12CommandAllocator_INTERFACE_DEFINED__ */


#ifndef __ID3D12Fence_INTERFACE_DEFINED__
#define __ID3D12Fence_INTERFACE_DEFINED__

/* interface ID3D12Fence */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12Fence;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0a753dcf-c4d8-4b91-adf6-be5a60d95a76")
    ID3D12Fence : public ID3D12Pageable
    {
    public:
        virtual UINT64 STDMETHODCALLTYPE GetCompletedValue( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetEventOnCompletion( 
            UINT64 Value,
            HANDLE hEvent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Signal( 
            UINT64 Value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12FenceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12Fence * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12Fence * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12Fence * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetPrivateData )( 
            ID3D12Fence * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _Inout_  UINT *pDataSize,
            /* [annotation] */ 
            _Out_writes_bytes_opt_( *pDataSize )  void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateData )( 
            ID3D12Fence * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_  UINT DataSize,
            /* [annotation] */ 
            _In_reads_bytes_opt_( DataSize )  const void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )( 
            ID3D12Fence * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_opt_  const IUnknown *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetName )( 
            ID3D12Fence * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR pName);
        
        void ( STDMETHODCALLTYPE *GetDevice )( 
            ID3D12Fence * This,
            /* [annotation] */ 
            _Out_  ID3D12Device **ppDevice);
        
        UINT64 ( STDMETHODCALLTYPE *GetCompletedValue )( 
            ID3D12Fence * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetEventOnCompletion )( 
            ID3D12Fence * This,
            UINT64 Value,
            HANDLE hEvent);
        
        HRESULT ( STDMETHODCALLTYPE *Signal )( 
            ID3D12Fence * This,
            UINT64 Value);
        
        END_INTERFACE
    } ID3D12FenceVtbl;

    interface ID3D12Fence
    {
        CONST_VTBL struct ID3D12FenceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12Fence_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12Fence_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12Fence_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12Fence_GetPrivateData(This,guid,pDataSize,pData)	\
    ( (This)->lpVtbl -> GetPrivateData(This,guid,pDataSize,pData) ) 

#define ID3D12Fence_SetPrivateData(This,guid,DataSize,pData)	\
    ( (This)->lpVtbl -> SetPrivateData(This,guid,DataSize,pData) ) 

#define ID3D12Fence_SetPrivateDataInterface(This,guid,pData)	\
    ( (This)->lpVtbl -> SetPrivateDataInterface(This,guid,pData) ) 

#define ID3D12Fence_SetName(This,pName)	\
    ( (This)->lpVtbl -> SetName(This,pName) ) 


#define ID3D12Fence_GetDevice(This,ppDevice)	\
    ( (This)->lpVtbl -> GetDevice(This,ppDevice) ) 



#define ID3D12Fence_GetCompletedValue(This)	\
    ( (This)->lpVtbl -> GetCompletedValue(This) ) 

#define ID3D12Fence_SetEventOnCompletion(This,Value,hEvent)	\
    ( (This)->lpVtbl -> SetEventOnCompletion(This,Value,hEvent) ) 

#define ID3D12Fence_Signal(This,Value)	\
    ( (This)->lpVtbl -> Signal(This,Value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ID3D12Fence_INTERFACE_DEFINED__ */


#ifndef __ID3D12PipelineState_INTERFACE_DEFINED__
#define __ID3D12PipelineState_INTERFACE_DEFINED__

/* interface ID3D12PipelineState */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12PipelineState;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("765a30f3-f624-4c6f-a828-ace948622445")
    ID3D12PipelineState : public ID3D12Pageable
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12PipelineStateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12PipelineState * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12PipelineState * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12PipelineState * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetPrivateData )( 
            ID3D12PipelineState * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _Inout_  UINT *pDataSize,
            /* [annotation] */ 
            _Out_writes_bytes_opt_( *pDataSize )  void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateData )( 
            ID3D12PipelineState * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_  UINT DataSize,
            /* [annotation] */ 
            _In_reads_bytes_opt_( DataSize )  const void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )( 
            ID3D12PipelineState * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_opt_  const IUnknown *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetName )( 
            ID3D12PipelineState * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR pName);
        
        void ( STDMETHODCALLTYPE *GetDevice )( 
            ID3D12PipelineState * This,
            /* [annotation] */ 
            _Out_  ID3D12Device **ppDevice);
        
        END_INTERFACE
    } ID3D12PipelineStateVtbl;

    interface ID3D12PipelineState
    {
        CONST_VTBL struct ID3D12PipelineStateVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12PipelineState_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12PipelineState_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12PipelineState_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12PipelineState_GetPrivateData(This,guid,pDataSize,pData)	\
    ( (This)->lpVtbl -> GetPrivateData(This,guid,pDataSize,pData) ) 

#define ID3D12PipelineState_SetPrivateData(This,guid,DataSize,pData)	\
    ( (This)->lpVtbl -> SetPrivateData(This,guid,DataSize,pData) ) 

#define ID3D12PipelineState_SetPrivateDataInterface(This,guid,pData)	\
    ( (This)->lpVtbl -> SetPrivateDataInterface(This,guid,pData) ) 

#define ID3D12PipelineState_SetName(This,pName)	\
    ( (This)->lpVtbl -> SetName(This,pName) ) 


#define ID3D12PipelineState_GetDevice(This,ppDevice)	\
    ( (This)->lpVtbl -> GetDevice(This,ppDevice) ) 



#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ID3D12PipelineState_INTERFACE_DEFINED__ */


#ifndef __ID3D12DescriptorHeap_INTERFACE_DEFINED__
#define __ID3D12DescriptorHeap_INTERFACE_DEFINED__

/* interface ID3D12DescriptorHeap */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12DescriptorHeap;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3c2922d5-f9eb-41a8-9a15-9e707610984a")
    ID3D12DescriptorHeap : public ID3D12Pageable
    {
    public:
        virtual void STDMETHODCALLTYPE GetDesc( 
            /* [annotation] */ 
            _Out_  D3D12_DESCRIPTOR_HEAP_DESC *pDescriptorHeapDesc) = 0;
        
        virtual D3D12_CPU_DESCRIPTOR_HANDLE STDMETHODCALLTYPE GetCPUDescriptorHandleForHeapStart( void) = 0;
        
        virtual D3D12_GPU_DESCRIPTOR_HANDLE STDMETHODCALLTYPE GetGPUDescriptorHandleForHeapStart( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12DescriptorHeapVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12DescriptorHeap * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12DescriptorHeap * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12DescriptorHeap * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetPrivateData )( 
            ID3D12DescriptorHeap * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _Inout_  UINT *pDataSize,
            /* [annotation] */ 
            _Out_writes_bytes_opt_( *pDataSize )  void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateData )( 
            ID3D12DescriptorHeap * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_  UINT DataSize,
            /* [annotation] */ 
            _In_reads_bytes_opt_( DataSize )  const void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )( 
            ID3D12DescriptorHeap * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_opt_  const IUnknown *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetName )( 
            ID3D12DescriptorHeap * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR pName);
        
        void ( STDMETHODCALLTYPE *GetDevice )( 
            ID3D12DescriptorHeap * This,
            /* [annotation] */ 
            _Out_  ID3D12Device **ppDevice);
        
        void ( STDMETHODCALLTYPE *GetDesc )( 
            ID3D12DescriptorHeap * This,
            /* [annotation] */ 
            _Out_  D3D12_DESCRIPTOR_HEAP_DESC *pDescriptorHeapDesc);
        
        D3D12_CPU_DESCRIPTOR_HANDLE ( STDMETHODCALLTYPE *GetCPUDescriptorHandleForHeapStart )( 
            ID3D12DescriptorHeap * This);
        
        D3D12_GPU_DESCRIPTOR_HANDLE ( STDMETHODCALLTYPE *GetGPUDescriptorHandleForHeapStart )( 
            ID3D12DescriptorHeap * This);
        
        END_INTERFACE
    } ID3D12DescriptorHeapVtbl;

    interface ID3D12DescriptorHeap
    {
        CONST_VTBL struct ID3D12DescriptorHeapVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12DescriptorHeap_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12DescriptorHeap_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12DescriptorHeap_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12DescriptorHeap_GetPrivateData(This,guid,pDataSize,pData)	\
    ( (This)->lpVtbl -> GetPrivateData(This,guid,pDataSize,pData) ) 

#define ID3D12DescriptorHeap_SetPrivateData(This,guid,DataSize,pData)	\
    ( (This)->lpVtbl -> SetPrivateData(This,guid,DataSize,pData) ) 

#define ID3D12DescriptorHeap_SetPrivateDataInterface(This,guid,pData)	\
    ( (This)->lpVtbl -> SetPrivateDataInterface(This,guid,pData) ) 

#define ID3D12DescriptorHeap_SetName(This,pName)	\
    ( (This)->lpVtbl -> SetName(This,pName) ) 


#define ID3D12DescriptorHeap_GetDevice(This,ppDevice)	\
    ( (This)->lpVtbl -> GetDevice(This,ppDevice) ) 



#define ID3D12DescriptorHeap_GetDesc(This,pDescriptorHeapDesc)	\
    ( (This)->lpVtbl -> GetDesc(This,pDescriptorHeapDesc) ) 

#define ID3D12DescriptorHeap_GetCPUDescriptorHandleForHeapStart(This)	\
    ( (This)->lpVtbl -> GetCPUDescriptorHandleForHeapStart(This) ) 

#define ID3D12DescriptorHeap_GetGPUDescriptorHandleForHeapStart(This)	\
    ( (This)->lpVtbl -> GetGPUDescriptorHandleForHeapStart(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



D3D12_CPU_DESCRIPTOR_HANDLE STDMETHODCALLTYPE ID3D12DescriptorHeap_GetCPUDescriptorHandleForHeapStart_Proxy( 
    ID3D12DescriptorHeap * This);


void __RPC_STUB ID3D12DescriptorHeap_GetCPUDescriptorHandleForHeapStart_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


D3D12_GPU_DESCRIPTOR_HANDLE STDMETHODCALLTYPE ID3D12DescriptorHeap_GetGPUDescriptorHandleForHeapStart_Proxy( 
    ID3D12DescriptorHeap * This);


void __RPC_STUB ID3D12DescriptorHeap_GetGPUDescriptorHandleForHeapStart_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ID3D12DescriptorHeap_INTERFACE_DEFINED__ */


#ifndef __ID3D12CommandList_INTERFACE_DEFINED__
#define __ID3D12CommandList_INTERFACE_DEFINED__

/* interface ID3D12CommandList */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12CommandList;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("168b7173-ba23-4bc8-9ff9-ee1db88afb20")
    ID3D12CommandList : public ID3D12DeviceChild
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( 
            /* [annotation] */ 
            _In_  ID3D12CommandAllocator *pAllocator,
            /* [annotation] */ 
            _In_opt_  ID3D12PipelineState *pInitialState) = 0;
        
        virtual void STDMETHODCALLTYPE ClearState( 
            /* [annotation] */ 
            _In_  ID3D12PipelineState *pPipelineState) = 0;
        
        virtual void STDMETHODCALLTYPE DrawInstanced( 
            /* [annotation] */ 
            _In_  UINT VertexCountPerInstance,
            /* [annotation] */ 
            _In_  UINT InstanceCount,
            /* [annotation] */ 
            _In_  UINT StartVertexLocation,
            /* [annotation] */ 
            _In_  UINT StartInstanceLocation) = 0;
        
        virtual void STDMETHODCALLTYPE DrawIndexedInstanced( 
            /* [annotation] */ 
            _In_  UINT IndexCountPerInstance,
            /* [annotation] */ 
            _In_  UINT InstanceCount,
            /* [annotation] */ 
            _In_  UINT StartIndexLocation,
            /* [annotation] */ 
            _In_  INT BaseVertexLocation,
            /* [annotation] */ 
            _In_  UINT StartInstanceLocation) = 0;
        
        virtual void STDMETHODCALLTYPE DrawInstancedIndirect( 
            /* [annotation] */ 
            _In_  ID3D12Resource *pBufferForArgs,
            /* [annotation] */ 
            _In_  UINT AlignedByteOffsetForArgs) = 0;
        
        virtual void STDMETHODCALLTYPE DrawIndexedInstancedIndirect( 
            /* [annotation] */ 
            _In_  ID3D12Resource *pBufferForArgs,
            /* [annotation] */ 
            _In_  UINT AlignedByteOffsetForArgs) = 0;
        
        virtual void STDMETHODCALLTYPE DrawAuto( void) = 0;
        
        virtual void STDMETHODCALLTYPE Dispatch( 
            /* [annotation] */ 
            _In_  UINT ThreadGroupCountX,
            /* [annotation] */ 
            _In_  UINT ThreadGroupCountY,
            /* [annotation] */ 
            _In_  UINT ThreadGroupCountZ) = 0;
        
        virtual void STDMETHODCALLTYPE DispatchIndirect( 
            /* [annotation] */ 
            _In_  ID3D12Resource *pBufferForArgs,
            /* [annotation] */ 
            _In_  UINT AlignedByteOffsetForArgs) = 0;
        
        virtual void STDMETHODCALLTYPE CopySubresourceRegion( 
            /* [annotation] */ 
            _In_  ID3D12Resource *pDstResource,
            D3D12_SUBRESOURCE_VIEW_TYPE DstViewType,
            /* [annotation] */ 
            _In_  const void *pDstViewDesc,
            UINT DstX,
            UINT DstY,
            UINT DstZ,
            /* [annotation] */ 
            _In_  ID3D12Resource *pSrcResource,
            D3D12_SUBRESOURCE_VIEW_TYPE SrcViewType,
            /* [annotation] */ 
            _In_  const void *pSrcViewDesc,
            /* [annotation] */ 
            _In_opt_  const D3D12_BOX *pSrcBox,
            D3D12_COPY_FLAGS CopyFlags) = 0;
        
        virtual void STDMETHODCALLTYPE CopyResource( 
            /* [annotation] */ 
            _In_  ID3D12Resource *pDstResource,
            /* [annotation] */ 
            _In_  ID3D12Resource *pSrcResource) = 0;
        
        virtual void STDMETHODCALLTYPE CopyTiles( 
            /* [annotation] */ 
            _In_  ID3D12Resource *pTiledResource,
            /* [annotation] */ 
            _In_  const D3D12_TILED_RESOURCE_COORDINATE *pTileRegionStartCoordinate,
            /* [annotation] */ 
            _In_  const D3D12_TILE_REGION_SIZE *pTileRegionSize,
            /* [annotation] */ 
            _In_  ID3D12Resource *pBuffer,
            UINT64 BufferStartOffsetInBytes,
            D3D12_TILE_COPY_FLAG Flags) = 0;
        
        virtual void STDMETHODCALLTYPE ResolveSubresource( 
            /* [annotation] */ 
            _In_  ID3D12Resource *pDstResource,
            /* [annotation] */ 
            _In_  UINT DstSubresource,
            /* [annotation] */ 
            _In_  ID3D12Resource *pSrcResource,
            /* [annotation] */ 
            _In_  UINT SrcSubresource,
            /* [annotation] */ 
            _In_  DXGI_FORMAT Format) = 0;
        
        virtual void STDMETHODCALLTYPE IASetPrimitiveTopology( 
            /* [annotation] */ 
            _In_  D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology) = 0;
        
        virtual void STDMETHODCALLTYPE RSSetViewports( 
            /* [annotation] */ 
            _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT Count,
            /* [annotation] */ 
            _In_reads_( Count)  const D3D12_VIEWPORT *pViewports) = 0;
        
        virtual void STDMETHODCALLTYPE RSSetScissorRects( 
            /* [annotation] */ 
            _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT Count,
            /* [annotation] */ 
            _In_reads_( Count)  const D3D12_RECT *pRects) = 0;
        
        virtual void STDMETHODCALLTYPE OMSetBlendFactor( 
            /* [annotation] */ 
            _In_opt_  const FLOAT BlendFactor[ 4 ]) = 0;
        
        virtual void STDMETHODCALLTYPE OMSetStencilRef( 
            /* [annotation] */ 
            _In_  UINT StencilRef) = 0;
        
        virtual void STDMETHODCALLTYPE SetPipelineState( 
            /* [annotation] */ 
            _In_  ID3D12PipelineState *pPipelineState) = 0;
        
        virtual void STDMETHODCALLTYPE ResourceBarrier( 
            /* [annotation] */ 
            _In_  UINT Count,
            /* [annotation] */ 
            _In_reads_(Count)  const D3D12_RESOURCE_BARRIER_DESC *pDesc) = 0;
        
        virtual void STDMETHODCALLTYPE ExecuteBundle( 
            /* [annotation] */ 
            _In_  ID3D12CommandList *pCommandList) = 0;
        
        virtual void STDMETHODCALLTYPE BeginQuery( 
            /* [annotation] */ 
            _In_  ID3D11Asynchronous *pAsync) = 0;
        
        virtual void STDMETHODCALLTYPE EndQuery( 
            /* [annotation] */ 
            _In_  ID3D11Asynchronous *pAsync) = 0;
        
        virtual void STDMETHODCALLTYPE SetPredication( 
            /* [annotation] */ 
            _In_opt_  ID3D11Predicate *pPredicate,
            /* [annotation] */ 
            _In_  BOOL PredicateValue) = 0;
        
        virtual void STDMETHODCALLTYPE SetDescriptorHeaps( 
            /* [annotation] */ 
            _In_reads_(NumDescriptorHeaps)  ID3D12DescriptorHeap **pDescriptorHeaps,
            /* [annotation] */ 
            _In_  UINT NumDescriptorHeaps) = 0;
        
        virtual void STDMETHODCALLTYPE SetComputeRootSignature( 
            /* [annotation] */ 
            _In_  ID3D12RootSignature *pRootSignature) = 0;
        
        virtual void STDMETHODCALLTYPE SetGraphicsRootSignature( 
            /* [annotation] */ 
            _In_  ID3D12RootSignature *pRootSignature) = 0;
        
        virtual void STDMETHODCALLTYPE SetComputeRootDescriptorTable( 
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor) = 0;
        
        virtual void STDMETHODCALLTYPE SetGraphicsRootDescriptorTable( 
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor) = 0;
        
        virtual void STDMETHODCALLTYPE SetComputeRoot32BitConstant( 
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  UINT SrcData,
            /* [annotation] */ 
            _In_  UINT DestOffsetIn32BitValues) = 0;
        
        virtual void STDMETHODCALLTYPE SetGraphicsRoot32BitConstant( 
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  UINT SrcData,
            /* [annotation] */ 
            _In_  UINT DestOffsetIn32BitValues) = 0;
        
        virtual void STDMETHODCALLTYPE SetComputeRoot32BitConstants( 
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_reads_(Num32BitValuesToSet*sizeof(UINT))  const void *pSrcData,
            /* [annotation] */ 
            _In_  UINT DestOffsetIn32BitValues,
            /* [annotation] */ 
            _In_  UINT Num32BitValuesToSet) = 0;
        
        virtual void STDMETHODCALLTYPE SetGraphicsRoot32BitConstants( 
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_reads_(Num32BitValuesToSet*sizeof(UINT))  const void *pSrcData,
            /* [annotation] */ 
            _In_  UINT DestOffsetIn32BitValues,
            /* [annotation] */ 
            _In_  UINT Num32BitValuesToSet) = 0;
        
        virtual void STDMETHODCALLTYPE SetComputeRootConstantBufferViewSingleUse( 
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pBuffer,
            /* [annotation] */ 
            _In_  const D3D12_CONSTANT_BUFFER_VIEW_DESC *pDesc) = 0;
        
        virtual void STDMETHODCALLTYPE SetGraphicsRootConstantBufferViewSingleUse( 
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pBuffer,
            /* [annotation] */ 
            _In_  const D3D12_CONSTANT_BUFFER_VIEW_DESC *pDesc) = 0;
        
        virtual void STDMETHODCALLTYPE SetComputeRootConstantBufferView( 
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle) = 0;
        
        virtual void STDMETHODCALLTYPE SetGraphicsRootConstantBufferView( 
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle) = 0;
        
        virtual void STDMETHODCALLTYPE SetComputeRootShaderResourceViewSingleUse( 
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_  const D3D12_SHADER_RESOURCE_VIEW_DESC *pDesc) = 0;
        
        virtual void STDMETHODCALLTYPE SetGraphicsRootShaderResourceViewSingleUse( 
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_  const D3D12_SHADER_RESOURCE_VIEW_DESC *pDesc) = 0;
        
        virtual void STDMETHODCALLTYPE SetComputeRootShaderResourceView( 
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle) = 0;
        
        virtual void STDMETHODCALLTYPE SetGraphicsRootShaderResourceView( 
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle) = 0;
        
        virtual void STDMETHODCALLTYPE SetComputeRootUnorderedAccessViewSingleUse( 
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_  const D3D12_UNORDERED_ACCESS_VIEW_DESC *pDesc) = 0;
        
        virtual void STDMETHODCALLTYPE SetGraphicsRootUnorderedAccessViewSingleUse( 
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_  const D3D12_UNORDERED_ACCESS_VIEW_DESC *pDesc) = 0;
        
        virtual void STDMETHODCALLTYPE SetComputeRootUnorderedAccessView( 
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle) = 0;
        
        virtual void STDMETHODCALLTYPE SetGraphicsRootUnorderedAccessView( 
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle) = 0;
        
        virtual void STDMETHODCALLTYPE SetIndexBuffer( 
            /* [annotation] */ 
            _In_opt_  const D3D12_CPU_DESCRIPTOR_HANDLE *pDescriptor) = 0;
        
        virtual void STDMETHODCALLTYPE SetIndexBufferSingleUse( 
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pBuffer,
            /* [annotation] */ 
            _In_  const D3D12_INDEX_BUFFER_VIEW_DESC *pDesc) = 0;
        
        virtual void STDMETHODCALLTYPE SetVertexBuffers( 
            /* [annotation] */ 
            _In_  UINT StartSlot,
            /* [annotation] */ 
            _In_  const D3D12_CPU_DESCRIPTOR_HANDLE *pDescriptors,
            /* [annotation] */ 
            _In_  BOOL SingleHandleToDescriptorRange,
            /* [annotation] */ 
            _In_  UINT NumDescriptors) = 0;
        
        virtual void STDMETHODCALLTYPE SetVertexBuffersSingleUse( 
            /* [annotation] */ 
            _In_  UINT StartSlot,
            /* [annotation] */ 
            _In_reads_(NumBuffers)  ID3D12Resource *const *ppBuffers,
            /* [annotation] */ 
            _In_reads_(NumBuffers)  const D3D12_VERTEX_BUFFER_VIEW_DESC *ppDescs,
            /* [annotation] */ 
            _In_  UINT NumBuffers) = 0;
        
        virtual void STDMETHODCALLTYPE SetStreamOutputBuffers( 
            /* [annotation] */ 
            _In_  UINT StartSlot,
            /* [annotation] */ 
            _In_  const D3D12_CPU_DESCRIPTOR_HANDLE *pDescriptors,
            /* [annotation] */ 
            _In_  BOOL SingleHandleToDescriptorRange,
            /* [annotation] */ 
            _In_  UINT NumDescriptors) = 0;
        
        virtual void STDMETHODCALLTYPE SetStreamOutputBuffersSingleUse( 
            /* [annotation] */ 
            _In_  UINT StartSlot,
            /* [annotation] */ 
            _In_reads_(NumBuffers)  ID3D12Resource *const *ppBuffers,
            /* [annotation] */ 
            _In_reads_(NumBuffers)  const D3D12_STREAM_OUTPUT_VIEW_DESC *pDescs,
            /* [annotation] */ 
            _In_  UINT NumBuffers) = 0;
        
        virtual void STDMETHODCALLTYPE SetRenderTargets( 
            /* [annotation] */ 
            _In_  const D3D12_CPU_DESCRIPTOR_HANDLE *pRenderTargetDescriptors,
            /* [annotation] */ 
            _In_  BOOL RTsSingleHandleToDescriptorRange,
            /* [annotation] */ 
            _In_  UINT NumRenderTargetDescriptors,
            /* [annotation] */ 
            _In_opt_  const D3D12_CPU_DESCRIPTOR_HANDLE *pDepthStencilDescriptor) = 0;
        
        virtual void STDMETHODCALLTYPE SetStreamOutputBufferOffset( 
            /* [annotation] */ 
            _In_  ID3D11Query *pSOOffset,
            /* [annotation] */ 
            _In_  UINT64 BufferOffsetInBytes) = 0;
        
        virtual void STDMETHODCALLTYPE SetUnorderedAccessViewCounterValue( 
            /* [annotation] */ 
            _In_  ID3D11Query *pUAVCounter,
            /* [annotation] */ 
            _In_  UINT CounterValue) = 0;
        
        virtual void STDMETHODCALLTYPE ClearDepthStencilView( 
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView,
            /* [annotation] */ 
            _In_  UINT ClearFlags,
            /* [annotation] */ 
            _In_  FLOAT Depth,
            /* [annotation] */ 
            _In_  UINT8 Stencil,
            /* [annotation] */ 
            _In_reads_opt_(NumRects)  const D3D12_RECT *pRect,
            /* [annotation] */ 
            _In_  UINT NumRects) = 0;
        
        virtual void STDMETHODCALLTYPE ClearRenderTargetView( 
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetView,
            /* [annotation] */ 
            _In_  const FLOAT ColorRGBA[ 4 ],
            /* [annotation] */ 
            _In_reads_opt_(NumRects)  const D3D12_RECT *pRects,
            /* [annotation] */ 
            _In_  UINT NumRects) = 0;
        
        virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewUint( 
            /* [annotation] */ 
            _In_  D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle,
            /* [annotation] */ 
            _In_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_  const UINT Values[ 4 ],
            /* [annotation] */ 
            _In_reads_opt_(NumRects)  const D3D12_RECT *pRects,
            /* [annotation] */ 
            _In_  UINT NumRects) = 0;
        
        virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewFloat( 
            /* [annotation] */ 
            _In_  D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle,
            /* [annotation] */ 
            _In_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_  const FLOAT Values[ 4 ],
            /* [annotation] */ 
            _In_reads_opt_(NumRects)  const D3D12_RECT *pRects,
            /* [annotation] */ 
            _In_  UINT NumRects) = 0;
        
        virtual void STDMETHODCALLTYPE CopyStructureCount( 
            /* [annotation] */ 
            _In_  ID3D12Resource *pDstBuffer,
            /* [annotation] */ 
            _In_  UINT DstAlignedByteOffset,
            /* [annotation] */ 
            _In_  ID3D11Query *pUAVCounter) = 0;
        
        virtual void STDMETHODCALLTYPE DiscardResource( 
            /* [annotation] */ 
            _In_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_opt_  const D3D12_DISCARD_RESOURCE_DESC *pDesc) = 0;
        
        virtual INT STDMETHODCALLTYPE PIXBeginEvent( 
            /* [annotation] */ 
            _In_z_  LPCWSTR Name) = 0;
        
        virtual INT STDMETHODCALLTYPE PIXEndEvent( void) = 0;
        
        virtual void STDMETHODCALLTYPE PIXSetMarker( 
            /* [annotation] */ 
            _In_z_  LPCWSTR Name) = 0;
        
        virtual BOOL STDMETHODCALLTYPE PIXGetStatus( void) = 0;
        
        virtual void STDMETHODCALLTYPE SetMarkerInt( 
            /* [annotation] */ 
            _In_  LPCWSTR pLabel,
            INT Data) = 0;
        
        virtual void STDMETHODCALLTYPE BeginEventInt( 
            /* [annotation] */ 
            _In_  LPCWSTR pLabel,
            INT Data) = 0;
        
        virtual void STDMETHODCALLTYPE EndEvent( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12CommandListVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12CommandList * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12CommandList * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12CommandList * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetPrivateData )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _Inout_  UINT *pDataSize,
            /* [annotation] */ 
            _Out_writes_bytes_opt_( *pDataSize )  void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateData )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_  UINT DataSize,
            /* [annotation] */ 
            _In_reads_bytes_opt_( DataSize )  const void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_opt_  const IUnknown *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetName )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR pName);
        
        void ( STDMETHODCALLTYPE *GetDevice )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _Out_  ID3D12Device **ppDevice);
        
        HRESULT ( STDMETHODCALLTYPE *Close )( 
            ID3D12CommandList * This);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D12CommandAllocator *pAllocator,
            /* [annotation] */ 
            _In_opt_  ID3D12PipelineState *pInitialState);
        
        void ( STDMETHODCALLTYPE *ClearState )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D12PipelineState *pPipelineState);
        
        void ( STDMETHODCALLTYPE *DrawInstanced )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT VertexCountPerInstance,
            /* [annotation] */ 
            _In_  UINT InstanceCount,
            /* [annotation] */ 
            _In_  UINT StartVertexLocation,
            /* [annotation] */ 
            _In_  UINT StartInstanceLocation);
        
        void ( STDMETHODCALLTYPE *DrawIndexedInstanced )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT IndexCountPerInstance,
            /* [annotation] */ 
            _In_  UINT InstanceCount,
            /* [annotation] */ 
            _In_  UINT StartIndexLocation,
            /* [annotation] */ 
            _In_  INT BaseVertexLocation,
            /* [annotation] */ 
            _In_  UINT StartInstanceLocation);
        
        void ( STDMETHODCALLTYPE *DrawInstancedIndirect )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D12Resource *pBufferForArgs,
            /* [annotation] */ 
            _In_  UINT AlignedByteOffsetForArgs);
        
        void ( STDMETHODCALLTYPE *DrawIndexedInstancedIndirect )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D12Resource *pBufferForArgs,
            /* [annotation] */ 
            _In_  UINT AlignedByteOffsetForArgs);
        
        void ( STDMETHODCALLTYPE *DrawAuto )( 
            ID3D12CommandList * This);
        
        void ( STDMETHODCALLTYPE *Dispatch )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT ThreadGroupCountX,
            /* [annotation] */ 
            _In_  UINT ThreadGroupCountY,
            /* [annotation] */ 
            _In_  UINT ThreadGroupCountZ);
        
        void ( STDMETHODCALLTYPE *DispatchIndirect )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D12Resource *pBufferForArgs,
            /* [annotation] */ 
            _In_  UINT AlignedByteOffsetForArgs);
        
        void ( STDMETHODCALLTYPE *CopySubresourceRegion )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D12Resource *pDstResource,
            D3D12_SUBRESOURCE_VIEW_TYPE DstViewType,
            /* [annotation] */ 
            _In_  const void *pDstViewDesc,
            UINT DstX,
            UINT DstY,
            UINT DstZ,
            /* [annotation] */ 
            _In_  ID3D12Resource *pSrcResource,
            D3D12_SUBRESOURCE_VIEW_TYPE SrcViewType,
            /* [annotation] */ 
            _In_  const void *pSrcViewDesc,
            /* [annotation] */ 
            _In_opt_  const D3D12_BOX *pSrcBox,
            D3D12_COPY_FLAGS CopyFlags);
        
        void ( STDMETHODCALLTYPE *CopyResource )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D12Resource *pDstResource,
            /* [annotation] */ 
            _In_  ID3D12Resource *pSrcResource);
        
        void ( STDMETHODCALLTYPE *CopyTiles )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D12Resource *pTiledResource,
            /* [annotation] */ 
            _In_  const D3D12_TILED_RESOURCE_COORDINATE *pTileRegionStartCoordinate,
            /* [annotation] */ 
            _In_  const D3D12_TILE_REGION_SIZE *pTileRegionSize,
            /* [annotation] */ 
            _In_  ID3D12Resource *pBuffer,
            UINT64 BufferStartOffsetInBytes,
            D3D12_TILE_COPY_FLAG Flags);
        
        void ( STDMETHODCALLTYPE *ResolveSubresource )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D12Resource *pDstResource,
            /* [annotation] */ 
            _In_  UINT DstSubresource,
            /* [annotation] */ 
            _In_  ID3D12Resource *pSrcResource,
            /* [annotation] */ 
            _In_  UINT SrcSubresource,
            /* [annotation] */ 
            _In_  DXGI_FORMAT Format);
        
        void ( STDMETHODCALLTYPE *IASetPrimitiveTopology )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology);
        
        void ( STDMETHODCALLTYPE *RSSetViewports )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT Count,
            /* [annotation] */ 
            _In_reads_( Count)  const D3D12_VIEWPORT *pViewports);
        
        void ( STDMETHODCALLTYPE *RSSetScissorRects )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT Count,
            /* [annotation] */ 
            _In_reads_( Count)  const D3D12_RECT *pRects);
        
        void ( STDMETHODCALLTYPE *OMSetBlendFactor )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_opt_  const FLOAT BlendFactor[ 4 ]);
        
        void ( STDMETHODCALLTYPE *OMSetStencilRef )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT StencilRef);
        
        void ( STDMETHODCALLTYPE *SetPipelineState )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D12PipelineState *pPipelineState);
        
        void ( STDMETHODCALLTYPE *ResourceBarrier )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT Count,
            /* [annotation] */ 
            _In_reads_(Count)  const D3D12_RESOURCE_BARRIER_DESC *pDesc);
        
        void ( STDMETHODCALLTYPE *ExecuteBundle )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D12CommandList *pCommandList);
        
        void ( STDMETHODCALLTYPE *BeginQuery )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D11Asynchronous *pAsync);
        
        void ( STDMETHODCALLTYPE *EndQuery )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D11Asynchronous *pAsync);
        
        void ( STDMETHODCALLTYPE *SetPredication )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_opt_  ID3D11Predicate *pPredicate,
            /* [annotation] */ 
            _In_  BOOL PredicateValue);
        
        void ( STDMETHODCALLTYPE *SetDescriptorHeaps )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_reads_(NumDescriptorHeaps)  ID3D12DescriptorHeap **pDescriptorHeaps,
            /* [annotation] */ 
            _In_  UINT NumDescriptorHeaps);
        
        void ( STDMETHODCALLTYPE *SetComputeRootSignature )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D12RootSignature *pRootSignature);
        
        void ( STDMETHODCALLTYPE *SetGraphicsRootSignature )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D12RootSignature *pRootSignature);
        
        void ( STDMETHODCALLTYPE *SetComputeRootDescriptorTable )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor);
        
        void ( STDMETHODCALLTYPE *SetGraphicsRootDescriptorTable )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor);
        
        void ( STDMETHODCALLTYPE *SetComputeRoot32BitConstant )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  UINT SrcData,
            /* [annotation] */ 
            _In_  UINT DestOffsetIn32BitValues);
        
        void ( STDMETHODCALLTYPE *SetGraphicsRoot32BitConstant )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  UINT SrcData,
            /* [annotation] */ 
            _In_  UINT DestOffsetIn32BitValues);
        
        void ( STDMETHODCALLTYPE *SetComputeRoot32BitConstants )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_reads_(Num32BitValuesToSet*sizeof(UINT))  const void *pSrcData,
            /* [annotation] */ 
            _In_  UINT DestOffsetIn32BitValues,
            /* [annotation] */ 
            _In_  UINT Num32BitValuesToSet);
        
        void ( STDMETHODCALLTYPE *SetGraphicsRoot32BitConstants )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_reads_(Num32BitValuesToSet*sizeof(UINT))  const void *pSrcData,
            /* [annotation] */ 
            _In_  UINT DestOffsetIn32BitValues,
            /* [annotation] */ 
            _In_  UINT Num32BitValuesToSet);
        
        void ( STDMETHODCALLTYPE *SetComputeRootConstantBufferViewSingleUse )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pBuffer,
            /* [annotation] */ 
            _In_  const D3D12_CONSTANT_BUFFER_VIEW_DESC *pDesc);
        
        void ( STDMETHODCALLTYPE *SetGraphicsRootConstantBufferViewSingleUse )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pBuffer,
            /* [annotation] */ 
            _In_  const D3D12_CONSTANT_BUFFER_VIEW_DESC *pDesc);
        
        void ( STDMETHODCALLTYPE *SetComputeRootConstantBufferView )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle);
        
        void ( STDMETHODCALLTYPE *SetGraphicsRootConstantBufferView )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle);
        
        void ( STDMETHODCALLTYPE *SetComputeRootShaderResourceViewSingleUse )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_  const D3D12_SHADER_RESOURCE_VIEW_DESC *pDesc);
        
        void ( STDMETHODCALLTYPE *SetGraphicsRootShaderResourceViewSingleUse )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_  const D3D12_SHADER_RESOURCE_VIEW_DESC *pDesc);
        
        void ( STDMETHODCALLTYPE *SetComputeRootShaderResourceView )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle);
        
        void ( STDMETHODCALLTYPE *SetGraphicsRootShaderResourceView )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle);
        
        void ( STDMETHODCALLTYPE *SetComputeRootUnorderedAccessViewSingleUse )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_  const D3D12_UNORDERED_ACCESS_VIEW_DESC *pDesc);
        
        void ( STDMETHODCALLTYPE *SetGraphicsRootUnorderedAccessViewSingleUse )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_  const D3D12_UNORDERED_ACCESS_VIEW_DESC *pDesc);
        
        void ( STDMETHODCALLTYPE *SetComputeRootUnorderedAccessView )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle);
        
        void ( STDMETHODCALLTYPE *SetGraphicsRootUnorderedAccessView )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT RootParameterIndex,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle);
        
        void ( STDMETHODCALLTYPE *SetIndexBuffer )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_opt_  const D3D12_CPU_DESCRIPTOR_HANDLE *pDescriptor);
        
        void ( STDMETHODCALLTYPE *SetIndexBufferSingleUse )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pBuffer,
            /* [annotation] */ 
            _In_  const D3D12_INDEX_BUFFER_VIEW_DESC *pDesc);
        
        void ( STDMETHODCALLTYPE *SetVertexBuffers )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT StartSlot,
            /* [annotation] */ 
            _In_  const D3D12_CPU_DESCRIPTOR_HANDLE *pDescriptors,
            /* [annotation] */ 
            _In_  BOOL SingleHandleToDescriptorRange,
            /* [annotation] */ 
            _In_  UINT NumDescriptors);
        
        void ( STDMETHODCALLTYPE *SetVertexBuffersSingleUse )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT StartSlot,
            /* [annotation] */ 
            _In_reads_(NumBuffers)  ID3D12Resource *const *ppBuffers,
            /* [annotation] */ 
            _In_reads_(NumBuffers)  const D3D12_VERTEX_BUFFER_VIEW_DESC *ppDescs,
            /* [annotation] */ 
            _In_  UINT NumBuffers);
        
        void ( STDMETHODCALLTYPE *SetStreamOutputBuffers )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT StartSlot,
            /* [annotation] */ 
            _In_  const D3D12_CPU_DESCRIPTOR_HANDLE *pDescriptors,
            /* [annotation] */ 
            _In_  BOOL SingleHandleToDescriptorRange,
            /* [annotation] */ 
            _In_  UINT NumDescriptors);
        
        void ( STDMETHODCALLTYPE *SetStreamOutputBuffersSingleUse )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  UINT StartSlot,
            /* [annotation] */ 
            _In_reads_(NumBuffers)  ID3D12Resource *const *ppBuffers,
            /* [annotation] */ 
            _In_reads_(NumBuffers)  const D3D12_STREAM_OUTPUT_VIEW_DESC *pDescs,
            /* [annotation] */ 
            _In_  UINT NumBuffers);
        
        void ( STDMETHODCALLTYPE *SetRenderTargets )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  const D3D12_CPU_DESCRIPTOR_HANDLE *pRenderTargetDescriptors,
            /* [annotation] */ 
            _In_  BOOL RTsSingleHandleToDescriptorRange,
            /* [annotation] */ 
            _In_  UINT NumRenderTargetDescriptors,
            /* [annotation] */ 
            _In_opt_  const D3D12_CPU_DESCRIPTOR_HANDLE *pDepthStencilDescriptor);
        
        void ( STDMETHODCALLTYPE *SetStreamOutputBufferOffset )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D11Query *pSOOffset,
            /* [annotation] */ 
            _In_  UINT64 BufferOffsetInBytes);
        
        void ( STDMETHODCALLTYPE *SetUnorderedAccessViewCounterValue )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D11Query *pUAVCounter,
            /* [annotation] */ 
            _In_  UINT CounterValue);
        
        void ( STDMETHODCALLTYPE *ClearDepthStencilView )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView,
            /* [annotation] */ 
            _In_  UINT ClearFlags,
            /* [annotation] */ 
            _In_  FLOAT Depth,
            /* [annotation] */ 
            _In_  UINT8 Stencil,
            /* [annotation] */ 
            _In_reads_opt_(NumRects)  const D3D12_RECT *pRect,
            /* [annotation] */ 
            _In_  UINT NumRects);
        
        void ( STDMETHODCALLTYPE *ClearRenderTargetView )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetView,
            /* [annotation] */ 
            _In_  const FLOAT ColorRGBA[ 4 ],
            /* [annotation] */ 
            _In_reads_opt_(NumRects)  const D3D12_RECT *pRects,
            /* [annotation] */ 
            _In_  UINT NumRects);
        
        void ( STDMETHODCALLTYPE *ClearUnorderedAccessViewUint )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle,
            /* [annotation] */ 
            _In_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_  const UINT Values[ 4 ],
            /* [annotation] */ 
            _In_reads_opt_(NumRects)  const D3D12_RECT *pRects,
            /* [annotation] */ 
            _In_  UINT NumRects);
        
        void ( STDMETHODCALLTYPE *ClearUnorderedAccessViewFloat )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle,
            /* [annotation] */ 
            _In_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_  const FLOAT Values[ 4 ],
            /* [annotation] */ 
            _In_reads_opt_(NumRects)  const D3D12_RECT *pRects,
            /* [annotation] */ 
            _In_  UINT NumRects);
        
        void ( STDMETHODCALLTYPE *CopyStructureCount )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D12Resource *pDstBuffer,
            /* [annotation] */ 
            _In_  UINT DstAlignedByteOffset,
            /* [annotation] */ 
            _In_  ID3D11Query *pUAVCounter);
        
        void ( STDMETHODCALLTYPE *DiscardResource )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_opt_  const D3D12_DISCARD_RESOURCE_DESC *pDesc);
        
        INT ( STDMETHODCALLTYPE *PIXBeginEvent )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR Name);
        
        INT ( STDMETHODCALLTYPE *PIXEndEvent )( 
            ID3D12CommandList * This);
        
        void ( STDMETHODCALLTYPE *PIXSetMarker )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR Name);
        
        BOOL ( STDMETHODCALLTYPE *PIXGetStatus )( 
            ID3D12CommandList * This);
        
        void ( STDMETHODCALLTYPE *SetMarkerInt )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  LPCWSTR pLabel,
            INT Data);
        
        void ( STDMETHODCALLTYPE *BeginEventInt )( 
            ID3D12CommandList * This,
            /* [annotation] */ 
            _In_  LPCWSTR pLabel,
            INT Data);
        
        void ( STDMETHODCALLTYPE *EndEvent )( 
            ID3D12CommandList * This);
        
        END_INTERFACE
    } ID3D12CommandListVtbl;

    interface ID3D12CommandList
    {
        CONST_VTBL struct ID3D12CommandListVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12CommandList_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12CommandList_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12CommandList_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12CommandList_GetPrivateData(This,guid,pDataSize,pData)	\
    ( (This)->lpVtbl -> GetPrivateData(This,guid,pDataSize,pData) ) 

#define ID3D12CommandList_SetPrivateData(This,guid,DataSize,pData)	\
    ( (This)->lpVtbl -> SetPrivateData(This,guid,DataSize,pData) ) 

#define ID3D12CommandList_SetPrivateDataInterface(This,guid,pData)	\
    ( (This)->lpVtbl -> SetPrivateDataInterface(This,guid,pData) ) 

#define ID3D12CommandList_SetName(This,pName)	\
    ( (This)->lpVtbl -> SetName(This,pName) ) 


#define ID3D12CommandList_GetDevice(This,ppDevice)	\
    ( (This)->lpVtbl -> GetDevice(This,ppDevice) ) 


#define ID3D12CommandList_Close(This)	\
    ( (This)->lpVtbl -> Close(This) ) 

#define ID3D12CommandList_Reset(This,pAllocator,pInitialState)	\
    ( (This)->lpVtbl -> Reset(This,pAllocator,pInitialState) ) 

#define ID3D12CommandList_ClearState(This,pPipelineState)	\
    ( (This)->lpVtbl -> ClearState(This,pPipelineState) ) 

#define ID3D12CommandList_DrawInstanced(This,VertexCountPerInstance,InstanceCount,StartVertexLocation,StartInstanceLocation)	\
    ( (This)->lpVtbl -> DrawInstanced(This,VertexCountPerInstance,InstanceCount,StartVertexLocation,StartInstanceLocation) ) 

#define ID3D12CommandList_DrawIndexedInstanced(This,IndexCountPerInstance,InstanceCount,StartIndexLocation,BaseVertexLocation,StartInstanceLocation)	\
    ( (This)->lpVtbl -> DrawIndexedInstanced(This,IndexCountPerInstance,InstanceCount,StartIndexLocation,BaseVertexLocation,StartInstanceLocation) ) 

#define ID3D12CommandList_DrawInstancedIndirect(This,pBufferForArgs,AlignedByteOffsetForArgs)	\
    ( (This)->lpVtbl -> DrawInstancedIndirect(This,pBufferForArgs,AlignedByteOffsetForArgs) ) 

#define ID3D12CommandList_DrawIndexedInstancedIndirect(This,pBufferForArgs,AlignedByteOffsetForArgs)	\
    ( (This)->lpVtbl -> DrawIndexedInstancedIndirect(This,pBufferForArgs,AlignedByteOffsetForArgs) ) 

#define ID3D12CommandList_DrawAuto(This)	\
    ( (This)->lpVtbl -> DrawAuto(This) ) 

#define ID3D12CommandList_Dispatch(This,ThreadGroupCountX,ThreadGroupCountY,ThreadGroupCountZ)	\
    ( (This)->lpVtbl -> Dispatch(This,ThreadGroupCountX,ThreadGroupCountY,ThreadGroupCountZ) ) 

#define ID3D12CommandList_DispatchIndirect(This,pBufferForArgs,AlignedByteOffsetForArgs)	\
    ( (This)->lpVtbl -> DispatchIndirect(This,pBufferForArgs,AlignedByteOffsetForArgs) ) 

#define ID3D12CommandList_CopySubresourceRegion(This,pDstResource,DstViewType,pDstViewDesc,DstX,DstY,DstZ,pSrcResource,SrcViewType,pSrcViewDesc,pSrcBox,CopyFlags)	\
    ( (This)->lpVtbl -> CopySubresourceRegion(This,pDstResource,DstViewType,pDstViewDesc,DstX,DstY,DstZ,pSrcResource,SrcViewType,pSrcViewDesc,pSrcBox,CopyFlags) ) 

#define ID3D12CommandList_CopyResource(This,pDstResource,pSrcResource)	\
    ( (This)->lpVtbl -> CopyResource(This,pDstResource,pSrcResource) ) 

#define ID3D12CommandList_CopyTiles(This,pTiledResource,pTileRegionStartCoordinate,pTileRegionSize,pBuffer,BufferStartOffsetInBytes,Flags)	\
    ( (This)->lpVtbl -> CopyTiles(This,pTiledResource,pTileRegionStartCoordinate,pTileRegionSize,pBuffer,BufferStartOffsetInBytes,Flags) ) 

#define ID3D12CommandList_ResolveSubresource(This,pDstResource,DstSubresource,pSrcResource,SrcSubresource,Format)	\
    ( (This)->lpVtbl -> ResolveSubresource(This,pDstResource,DstSubresource,pSrcResource,SrcSubresource,Format) ) 

#define ID3D12CommandList_IASetPrimitiveTopology(This,PrimitiveTopology)	\
    ( (This)->lpVtbl -> IASetPrimitiveTopology(This,PrimitiveTopology) ) 

#define ID3D12CommandList_RSSetViewports(This,Count,pViewports)	\
    ( (This)->lpVtbl -> RSSetViewports(This,Count,pViewports) ) 

#define ID3D12CommandList_RSSetScissorRects(This,Count,pRects)	\
    ( (This)->lpVtbl -> RSSetScissorRects(This,Count,pRects) ) 

#define ID3D12CommandList_OMSetBlendFactor(This,BlendFactor)	\
    ( (This)->lpVtbl -> OMSetBlendFactor(This,BlendFactor) ) 

#define ID3D12CommandList_OMSetStencilRef(This,StencilRef)	\
    ( (This)->lpVtbl -> OMSetStencilRef(This,StencilRef) ) 

#define ID3D12CommandList_SetPipelineState(This,pPipelineState)	\
    ( (This)->lpVtbl -> SetPipelineState(This,pPipelineState) ) 

#define ID3D12CommandList_ResourceBarrier(This,Count,pDesc)	\
    ( (This)->lpVtbl -> ResourceBarrier(This,Count,pDesc) ) 

#define ID3D12CommandList_ExecuteBundle(This,pCommandList)	\
    ( (This)->lpVtbl -> ExecuteBundle(This,pCommandList) ) 

#define ID3D12CommandList_BeginQuery(This,pAsync)	\
    ( (This)->lpVtbl -> BeginQuery(This,pAsync) ) 

#define ID3D12CommandList_EndQuery(This,pAsync)	\
    ( (This)->lpVtbl -> EndQuery(This,pAsync) ) 

#define ID3D12CommandList_SetPredication(This,pPredicate,PredicateValue)	\
    ( (This)->lpVtbl -> SetPredication(This,pPredicate,PredicateValue) ) 

#define ID3D12CommandList_SetDescriptorHeaps(This,pDescriptorHeaps,NumDescriptorHeaps)	\
    ( (This)->lpVtbl -> SetDescriptorHeaps(This,pDescriptorHeaps,NumDescriptorHeaps) ) 

#define ID3D12CommandList_SetComputeRootSignature(This,pRootSignature)	\
    ( (This)->lpVtbl -> SetComputeRootSignature(This,pRootSignature) ) 

#define ID3D12CommandList_SetGraphicsRootSignature(This,pRootSignature)	\
    ( (This)->lpVtbl -> SetGraphicsRootSignature(This,pRootSignature) ) 

#define ID3D12CommandList_SetComputeRootDescriptorTable(This,RootParameterIndex,BaseDescriptor)	\
    ( (This)->lpVtbl -> SetComputeRootDescriptorTable(This,RootParameterIndex,BaseDescriptor) ) 

#define ID3D12CommandList_SetGraphicsRootDescriptorTable(This,RootParameterIndex,BaseDescriptor)	\
    ( (This)->lpVtbl -> SetGraphicsRootDescriptorTable(This,RootParameterIndex,BaseDescriptor) ) 

#define ID3D12CommandList_SetComputeRoot32BitConstant(This,RootParameterIndex,SrcData,DestOffsetIn32BitValues)	\
    ( (This)->lpVtbl -> SetComputeRoot32BitConstant(This,RootParameterIndex,SrcData,DestOffsetIn32BitValues) ) 

#define ID3D12CommandList_SetGraphicsRoot32BitConstant(This,RootParameterIndex,SrcData,DestOffsetIn32BitValues)	\
    ( (This)->lpVtbl -> SetGraphicsRoot32BitConstant(This,RootParameterIndex,SrcData,DestOffsetIn32BitValues) ) 

#define ID3D12CommandList_SetComputeRoot32BitConstants(This,RootParameterIndex,pSrcData,DestOffsetIn32BitValues,Num32BitValuesToSet)	\
    ( (This)->lpVtbl -> SetComputeRoot32BitConstants(This,RootParameterIndex,pSrcData,DestOffsetIn32BitValues,Num32BitValuesToSet) ) 

#define ID3D12CommandList_SetGraphicsRoot32BitConstants(This,RootParameterIndex,pSrcData,DestOffsetIn32BitValues,Num32BitValuesToSet)	\
    ( (This)->lpVtbl -> SetGraphicsRoot32BitConstants(This,RootParameterIndex,pSrcData,DestOffsetIn32BitValues,Num32BitValuesToSet) ) 

#define ID3D12CommandList_SetComputeRootConstantBufferViewSingleUse(This,RootParameterIndex,pBuffer,pDesc)	\
    ( (This)->lpVtbl -> SetComputeRootConstantBufferViewSingleUse(This,RootParameterIndex,pBuffer,pDesc) ) 

#define ID3D12CommandList_SetGraphicsRootConstantBufferViewSingleUse(This,RootParameterIndex,pBuffer,pDesc)	\
    ( (This)->lpVtbl -> SetGraphicsRootConstantBufferViewSingleUse(This,RootParameterIndex,pBuffer,pDesc) ) 

#define ID3D12CommandList_SetComputeRootConstantBufferView(This,RootParameterIndex,DescriptorHandle)	\
    ( (This)->lpVtbl -> SetComputeRootConstantBufferView(This,RootParameterIndex,DescriptorHandle) ) 

#define ID3D12CommandList_SetGraphicsRootConstantBufferView(This,RootParameterIndex,DescriptorHandle)	\
    ( (This)->lpVtbl -> SetGraphicsRootConstantBufferView(This,RootParameterIndex,DescriptorHandle) ) 

#define ID3D12CommandList_SetComputeRootShaderResourceViewSingleUse(This,RootParameterIndex,pResource,pDesc)	\
    ( (This)->lpVtbl -> SetComputeRootShaderResourceViewSingleUse(This,RootParameterIndex,pResource,pDesc) ) 

#define ID3D12CommandList_SetGraphicsRootShaderResourceViewSingleUse(This,RootParameterIndex,pResource,pDesc)	\
    ( (This)->lpVtbl -> SetGraphicsRootShaderResourceViewSingleUse(This,RootParameterIndex,pResource,pDesc) ) 

#define ID3D12CommandList_SetComputeRootShaderResourceView(This,RootParameterIndex,DescriptorHandle)	\
    ( (This)->lpVtbl -> SetComputeRootShaderResourceView(This,RootParameterIndex,DescriptorHandle) ) 

#define ID3D12CommandList_SetGraphicsRootShaderResourceView(This,RootParameterIndex,DescriptorHandle)	\
    ( (This)->lpVtbl -> SetGraphicsRootShaderResourceView(This,RootParameterIndex,DescriptorHandle) ) 

#define ID3D12CommandList_SetComputeRootUnorderedAccessViewSingleUse(This,RootParameterIndex,pResource,pDesc)	\
    ( (This)->lpVtbl -> SetComputeRootUnorderedAccessViewSingleUse(This,RootParameterIndex,pResource,pDesc) ) 

#define ID3D12CommandList_SetGraphicsRootUnorderedAccessViewSingleUse(This,RootParameterIndex,pResource,pDesc)	\
    ( (This)->lpVtbl -> SetGraphicsRootUnorderedAccessViewSingleUse(This,RootParameterIndex,pResource,pDesc) ) 

#define ID3D12CommandList_SetComputeRootUnorderedAccessView(This,RootParameterIndex,DescriptorHandle)	\
    ( (This)->lpVtbl -> SetComputeRootUnorderedAccessView(This,RootParameterIndex,DescriptorHandle) ) 

#define ID3D12CommandList_SetGraphicsRootUnorderedAccessView(This,RootParameterIndex,DescriptorHandle)	\
    ( (This)->lpVtbl -> SetGraphicsRootUnorderedAccessView(This,RootParameterIndex,DescriptorHandle) ) 

#define ID3D12CommandList_SetIndexBuffer(This,pDescriptor)	\
    ( (This)->lpVtbl -> SetIndexBuffer(This,pDescriptor) ) 

#define ID3D12CommandList_SetIndexBufferSingleUse(This,pBuffer,pDesc)	\
    ( (This)->lpVtbl -> SetIndexBufferSingleUse(This,pBuffer,pDesc) ) 

#define ID3D12CommandList_SetVertexBuffers(This,StartSlot,pDescriptors,SingleHandleToDescriptorRange,NumDescriptors)	\
    ( (This)->lpVtbl -> SetVertexBuffers(This,StartSlot,pDescriptors,SingleHandleToDescriptorRange,NumDescriptors) ) 

#define ID3D12CommandList_SetVertexBuffersSingleUse(This,StartSlot,ppBuffers,ppDescs,NumBuffers)	\
    ( (This)->lpVtbl -> SetVertexBuffersSingleUse(This,StartSlot,ppBuffers,ppDescs,NumBuffers) ) 

#define ID3D12CommandList_SetStreamOutputBuffers(This,StartSlot,pDescriptors,SingleHandleToDescriptorRange,NumDescriptors)	\
    ( (This)->lpVtbl -> SetStreamOutputBuffers(This,StartSlot,pDescriptors,SingleHandleToDescriptorRange,NumDescriptors) ) 

#define ID3D12CommandList_SetStreamOutputBuffersSingleUse(This,StartSlot,ppBuffers,pDescs,NumBuffers)	\
    ( (This)->lpVtbl -> SetStreamOutputBuffersSingleUse(This,StartSlot,ppBuffers,pDescs,NumBuffers) ) 

#define ID3D12CommandList_SetRenderTargets(This,pRenderTargetDescriptors,RTsSingleHandleToDescriptorRange,NumRenderTargetDescriptors,pDepthStencilDescriptor)	\
    ( (This)->lpVtbl -> SetRenderTargets(This,pRenderTargetDescriptors,RTsSingleHandleToDescriptorRange,NumRenderTargetDescriptors,pDepthStencilDescriptor) ) 

#define ID3D12CommandList_SetStreamOutputBufferOffset(This,pSOOffset,BufferOffsetInBytes)	\
    ( (This)->lpVtbl -> SetStreamOutputBufferOffset(This,pSOOffset,BufferOffsetInBytes) ) 

#define ID3D12CommandList_SetUnorderedAccessViewCounterValue(This,pUAVCounter,CounterValue)	\
    ( (This)->lpVtbl -> SetUnorderedAccessViewCounterValue(This,pUAVCounter,CounterValue) ) 

#define ID3D12CommandList_ClearDepthStencilView(This,DepthStencilView,ClearFlags,Depth,Stencil,pRect,NumRects)	\
    ( (This)->lpVtbl -> ClearDepthStencilView(This,DepthStencilView,ClearFlags,Depth,Stencil,pRect,NumRects) ) 

#define ID3D12CommandList_ClearRenderTargetView(This,RenderTargetView,ColorRGBA,pRects,NumRects)	\
    ( (This)->lpVtbl -> ClearRenderTargetView(This,RenderTargetView,ColorRGBA,pRects,NumRects) ) 

#define ID3D12CommandList_ClearUnorderedAccessViewUint(This,ViewGPUHandleInCurrentHeap,ViewCPUHandle,pResource,Values,pRects,NumRects)	\
    ( (This)->lpVtbl -> ClearUnorderedAccessViewUint(This,ViewGPUHandleInCurrentHeap,ViewCPUHandle,pResource,Values,pRects,NumRects) ) 

#define ID3D12CommandList_ClearUnorderedAccessViewFloat(This,ViewGPUHandleInCurrentHeap,ViewCPUHandle,pResource,Values,pRects,NumRects)	\
    ( (This)->lpVtbl -> ClearUnorderedAccessViewFloat(This,ViewGPUHandleInCurrentHeap,ViewCPUHandle,pResource,Values,pRects,NumRects) ) 

#define ID3D12CommandList_CopyStructureCount(This,pDstBuffer,DstAlignedByteOffset,pUAVCounter)	\
    ( (This)->lpVtbl -> CopyStructureCount(This,pDstBuffer,DstAlignedByteOffset,pUAVCounter) ) 

#define ID3D12CommandList_DiscardResource(This,pResource,pDesc)	\
    ( (This)->lpVtbl -> DiscardResource(This,pResource,pDesc) ) 

#define ID3D12CommandList_PIXBeginEvent(This,Name)	\
    ( (This)->lpVtbl -> PIXBeginEvent(This,Name) ) 

#define ID3D12CommandList_PIXEndEvent(This)	\
    ( (This)->lpVtbl -> PIXEndEvent(This) ) 

#define ID3D12CommandList_PIXSetMarker(This,Name)	\
    ( (This)->lpVtbl -> PIXSetMarker(This,Name) ) 

#define ID3D12CommandList_PIXGetStatus(This)	\
    ( (This)->lpVtbl -> PIXGetStatus(This) ) 

#define ID3D12CommandList_SetMarkerInt(This,pLabel,Data)	\
    ( (This)->lpVtbl -> SetMarkerInt(This,pLabel,Data) ) 

#define ID3D12CommandList_BeginEventInt(This,pLabel,Data)	\
    ( (This)->lpVtbl -> BeginEventInt(This,pLabel,Data) ) 

#define ID3D12CommandList_EndEvent(This)	\
    ( (This)->lpVtbl -> EndEvent(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ID3D12CommandList_INTERFACE_DEFINED__ */


#ifndef __ID3D12CommandQueue_INTERFACE_DEFINED__
#define __ID3D12CommandQueue_INTERFACE_DEFINED__

/* interface ID3D12CommandQueue */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12CommandQueue;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c93c8c3d-bf95-441a-b0cd-56ea3c3287f1")
    ID3D12CommandQueue : public ID3D12Pageable
    {
    public:
        virtual void STDMETHODCALLTYPE ExecuteCommandList( 
            /* [annotation] */ 
            _In_  ID3D12CommandList *pCommandList) = 0;
        
        virtual void STDMETHODCALLTYPE UpdateTileMappings( 
            /* [annotation] */ 
            _In_  ID3D12Resource *pResource,
            UINT NumResourceRegions,
            /* [annotation] */ 
            _In_reads_opt_(NumResourceRegions)  const D3D12_TILED_RESOURCE_COORDINATE *pResourceRegionStartCoordinates,
            /* [annotation] */ 
            _In_reads_opt_(NumResourceRegions)  const D3D12_TILE_REGION_SIZE *pResourceRegionSizes,
            /* [annotation] */ 
            _In_opt_  ID3D12Heap *pHeap,
            UINT NumRanges,
            /* [annotation] */ 
            _In_reads_opt_(NumRanges)  const D3D12_TILE_RANGE_FLAG *pRangeFlags,
            /* [annotation] */ 
            _In_reads_opt_(NumRanges)  const UINT *pHeapRangeStartOffsets,
            /* [annotation] */ 
            _In_reads_opt_(NumRanges)  const UINT *pRangeTileCounts,
            D3D12_TILE_MAPPING_FLAG Flags) = 0;
        
        virtual void STDMETHODCALLTYPE CopyTileMappings( 
            /* [annotation] */ 
            _In_  ID3D12Resource *pDstResource,
            /* [annotation] */ 
            _In_  const D3D12_TILED_RESOURCE_COORDINATE *pDstRegionStartCoordinate,
            /* [annotation] */ 
            _In_  ID3D12Resource *pSrcResource,
            /* [annotation] */ 
            _In_  const D3D12_TILED_RESOURCE_COORDINATE *pSrcRegionStartCoordinate,
            /* [annotation] */ 
            _In_  const D3D12_TILE_REGION_SIZE *pRegionSize,
            D3D12_TILE_MAPPING_FLAG Flags) = 0;
        
        virtual UINT64 STDMETHODCALLTYPE GetCurrentFence( void) = 0;
        
        virtual UINT64 STDMETHODCALLTYPE AdvanceFence( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetEventOnFenceCompletion( 
            UINT64 Value,
            HANDLE hEvent) = 0;
        
        virtual UINT64 STDMETHODCALLTYPE GetLastCompletedFence( void) = 0;
        
        virtual void STDMETHODCALLTYPE ExecuteCommandLists( 
            /* [annotation] */ 
            _In_  UINT Count,
            /* [annotation] */ 
            _In_reads_(Count)  ID3D12CommandList *const *ppCommandLists) = 0;
        
        virtual INT STDMETHODCALLTYPE PIXBeginEvent( 
            /* [annotation] */ 
            _In_z_  LPCWSTR Name) = 0;
        
        virtual INT STDMETHODCALLTYPE PIXEndEvent( void) = 0;
        
        virtual void STDMETHODCALLTYPE PIXSetMarker( 
            /* [annotation] */ 
            _In_z_  LPCWSTR Name) = 0;
        
        virtual BOOL STDMETHODCALLTYPE PIXGetStatus( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Signal( 
            ID3D12Fence *pFence,
            UINT64 Value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Wait( 
            ID3D12Fence *pFence,
            UINT64 Value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12CommandQueueVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12CommandQueue * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12CommandQueue * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12CommandQueue * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetPrivateData )( 
            ID3D12CommandQueue * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _Inout_  UINT *pDataSize,
            /* [annotation] */ 
            _Out_writes_bytes_opt_( *pDataSize )  void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateData )( 
            ID3D12CommandQueue * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_  UINT DataSize,
            /* [annotation] */ 
            _In_reads_bytes_opt_( DataSize )  const void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )( 
            ID3D12CommandQueue * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_opt_  const IUnknown *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetName )( 
            ID3D12CommandQueue * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR pName);
        
        void ( STDMETHODCALLTYPE *GetDevice )( 
            ID3D12CommandQueue * This,
            /* [annotation] */ 
            _Out_  ID3D12Device **ppDevice);
        
        void ( STDMETHODCALLTYPE *ExecuteCommandList )( 
            ID3D12CommandQueue * This,
            /* [annotation] */ 
            _In_  ID3D12CommandList *pCommandList);
        
        void ( STDMETHODCALLTYPE *UpdateTileMappings )( 
            ID3D12CommandQueue * This,
            /* [annotation] */ 
            _In_  ID3D12Resource *pResource,
            UINT NumResourceRegions,
            /* [annotation] */ 
            _In_reads_opt_(NumResourceRegions)  const D3D12_TILED_RESOURCE_COORDINATE *pResourceRegionStartCoordinates,
            /* [annotation] */ 
            _In_reads_opt_(NumResourceRegions)  const D3D12_TILE_REGION_SIZE *pResourceRegionSizes,
            /* [annotation] */ 
            _In_opt_  ID3D12Heap *pHeap,
            UINT NumRanges,
            /* [annotation] */ 
            _In_reads_opt_(NumRanges)  const D3D12_TILE_RANGE_FLAG *pRangeFlags,
            /* [annotation] */ 
            _In_reads_opt_(NumRanges)  const UINT *pHeapRangeStartOffsets,
            /* [annotation] */ 
            _In_reads_opt_(NumRanges)  const UINT *pRangeTileCounts,
            D3D12_TILE_MAPPING_FLAG Flags);
        
        void ( STDMETHODCALLTYPE *CopyTileMappings )( 
            ID3D12CommandQueue * This,
            /* [annotation] */ 
            _In_  ID3D12Resource *pDstResource,
            /* [annotation] */ 
            _In_  const D3D12_TILED_RESOURCE_COORDINATE *pDstRegionStartCoordinate,
            /* [annotation] */ 
            _In_  ID3D12Resource *pSrcResource,
            /* [annotation] */ 
            _In_  const D3D12_TILED_RESOURCE_COORDINATE *pSrcRegionStartCoordinate,
            /* [annotation] */ 
            _In_  const D3D12_TILE_REGION_SIZE *pRegionSize,
            D3D12_TILE_MAPPING_FLAG Flags);
        
        UINT64 ( STDMETHODCALLTYPE *GetCurrentFence )( 
            ID3D12CommandQueue * This);
        
        UINT64 ( STDMETHODCALLTYPE *AdvanceFence )( 
            ID3D12CommandQueue * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetEventOnFenceCompletion )( 
            ID3D12CommandQueue * This,
            UINT64 Value,
            HANDLE hEvent);
        
        UINT64 ( STDMETHODCALLTYPE *GetLastCompletedFence )( 
            ID3D12CommandQueue * This);
        
        void ( STDMETHODCALLTYPE *ExecuteCommandLists )( 
            ID3D12CommandQueue * This,
            /* [annotation] */ 
            _In_  UINT Count,
            /* [annotation] */ 
            _In_reads_(Count)  ID3D12CommandList *const *ppCommandLists);
        
        INT ( STDMETHODCALLTYPE *PIXBeginEvent )( 
            ID3D12CommandQueue * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR Name);
        
        INT ( STDMETHODCALLTYPE *PIXEndEvent )( 
            ID3D12CommandQueue * This);
        
        void ( STDMETHODCALLTYPE *PIXSetMarker )( 
            ID3D12CommandQueue * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR Name);
        
        BOOL ( STDMETHODCALLTYPE *PIXGetStatus )( 
            ID3D12CommandQueue * This);
        
        HRESULT ( STDMETHODCALLTYPE *Signal )( 
            ID3D12CommandQueue * This,
            ID3D12Fence *pFence,
            UINT64 Value);
        
        HRESULT ( STDMETHODCALLTYPE *Wait )( 
            ID3D12CommandQueue * This,
            ID3D12Fence *pFence,
            UINT64 Value);
        
        END_INTERFACE
    } ID3D12CommandQueueVtbl;

    interface ID3D12CommandQueue
    {
        CONST_VTBL struct ID3D12CommandQueueVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12CommandQueue_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12CommandQueue_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12CommandQueue_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12CommandQueue_GetPrivateData(This,guid,pDataSize,pData)	\
    ( (This)->lpVtbl -> GetPrivateData(This,guid,pDataSize,pData) ) 

#define ID3D12CommandQueue_SetPrivateData(This,guid,DataSize,pData)	\
    ( (This)->lpVtbl -> SetPrivateData(This,guid,DataSize,pData) ) 

#define ID3D12CommandQueue_SetPrivateDataInterface(This,guid,pData)	\
    ( (This)->lpVtbl -> SetPrivateDataInterface(This,guid,pData) ) 

#define ID3D12CommandQueue_SetName(This,pName)	\
    ( (This)->lpVtbl -> SetName(This,pName) ) 


#define ID3D12CommandQueue_GetDevice(This,ppDevice)	\
    ( (This)->lpVtbl -> GetDevice(This,ppDevice) ) 



#define ID3D12CommandQueue_ExecuteCommandList(This,pCommandList)	\
    ( (This)->lpVtbl -> ExecuteCommandList(This,pCommandList) ) 

#define ID3D12CommandQueue_UpdateTileMappings(This,pResource,NumResourceRegions,pResourceRegionStartCoordinates,pResourceRegionSizes,pHeap,NumRanges,pRangeFlags,pHeapRangeStartOffsets,pRangeTileCounts,Flags)	\
    ( (This)->lpVtbl -> UpdateTileMappings(This,pResource,NumResourceRegions,pResourceRegionStartCoordinates,pResourceRegionSizes,pHeap,NumRanges,pRangeFlags,pHeapRangeStartOffsets,pRangeTileCounts,Flags) ) 

#define ID3D12CommandQueue_CopyTileMappings(This,pDstResource,pDstRegionStartCoordinate,pSrcResource,pSrcRegionStartCoordinate,pRegionSize,Flags)	\
    ( (This)->lpVtbl -> CopyTileMappings(This,pDstResource,pDstRegionStartCoordinate,pSrcResource,pSrcRegionStartCoordinate,pRegionSize,Flags) ) 

#define ID3D12CommandQueue_GetCurrentFence(This)	\
    ( (This)->lpVtbl -> GetCurrentFence(This) ) 

#define ID3D12CommandQueue_AdvanceFence(This)	\
    ( (This)->lpVtbl -> AdvanceFence(This) ) 

#define ID3D12CommandQueue_SetEventOnFenceCompletion(This,Value,hEvent)	\
    ( (This)->lpVtbl -> SetEventOnFenceCompletion(This,Value,hEvent) ) 

#define ID3D12CommandQueue_GetLastCompletedFence(This)	\
    ( (This)->lpVtbl -> GetLastCompletedFence(This) ) 

#define ID3D12CommandQueue_ExecuteCommandLists(This,Count,ppCommandLists)	\
    ( (This)->lpVtbl -> ExecuteCommandLists(This,Count,ppCommandLists) ) 

#define ID3D12CommandQueue_PIXBeginEvent(This,Name)	\
    ( (This)->lpVtbl -> PIXBeginEvent(This,Name) ) 

#define ID3D12CommandQueue_PIXEndEvent(This)	\
    ( (This)->lpVtbl -> PIXEndEvent(This) ) 

#define ID3D12CommandQueue_PIXSetMarker(This,Name)	\
    ( (This)->lpVtbl -> PIXSetMarker(This,Name) ) 

#define ID3D12CommandQueue_PIXGetStatus(This)	\
    ( (This)->lpVtbl -> PIXGetStatus(This) ) 

#define ID3D12CommandQueue_Signal(This,pFence,Value)	\
    ( (This)->lpVtbl -> Signal(This,pFence,Value) ) 

#define ID3D12CommandQueue_Wait(This,pFence,Value)	\
    ( (This)->lpVtbl -> Wait(This,pFence,Value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ID3D12CommandQueue_INTERFACE_DEFINED__ */


#ifndef __ID3D12Asynchronous_INTERFACE_DEFINED__
#define __ID3D12Asynchronous_INTERFACE_DEFINED__

/* interface ID3D12Asynchronous */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12Asynchronous;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5d1f22cd-756f-46f4-9c5c-86916eae4e02")
    ID3D12Asynchronous : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetData( 
            /* [annotation] */ 
            _Out_writes_bytes_opt_( DataSize )  void *pData,
            /* [annotation] */ 
            _In_  UINT DataSize) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12AsynchronousVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12Asynchronous * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12Asynchronous * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12Asynchronous * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetData )( 
            ID3D12Asynchronous * This,
            /* [annotation] */ 
            _Out_writes_bytes_opt_( DataSize )  void *pData,
            /* [annotation] */ 
            _In_  UINT DataSize);
        
        END_INTERFACE
    } ID3D12AsynchronousVtbl;

    interface ID3D12Asynchronous
    {
        CONST_VTBL struct ID3D12AsynchronousVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12Asynchronous_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12Asynchronous_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12Asynchronous_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12Asynchronous_GetData(This,pData,DataSize)	\
    ( (This)->lpVtbl -> GetData(This,pData,DataSize) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ID3D12Asynchronous_INTERFACE_DEFINED__ */


#ifndef __ID3D12Library_INTERFACE_DEFINED__
#define __ID3D12Library_INTERFACE_DEFINED__

/* interface ID3D12Library */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12Library;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7579364f-41b3-455a-8785-ed83281ed144")
    ID3D12Library : public ID3D12DeviceChild
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE StoreObject( 
            LPCSTR pName,
            /* [annotation] */ 
            _In_  ID3D12DeviceChild *pDeviceChild) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LoadObject( 
            LPCSTR pName,
            REFGUID UUID,
            /* [annotation] */ 
            _Outptr_  void **ppObject) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Link( 
            /* [annotation] */ 
            _Outptr_  ID3D10Blob **ppBlob) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12LibraryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12Library * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12Library * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12Library * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetPrivateData )( 
            ID3D12Library * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _Inout_  UINT *pDataSize,
            /* [annotation] */ 
            _Out_writes_bytes_opt_( *pDataSize )  void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateData )( 
            ID3D12Library * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_  UINT DataSize,
            /* [annotation] */ 
            _In_reads_bytes_opt_( DataSize )  const void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )( 
            ID3D12Library * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_opt_  const IUnknown *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetName )( 
            ID3D12Library * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR pName);
        
        void ( STDMETHODCALLTYPE *GetDevice )( 
            ID3D12Library * This,
            /* [annotation] */ 
            _Out_  ID3D12Device **ppDevice);
        
        HRESULT ( STDMETHODCALLTYPE *StoreObject )( 
            ID3D12Library * This,
            LPCSTR pName,
            /* [annotation] */ 
            _In_  ID3D12DeviceChild *pDeviceChild);
        
        HRESULT ( STDMETHODCALLTYPE *LoadObject )( 
            ID3D12Library * This,
            LPCSTR pName,
            REFGUID UUID,
            /* [annotation] */ 
            _Outptr_  void **ppObject);
        
        HRESULT ( STDMETHODCALLTYPE *Link )( 
            ID3D12Library * This,
            /* [annotation] */ 
            _Outptr_  ID3D10Blob **ppBlob);
        
        END_INTERFACE
    } ID3D12LibraryVtbl;

    interface ID3D12Library
    {
        CONST_VTBL struct ID3D12LibraryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12Library_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12Library_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12Library_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12Library_GetPrivateData(This,guid,pDataSize,pData)	\
    ( (This)->lpVtbl -> GetPrivateData(This,guid,pDataSize,pData) ) 

#define ID3D12Library_SetPrivateData(This,guid,DataSize,pData)	\
    ( (This)->lpVtbl -> SetPrivateData(This,guid,DataSize,pData) ) 

#define ID3D12Library_SetPrivateDataInterface(This,guid,pData)	\
    ( (This)->lpVtbl -> SetPrivateDataInterface(This,guid,pData) ) 

#define ID3D12Library_SetName(This,pName)	\
    ( (This)->lpVtbl -> SetName(This,pName) ) 


#define ID3D12Library_GetDevice(This,ppDevice)	\
    ( (This)->lpVtbl -> GetDevice(This,ppDevice) ) 


#define ID3D12Library_StoreObject(This,pName,pDeviceChild)	\
    ( (This)->lpVtbl -> StoreObject(This,pName,pDeviceChild) ) 

#define ID3D12Library_LoadObject(This,pName,UUID,ppObject)	\
    ( (This)->lpVtbl -> LoadObject(This,pName,UUID,ppObject) ) 

#define ID3D12Library_Link(This,ppBlob)	\
    ( (This)->lpVtbl -> Link(This,ppBlob) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ID3D12Library_INTERFACE_DEFINED__ */


#ifndef __ID3D12Callback_INTERFACE_DEFINED__
#define __ID3D12Callback_INTERFACE_DEFINED__

/* interface ID3D12Callback */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12Callback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("cbb5d674-cc35-4481-8b09-be1750813f3e")
    ID3D12Callback : public ID3D12DeviceChild
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12CallbackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12Callback * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12Callback * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12Callback * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetPrivateData )( 
            ID3D12Callback * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _Inout_  UINT *pDataSize,
            /* [annotation] */ 
            _Out_writes_bytes_opt_( *pDataSize )  void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateData )( 
            ID3D12Callback * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_  UINT DataSize,
            /* [annotation] */ 
            _In_reads_bytes_opt_( DataSize )  const void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )( 
            ID3D12Callback * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_opt_  const IUnknown *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetName )( 
            ID3D12Callback * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR pName);
        
        void ( STDMETHODCALLTYPE *GetDevice )( 
            ID3D12Callback * This,
            /* [annotation] */ 
            _Out_  ID3D12Device **ppDevice);
        
        END_INTERFACE
    } ID3D12CallbackVtbl;

    interface ID3D12Callback
    {
        CONST_VTBL struct ID3D12CallbackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12Callback_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12Callback_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12Callback_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12Callback_GetPrivateData(This,guid,pDataSize,pData)	\
    ( (This)->lpVtbl -> GetPrivateData(This,guid,pDataSize,pData) ) 

#define ID3D12Callback_SetPrivateData(This,guid,DataSize,pData)	\
    ( (This)->lpVtbl -> SetPrivateData(This,guid,DataSize,pData) ) 

#define ID3D12Callback_SetPrivateDataInterface(This,guid,pData)	\
    ( (This)->lpVtbl -> SetPrivateDataInterface(This,guid,pData) ) 

#define ID3D12Callback_SetName(This,pName)	\
    ( (This)->lpVtbl -> SetName(This,pName) ) 


#define ID3D12Callback_GetDevice(This,ppDevice)	\
    ( (This)->lpVtbl -> GetDevice(This,ppDevice) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ID3D12Callback_INTERFACE_DEFINED__ */


#ifndef __ID3D12Device_INTERFACE_DEFINED__
#define __ID3D12Device_INTERFACE_DEFINED__

/* interface ID3D12Device */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_ID3D12Device;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("13f7b9bd-7113-46ff-ac01-b3e10921f23d")
    ID3D12Device : public ID3D12Object
    {
    public:
        virtual void STDMETHODCALLTYPE GetDefaultCommandQueue( 
            /* [annotation] */ 
            _Outptr_   ID3D12CommandQueue **ppCommandQueue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateCommandQueue( 
            D3D12_COMMAND_LIST_TYPE type,
            INT priority,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppCommandQueue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateCommandAllocator( 
            /* [annotation] */ 
            _In_  D3D12_COMMAND_LIST_TYPE type,
            /* [annotation] */ 
            _Outptr_  ID3D12CommandAllocator **ppCommandAllocator) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateGraphicsPipelineState( 
            /* [annotation] */ 
            _In_  const D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc,
            /* [annotation] */ 
            _Outptr_  ID3D12PipelineState **ppPipelineState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateComputePipelineState( 
            /* [annotation] */ 
            _In_  const D3D12_COMPUTE_PIPELINE_STATE_DESC *pDesc,
            /* [annotation] */ 
            _Outptr_  ID3D12PipelineState **ppPipelineState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateCommandList( 
            /* [annotation] */ 
            _In_  D3D12_COMMAND_LIST_TYPE type,
            /* [annotation] */ 
            _In_  ID3D12CommandAllocator *pCommandAllocator,
            /* [annotation] */ 
            _In_opt_  ID3D12PipelineState *pInitialState,
            /* [annotation] */ 
            _Outptr_  ID3D12CommandList **ppCommandList) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateBuffer( 
            D3D12_HEAP_TYPE HeapType,
            UINT64 ByteSize,
            D3D12_RESOURCE_MISC_FLAG MiscFlags,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvBuffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateDefaultResource( 
            /* [annotation] */ 
            _In_  const D3D11_RESOURCE_DESC *pDesc,
            /* [annotation] */ 
            _In_opt_  const D3D11_SUBRESOURCE_DATA *pInitialData,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvResource) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CheckFeatureSupport( 
            D3D12_FEATURE Feature,
            /* [annotation] */ 
            _Inout_updates_bytes_(FeatureSupportDataSize)  void *pFeatureSupportData,
            UINT FeatureSupportDataSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateDescriptorHeap( 
            /* [annotation] */ 
            _In_  const D3D12_DESCRIPTOR_HEAP_DESC *pDescriptorHeapDesc,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvHeap) = 0;
        
        virtual UINT STDMETHODCALLTYPE GetDescriptorHandleIncrementSize( 
            /* [annotation] */ 
            _In_  D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateRootSignature( 
            /* [annotation] */ 
            _In_reads_(BlobLengthInBytes)  const void *pBlobWithRootSignature,
            /* [annotation] */ 
            _In_  SIZE_T BlobLengthInBytes,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvRootSignature) = 0;
        
        virtual void STDMETHODCALLTYPE CreateIndexBufferView( 
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pBuffer,
            /* [annotation] */ 
            _In_  const D3D12_INDEX_BUFFER_VIEW_DESC *pDesc,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) = 0;
        
        virtual void STDMETHODCALLTYPE CreateVertexBufferView( 
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pBuffer,
            /* [annotation] */ 
            _In_  const D3D12_VERTEX_BUFFER_VIEW_DESC *pDesc,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) = 0;
        
        virtual void STDMETHODCALLTYPE CreateConstantBufferView( 
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pBuffer,
            /* [annotation] */ 
            _In_  const D3D12_CONSTANT_BUFFER_VIEW_DESC *pDesc,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) = 0;
        
        virtual void STDMETHODCALLTYPE CreateShaderResourceView( 
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_opt_  const D3D12_SHADER_RESOURCE_VIEW_DESC *pDesc,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) = 0;
        
        virtual void STDMETHODCALLTYPE CreateUnorderedAccessView( 
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_opt_  const D3D12_UNORDERED_ACCESS_VIEW_DESC *pDesc,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) = 0;
        
        virtual void STDMETHODCALLTYPE CreateStreamOutputView( 
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pBuffer,
            /* [annotation] */ 
            _In_  const D3D12_STREAM_OUTPUT_VIEW_DESC *pDesc,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) = 0;
        
        virtual void STDMETHODCALLTYPE CreateRenderTargetView( 
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_opt_  const D3D12_RENDER_TARGET_VIEW_DESC *pDesc,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) = 0;
        
        virtual void STDMETHODCALLTYPE CreateDepthStencilView( 
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_opt_  const D3D12_DEPTH_STENCIL_VIEW_DESC *pDesc,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) = 0;
        
        virtual void STDMETHODCALLTYPE CreateSampler( 
            /* [annotation] */ 
            _In_  const D3D12_SAMPLER_DESC *pDesc,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) = 0;
        
        virtual void STDMETHODCALLTYPE CopyDescriptors( 
            /* [annotation] */ 
            _In_reads_(NumDestDescriptorRanges)  const D3D12_CPU_DESCRIPTOR_HANDLE *pDestDescriptorRangeStarts,
            /* [annotation] */ 
            _In_reads_opt_(NumDestDescriptorRanges)  const UINT *pDestDescriptorRangeSizes,
            /* [annotation] */ 
            _In_  UINT NumDestDescriptorRanges,
            /* [annotation] */ 
            _In_reads_(NumSrcDescriptorRanges)  const D3D12_CPU_DESCRIPTOR_HANDLE *pSrcDescriptorRangeStarts,
            /* [annotation] */ 
            _In_reads_opt_(NumSrcDescriptorRanges)  const UINT *pSrcDescriptorRangeSizes,
            /* [annotation] */ 
            _In_  UINT NumSrcDescriptorRanges,
            /* [annotation] */ 
            _In_  D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapsType) = 0;
        
        virtual void STDMETHODCALLTYPE CopyDescriptorsSimple( 
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptorRangeStart,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE SrcDescriptorRangeStart,
            /* [annotation] */ 
            _In_  UINT NumDescriptors,
            /* [annotation] */ 
            _In_  D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapsType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateLibrary( 
            /* [annotation] */ 
            _In_reads_(BlobLength)  const void *pLibraryBlob,
            SIZE_T BlobLength,
            /* [annotation] */ 
            _Outptr_  ID3D12Library **ppLibrary) = 0;
        
        virtual D3D12_VIDEO_MEMORY_INFO STDMETHODCALLTYPE QueryVideoMemoryInfo( void) = 0;
        
        virtual void STDMETHODCALLTYPE SetVideoMemoryReservation( 
            /* [annotation] */ 
            _In_  UINT64 Reservation) = 0;
        
        virtual D3D12_RESOURCE_ALLOCATION_INFO STDMETHODCALLTYPE GetResourceAllocationInfo( 
            UINT RDescs,
            /* [annotation] */ 
            _In_reads_(RDescs)  const D3D12_RESOURCE_DESC *pRDescs) = 0;
        
        virtual D3D12_HEAP_PROPERTIES STDMETHODCALLTYPE GetCustomHeapProperties( 
            D3D12_HEAP_TYPE HeapType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateCommittedResource( 
            /* [annotation] */ 
            _In_  const D3D12_HEAP_PROPERTIES *pHeapProperties,
            D3D12_HEAP_MISC_FLAG HeapMiscFlags,
            /* [annotation] */ 
            _In_  const D3D12_RESOURCE_DESC *pResourceDesc,
            D3D12_RESOURCE_USAGE InitialResourceState,
            /* [in] */ REFIID riidResource,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_opt_  void **ppvResource) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateHeap( 
            /* [annotation] */ 
            _In_  const D3D12_HEAP_DESC *pDesc,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_opt_  void **ppvHeap) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreatePlacedResource( 
            /* [annotation] */ 
            _In_opt_  ID3D12Heap *pHeap,
            UINT64 HeapOffset,
            /* [annotation] */ 
            _In_  const D3D12_RESOURCE_DESC *pDesc,
            D3D12_RESOURCE_USAGE InitialState,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_opt_  void **ppvResource) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateReservedResource( 
            /* [annotation] */ 
            _In_  const D3D12_RESOURCE_DESC *pDesc,
            D3D12_RESOURCE_USAGE InitialState,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_opt_  void **ppvResource) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenSharedResourceAndHeap( 
            /* [annotation] */ 
            _In_opt_  HANDLE hResourceAndHeap,
            /* [annotation] */ 
            _In_opt_  LPCWSTR pName,
            UINT DesiredAccess,
            D3D12_RESOURCE_USAGE InitialResourceState,
            /* [in] */ REFIID riidResource,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_opt_  void **ppvResource,
            /* [in] */ REFIID riidHeap,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_opt_  void **ppvHeap) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateSharedHandle( 
            /* [annotation] */ 
            _In_  ID3D12DeviceChild *pObject,
            /* [annotation] */ 
            _In_opt_  const SECURITY_ATTRIBUTES *pAttributes,
            DWORD Access,
            /* [annotation] */ 
            _In_opt_  LPCWSTR pName,
            /* [annotation] */ 
            _Out_  HANDLE *pHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MakeResident( 
            UINT Objects,
            /* [annotation] */ 
            _In_reads_(Objects)  ID3D12Pageable *const *ppObjects) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Evict( 
            UINT Objects,
            /* [annotation] */ 
            _In_reads_(Objects)  ID3D12Pageable *const *ppObjects) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterBudgetChangeCallback( 
            /* [annotation] */ 
            _In_  PFN_D3D12_BUDGET_CHANGED pfnCallback,
            /* [annotation] */ 
            _In_opt_  PVOID pContext,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvCallback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateFence( 
            UINT64 InitialValue,
            D3D12_FENCE_MISC_FLAG Flags,
            /* [annotation] */ 
            _Outptr_  ID3D12Fence **ppFence) = 0;
        
        virtual D3D12_CREATE_DEVICE_FLAG STDMETHODCALLTYPE GetCreateDeviceFlags( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDeviceRemovedReason( void) = 0;
        
        virtual void STDMETHODCALLTYPE GetCopyableLayout( 
            /* [annotation] */ 
            _In_  const D3D12_RESOURCE_DESC *pResourceDesc,
            UINT FirstSubresource,
            UINT NumSubresources,
            UINT64 BaseOffset,
            /* [annotation] */ 
            _Out_writes_opt_(NumSubresources)  D3D12_PLACED_PITCHED_SUBRESOURCE_DESC *pLayouts,
            /* [annotation] */ 
            _Out_writes_opt_(NumSubresources)  UINT *pNumRows,
            /* [annotation] */ 
            _Out_writes_opt_(NumSubresources)  UINT64 *pRowSizeInBytes,
            /* [annotation] */ 
            _Out_opt_  UINT64 *pTotalBytes) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ID3D12DeviceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3D12Device * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3D12Device * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3D12Device * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetPrivateData )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _Inout_  UINT *pDataSize,
            /* [annotation] */ 
            _Out_writes_bytes_opt_( *pDataSize )  void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateData )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_  UINT DataSize,
            /* [annotation] */ 
            _In_reads_bytes_opt_( DataSize )  const void *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  REFGUID guid,
            /* [annotation] */ 
            _In_opt_  const IUnknown *pData);
        
        HRESULT ( STDMETHODCALLTYPE *SetName )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_z_  LPCWSTR pName);
        
        void ( STDMETHODCALLTYPE *GetDefaultCommandQueue )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _Outptr_   ID3D12CommandQueue **ppCommandQueue);
        
        HRESULT ( STDMETHODCALLTYPE *CreateCommandQueue )( 
            ID3D12Device * This,
            D3D12_COMMAND_LIST_TYPE type,
            INT priority,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppCommandQueue);
        
        HRESULT ( STDMETHODCALLTYPE *CreateCommandAllocator )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  D3D12_COMMAND_LIST_TYPE type,
            /* [annotation] */ 
            _Outptr_  ID3D12CommandAllocator **ppCommandAllocator);
        
        HRESULT ( STDMETHODCALLTYPE *CreateGraphicsPipelineState )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  const D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc,
            /* [annotation] */ 
            _Outptr_  ID3D12PipelineState **ppPipelineState);
        
        HRESULT ( STDMETHODCALLTYPE *CreateComputePipelineState )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  const D3D12_COMPUTE_PIPELINE_STATE_DESC *pDesc,
            /* [annotation] */ 
            _Outptr_  ID3D12PipelineState **ppPipelineState);
        
        HRESULT ( STDMETHODCALLTYPE *CreateCommandList )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  D3D12_COMMAND_LIST_TYPE type,
            /* [annotation] */ 
            _In_  ID3D12CommandAllocator *pCommandAllocator,
            /* [annotation] */ 
            _In_opt_  ID3D12PipelineState *pInitialState,
            /* [annotation] */ 
            _Outptr_  ID3D12CommandList **ppCommandList);
        
        HRESULT ( STDMETHODCALLTYPE *CreateBuffer )( 
            ID3D12Device * This,
            D3D12_HEAP_TYPE HeapType,
            UINT64 ByteSize,
            D3D12_RESOURCE_MISC_FLAG MiscFlags,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvBuffer);
        
        HRESULT ( STDMETHODCALLTYPE *CreateDefaultResource )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  const D3D11_RESOURCE_DESC *pDesc,
            /* [annotation] */ 
            _In_opt_  const D3D11_SUBRESOURCE_DATA *pInitialData,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvResource);
        
        HRESULT ( STDMETHODCALLTYPE *CheckFeatureSupport )( 
            ID3D12Device * This,
            D3D12_FEATURE Feature,
            /* [annotation] */ 
            _Inout_updates_bytes_(FeatureSupportDataSize)  void *pFeatureSupportData,
            UINT FeatureSupportDataSize);
        
        HRESULT ( STDMETHODCALLTYPE *CreateDescriptorHeap )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  const D3D12_DESCRIPTOR_HEAP_DESC *pDescriptorHeapDesc,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvHeap);
        
        UINT ( STDMETHODCALLTYPE *GetDescriptorHandleIncrementSize )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapType);
        
        HRESULT ( STDMETHODCALLTYPE *CreateRootSignature )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_reads_(BlobLengthInBytes)  const void *pBlobWithRootSignature,
            /* [annotation] */ 
            _In_  SIZE_T BlobLengthInBytes,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvRootSignature);
        
        void ( STDMETHODCALLTYPE *CreateIndexBufferView )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pBuffer,
            /* [annotation] */ 
            _In_  const D3D12_INDEX_BUFFER_VIEW_DESC *pDesc,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);
        
        void ( STDMETHODCALLTYPE *CreateVertexBufferView )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pBuffer,
            /* [annotation] */ 
            _In_  const D3D12_VERTEX_BUFFER_VIEW_DESC *pDesc,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);
        
        void ( STDMETHODCALLTYPE *CreateConstantBufferView )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pBuffer,
            /* [annotation] */ 
            _In_  const D3D12_CONSTANT_BUFFER_VIEW_DESC *pDesc,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);
        
        void ( STDMETHODCALLTYPE *CreateShaderResourceView )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_opt_  const D3D12_SHADER_RESOURCE_VIEW_DESC *pDesc,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);
        
        void ( STDMETHODCALLTYPE *CreateUnorderedAccessView )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_opt_  const D3D12_UNORDERED_ACCESS_VIEW_DESC *pDesc,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);
        
        void ( STDMETHODCALLTYPE *CreateStreamOutputView )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pBuffer,
            /* [annotation] */ 
            _In_  const D3D12_STREAM_OUTPUT_VIEW_DESC *pDesc,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);
        
        void ( STDMETHODCALLTYPE *CreateRenderTargetView )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_opt_  const D3D12_RENDER_TARGET_VIEW_DESC *pDesc,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);
        
        void ( STDMETHODCALLTYPE *CreateDepthStencilView )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_opt_  ID3D12Resource *pResource,
            /* [annotation] */ 
            _In_opt_  const D3D12_DEPTH_STENCIL_VIEW_DESC *pDesc,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);
        
        void ( STDMETHODCALLTYPE *CreateSampler )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  const D3D12_SAMPLER_DESC *pDesc,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);
        
        void ( STDMETHODCALLTYPE *CopyDescriptors )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_reads_(NumDestDescriptorRanges)  const D3D12_CPU_DESCRIPTOR_HANDLE *pDestDescriptorRangeStarts,
            /* [annotation] */ 
            _In_reads_opt_(NumDestDescriptorRanges)  const UINT *pDestDescriptorRangeSizes,
            /* [annotation] */ 
            _In_  UINT NumDestDescriptorRanges,
            /* [annotation] */ 
            _In_reads_(NumSrcDescriptorRanges)  const D3D12_CPU_DESCRIPTOR_HANDLE *pSrcDescriptorRangeStarts,
            /* [annotation] */ 
            _In_reads_opt_(NumSrcDescriptorRanges)  const UINT *pSrcDescriptorRangeSizes,
            /* [annotation] */ 
            _In_  UINT NumSrcDescriptorRanges,
            /* [annotation] */ 
            _In_  D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapsType);
        
        void ( STDMETHODCALLTYPE *CopyDescriptorsSimple )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptorRangeStart,
            /* [annotation] */ 
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE SrcDescriptorRangeStart,
            /* [annotation] */ 
            _In_  UINT NumDescriptors,
            /* [annotation] */ 
            _In_  D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapsType);
        
        HRESULT ( STDMETHODCALLTYPE *CreateLibrary )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_reads_(BlobLength)  const void *pLibraryBlob,
            SIZE_T BlobLength,
            /* [annotation] */ 
            _Outptr_  ID3D12Library **ppLibrary);
        
        D3D12_VIDEO_MEMORY_INFO ( STDMETHODCALLTYPE *QueryVideoMemoryInfo )( 
            ID3D12Device * This);
        
        void ( STDMETHODCALLTYPE *SetVideoMemoryReservation )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  UINT64 Reservation);
        
        D3D12_RESOURCE_ALLOCATION_INFO ( STDMETHODCALLTYPE *GetResourceAllocationInfo )( 
            ID3D12Device * This,
            UINT RDescs,
            /* [annotation] */ 
            _In_reads_(RDescs)  const D3D12_RESOURCE_DESC *pRDescs);
        
        D3D12_HEAP_PROPERTIES ( STDMETHODCALLTYPE *GetCustomHeapProperties )( 
            ID3D12Device * This,
            D3D12_HEAP_TYPE HeapType);
        
        HRESULT ( STDMETHODCALLTYPE *CreateCommittedResource )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  const D3D12_HEAP_PROPERTIES *pHeapProperties,
            D3D12_HEAP_MISC_FLAG HeapMiscFlags,
            /* [annotation] */ 
            _In_  const D3D12_RESOURCE_DESC *pResourceDesc,
            D3D12_RESOURCE_USAGE InitialResourceState,
            /* [in] */ REFIID riidResource,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_opt_  void **ppvResource);
        
        HRESULT ( STDMETHODCALLTYPE *CreateHeap )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  const D3D12_HEAP_DESC *pDesc,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_opt_  void **ppvHeap);
        
        HRESULT ( STDMETHODCALLTYPE *CreatePlacedResource )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_opt_  ID3D12Heap *pHeap,
            UINT64 HeapOffset,
            /* [annotation] */ 
            _In_  const D3D12_RESOURCE_DESC *pDesc,
            D3D12_RESOURCE_USAGE InitialState,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_opt_  void **ppvResource);
        
        HRESULT ( STDMETHODCALLTYPE *CreateReservedResource )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  const D3D12_RESOURCE_DESC *pDesc,
            D3D12_RESOURCE_USAGE InitialState,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_opt_  void **ppvResource);
        
        HRESULT ( STDMETHODCALLTYPE *OpenSharedResourceAndHeap )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_opt_  HANDLE hResourceAndHeap,
            /* [annotation] */ 
            _In_opt_  LPCWSTR pName,
            UINT DesiredAccess,
            D3D12_RESOURCE_USAGE InitialResourceState,
            /* [in] */ REFIID riidResource,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_opt_  void **ppvResource,
            /* [in] */ REFIID riidHeap,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_opt_  void **ppvHeap);
        
        HRESULT ( STDMETHODCALLTYPE *CreateSharedHandle )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  ID3D12DeviceChild *pObject,
            /* [annotation] */ 
            _In_opt_  const SECURITY_ATTRIBUTES *pAttributes,
            DWORD Access,
            /* [annotation] */ 
            _In_opt_  LPCWSTR pName,
            /* [annotation] */ 
            _Out_  HANDLE *pHandle);
        
        HRESULT ( STDMETHODCALLTYPE *MakeResident )( 
            ID3D12Device * This,
            UINT Objects,
            /* [annotation] */ 
            _In_reads_(Objects)  ID3D12Pageable *const *ppObjects);
        
        HRESULT ( STDMETHODCALLTYPE *Evict )( 
            ID3D12Device * This,
            UINT Objects,
            /* [annotation] */ 
            _In_reads_(Objects)  ID3D12Pageable *const *ppObjects);
        
        HRESULT ( STDMETHODCALLTYPE *RegisterBudgetChangeCallback )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  PFN_D3D12_BUDGET_CHANGED pfnCallback,
            /* [annotation] */ 
            _In_opt_  PVOID pContext,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvCallback);
        
        HRESULT ( STDMETHODCALLTYPE *CreateFence )( 
            ID3D12Device * This,
            UINT64 InitialValue,
            D3D12_FENCE_MISC_FLAG Flags,
            /* [annotation] */ 
            _Outptr_  ID3D12Fence **ppFence);
        
        D3D12_CREATE_DEVICE_FLAG ( STDMETHODCALLTYPE *GetCreateDeviceFlags )( 
            ID3D12Device * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetDeviceRemovedReason )( 
            ID3D12Device * This);
        
        void ( STDMETHODCALLTYPE *GetCopyableLayout )( 
            ID3D12Device * This,
            /* [annotation] */ 
            _In_  const D3D12_RESOURCE_DESC *pResourceDesc,
            UINT FirstSubresource,
            UINT NumSubresources,
            UINT64 BaseOffset,
            /* [annotation] */ 
            _Out_writes_opt_(NumSubresources)  D3D12_PLACED_PITCHED_SUBRESOURCE_DESC *pLayouts,
            /* [annotation] */ 
            _Out_writes_opt_(NumSubresources)  UINT *pNumRows,
            /* [annotation] */ 
            _Out_writes_opt_(NumSubresources)  UINT64 *pRowSizeInBytes,
            /* [annotation] */ 
            _Out_opt_  UINT64 *pTotalBytes);
        
        END_INTERFACE
    } ID3D12DeviceVtbl;

    interface ID3D12Device
    {
        CONST_VTBL struct ID3D12DeviceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3D12Device_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ID3D12Device_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ID3D12Device_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ID3D12Device_GetPrivateData(This,guid,pDataSize,pData)	\
    ( (This)->lpVtbl -> GetPrivateData(This,guid,pDataSize,pData) ) 

#define ID3D12Device_SetPrivateData(This,guid,DataSize,pData)	\
    ( (This)->lpVtbl -> SetPrivateData(This,guid,DataSize,pData) ) 

#define ID3D12Device_SetPrivateDataInterface(This,guid,pData)	\
    ( (This)->lpVtbl -> SetPrivateDataInterface(This,guid,pData) ) 

#define ID3D12Device_SetName(This,pName)	\
    ( (This)->lpVtbl -> SetName(This,pName) ) 


#define ID3D12Device_GetDefaultCommandQueue(This,ppCommandQueue)	\
    ( (This)->lpVtbl -> GetDefaultCommandQueue(This,ppCommandQueue) ) 

#define ID3D12Device_CreateCommandQueue(This,type,priority,riid,ppCommandQueue)	\
    ( (This)->lpVtbl -> CreateCommandQueue(This,type,priority,riid,ppCommandQueue) ) 

#define ID3D12Device_CreateCommandAllocator(This,type,ppCommandAllocator)	\
    ( (This)->lpVtbl -> CreateCommandAllocator(This,type,ppCommandAllocator) ) 

#define ID3D12Device_CreateGraphicsPipelineState(This,pDesc,ppPipelineState)	\
    ( (This)->lpVtbl -> CreateGraphicsPipelineState(This,pDesc,ppPipelineState) ) 

#define ID3D12Device_CreateComputePipelineState(This,pDesc,ppPipelineState)	\
    ( (This)->lpVtbl -> CreateComputePipelineState(This,pDesc,ppPipelineState) ) 

#define ID3D12Device_CreateCommandList(This,type,pCommandAllocator,pInitialState,ppCommandList)	\
    ( (This)->lpVtbl -> CreateCommandList(This,type,pCommandAllocator,pInitialState,ppCommandList) ) 

#define ID3D12Device_CreateBuffer(This,HeapType,ByteSize,MiscFlags,riid,ppvBuffer)	\
    ( (This)->lpVtbl -> CreateBuffer(This,HeapType,ByteSize,MiscFlags,riid,ppvBuffer) ) 

#define ID3D12Device_CreateDefaultResource(This,pDesc,pInitialData,riid,ppvResource)	\
    ( (This)->lpVtbl -> CreateDefaultResource(This,pDesc,pInitialData,riid,ppvResource) ) 

#define ID3D12Device_CheckFeatureSupport(This,Feature,pFeatureSupportData,FeatureSupportDataSize)	\
    ( (This)->lpVtbl -> CheckFeatureSupport(This,Feature,pFeatureSupportData,FeatureSupportDataSize) ) 

#define ID3D12Device_CreateDescriptorHeap(This,pDescriptorHeapDesc,riid,ppvHeap)	\
    ( (This)->lpVtbl -> CreateDescriptorHeap(This,pDescriptorHeapDesc,riid,ppvHeap) ) 

#define ID3D12Device_GetDescriptorHandleIncrementSize(This,DescriptorHeapType)	\
    ( (This)->lpVtbl -> GetDescriptorHandleIncrementSize(This,DescriptorHeapType) ) 

#define ID3D12Device_CreateRootSignature(This,pBlobWithRootSignature,BlobLengthInBytes,riid,ppvRootSignature)	\
    ( (This)->lpVtbl -> CreateRootSignature(This,pBlobWithRootSignature,BlobLengthInBytes,riid,ppvRootSignature) ) 

#define ID3D12Device_CreateIndexBufferView(This,pBuffer,pDesc,DestDescriptor)	\
    ( (This)->lpVtbl -> CreateIndexBufferView(This,pBuffer,pDesc,DestDescriptor) ) 

#define ID3D12Device_CreateVertexBufferView(This,pBuffer,pDesc,DestDescriptor)	\
    ( (This)->lpVtbl -> CreateVertexBufferView(This,pBuffer,pDesc,DestDescriptor) ) 

#define ID3D12Device_CreateConstantBufferView(This,pBuffer,pDesc,DestDescriptor)	\
    ( (This)->lpVtbl -> CreateConstantBufferView(This,pBuffer,pDesc,DestDescriptor) ) 

#define ID3D12Device_CreateShaderResourceView(This,pResource,pDesc,DestDescriptor)	\
    ( (This)->lpVtbl -> CreateShaderResourceView(This,pResource,pDesc,DestDescriptor) ) 

#define ID3D12Device_CreateUnorderedAccessView(This,pResource,pDesc,DestDescriptor)	\
    ( (This)->lpVtbl -> CreateUnorderedAccessView(This,pResource,pDesc,DestDescriptor) ) 

#define ID3D12Device_CreateStreamOutputView(This,pBuffer,pDesc,DestDescriptor)	\
    ( (This)->lpVtbl -> CreateStreamOutputView(This,pBuffer,pDesc,DestDescriptor) ) 

#define ID3D12Device_CreateRenderTargetView(This,pResource,pDesc,DestDescriptor)	\
    ( (This)->lpVtbl -> CreateRenderTargetView(This,pResource,pDesc,DestDescriptor) ) 

#define ID3D12Device_CreateDepthStencilView(This,pResource,pDesc,DestDescriptor)	\
    ( (This)->lpVtbl -> CreateDepthStencilView(This,pResource,pDesc,DestDescriptor) ) 

#define ID3D12Device_CreateSampler(This,pDesc,DestDescriptor)	\
    ( (This)->lpVtbl -> CreateSampler(This,pDesc,DestDescriptor) ) 

#define ID3D12Device_CopyDescriptors(This,pDestDescriptorRangeStarts,pDestDescriptorRangeSizes,NumDestDescriptorRanges,pSrcDescriptorRangeStarts,pSrcDescriptorRangeSizes,NumSrcDescriptorRanges,DescriptorHeapsType)	\
    ( (This)->lpVtbl -> CopyDescriptors(This,pDestDescriptorRangeStarts,pDestDescriptorRangeSizes,NumDestDescriptorRanges,pSrcDescriptorRangeStarts,pSrcDescriptorRangeSizes,NumSrcDescriptorRanges,DescriptorHeapsType) ) 

#define ID3D12Device_CopyDescriptorsSimple(This,DestDescriptorRangeStart,SrcDescriptorRangeStart,NumDescriptors,DescriptorHeapsType)	\
    ( (This)->lpVtbl -> CopyDescriptorsSimple(This,DestDescriptorRangeStart,SrcDescriptorRangeStart,NumDescriptors,DescriptorHeapsType) ) 

#define ID3D12Device_CreateLibrary(This,pLibraryBlob,BlobLength,ppLibrary)	\
    ( (This)->lpVtbl -> CreateLibrary(This,pLibraryBlob,BlobLength,ppLibrary) ) 

#define ID3D12Device_QueryVideoMemoryInfo(This)	\
    ( (This)->lpVtbl -> QueryVideoMemoryInfo(This) ) 

#define ID3D12Device_SetVideoMemoryReservation(This,Reservation)	\
    ( (This)->lpVtbl -> SetVideoMemoryReservation(This,Reservation) ) 

#define ID3D12Device_GetResourceAllocationInfo(This,RDescs,pRDescs)	\
    ( (This)->lpVtbl -> GetResourceAllocationInfo(This,RDescs,pRDescs) ) 

#define ID3D12Device_GetCustomHeapProperties(This,HeapType)	\
    ( (This)->lpVtbl -> GetCustomHeapProperties(This,HeapType) ) 

#define ID3D12Device_CreateCommittedResource(This,pHeapProperties,HeapMiscFlags,pResourceDesc,InitialResourceState,riidResource,ppvResource)	\
    ( (This)->lpVtbl -> CreateCommittedResource(This,pHeapProperties,HeapMiscFlags,pResourceDesc,InitialResourceState,riidResource,ppvResource) ) 

#define ID3D12Device_CreateHeap(This,pDesc,riid,ppvHeap)	\
    ( (This)->lpVtbl -> CreateHeap(This,pDesc,riid,ppvHeap) ) 

#define ID3D12Device_CreatePlacedResource(This,pHeap,HeapOffset,pDesc,InitialState,riid,ppvResource)	\
    ( (This)->lpVtbl -> CreatePlacedResource(This,pHeap,HeapOffset,pDesc,InitialState,riid,ppvResource) ) 

#define ID3D12Device_CreateReservedResource(This,pDesc,InitialState,riid,ppvResource)	\
    ( (This)->lpVtbl -> CreateReservedResource(This,pDesc,InitialState,riid,ppvResource) ) 

#define ID3D12Device_OpenSharedResourceAndHeap(This,hResourceAndHeap,pName,DesiredAccess,InitialResourceState,riidResource,ppvResource,riidHeap,ppvHeap)	\
    ( (This)->lpVtbl -> OpenSharedResourceAndHeap(This,hResourceAndHeap,pName,DesiredAccess,InitialResourceState,riidResource,ppvResource,riidHeap,ppvHeap) ) 

#define ID3D12Device_CreateSharedHandle(This,pObject,pAttributes,Access,pName,pHandle)	\
    ( (This)->lpVtbl -> CreateSharedHandle(This,pObject,pAttributes,Access,pName,pHandle) ) 

#define ID3D12Device_MakeResident(This,Objects,ppObjects)	\
    ( (This)->lpVtbl -> MakeResident(This,Objects,ppObjects) ) 

#define ID3D12Device_Evict(This,Objects,ppObjects)	\
    ( (This)->lpVtbl -> Evict(This,Objects,ppObjects) ) 

#define ID3D12Device_RegisterBudgetChangeCallback(This,pfnCallback,pContext,riid,ppvCallback)	\
    ( (This)->lpVtbl -> RegisterBudgetChangeCallback(This,pfnCallback,pContext,riid,ppvCallback) ) 

#define ID3D12Device_CreateFence(This,InitialValue,Flags,ppFence)	\
    ( (This)->lpVtbl -> CreateFence(This,InitialValue,Flags,ppFence) ) 

#define ID3D12Device_GetCreateDeviceFlags(This)	\
    ( (This)->lpVtbl -> GetCreateDeviceFlags(This) ) 

#define ID3D12Device_GetDeviceRemovedReason(This)	\
    ( (This)->lpVtbl -> GetDeviceRemovedReason(This) ) 

#define ID3D12Device_GetCopyableLayout(This,pResourceDesc,FirstSubresource,NumSubresources,BaseOffset,pLayouts,pNumRows,pRowSizeInBytes,pTotalBytes)	\
    ( (This)->lpVtbl -> GetCopyableLayout(This,pResourceDesc,FirstSubresource,NumSubresources,BaseOffset,pLayouts,pNumRows,pRowSizeInBytes,pTotalBytes) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



D3D12_VIDEO_MEMORY_INFO STDMETHODCALLTYPE ID3D12Device_QueryVideoMemoryInfo_Proxy( 
    ID3D12Device * This);


void __RPC_STUB ID3D12Device_QueryVideoMemoryInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


D3D12_RESOURCE_ALLOCATION_INFO STDMETHODCALLTYPE ID3D12Device_GetResourceAllocationInfo_Proxy( 
    ID3D12Device * This,
    UINT RDescs,
    /* [annotation] */ 
    _In_reads_(RDescs)  const D3D12_RESOURCE_DESC *pRDescs);


void __RPC_STUB ID3D12Device_GetResourceAllocationInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


D3D12_HEAP_PROPERTIES STDMETHODCALLTYPE ID3D12Device_GetCustomHeapProperties_Proxy( 
    ID3D12Device * This,
    D3D12_HEAP_TYPE HeapType);


void __RPC_STUB ID3D12Device_GetCustomHeapProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ID3D12Device_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_d3d12_0000_0017 */
/* [local] */ 

typedef struct D3D12_SUBRESOURCE_DATA
    {
    const void *pData;
    LONG_PTR RowPitch;
    LONG_PTR SlicePitch;
    } 	D3D12_SUBRESOURCE_DATA;

typedef struct D3D12_MEMCPY_DEST
    {
    void *pData;
    SIZE_T RowPitch;
    SIZE_T SlicePitch;
    } 	D3D12_MEMCPY_DEST;

#if !defined( D3D12_NO_HELPERS ) && defined( __cplusplus )
}
// Row-by-row memcpy
inline void MemcpySubresource(
    const D3D12_MEMCPY_DEST* pDest,
    const D3D12_SUBRESOURCE_DATA* pSrc,
    SIZE_T RowSizeInBytes,
    UINT NumRows,
    UINT NumSlices)
{
    for (UINT z = 0; z < NumSlices; ++z)
    {
        BYTE* pDestSlice = reinterpret_cast<BYTE*>(pDest->pData) + pDest->SlicePitch * z;
        const BYTE* pSrcSlice = reinterpret_cast<const BYTE*>(pSrc->pData) + pSrc->SlicePitch * z;
        for (UINT y = 0; y < NumRows; ++y)
        {
            memcpy(pDestSlice + pDest->RowPitch * y,
                   pSrcSlice + pSrc->RowPitch * y,
                   RowSizeInBytes);
        }
    }
}

// Returns required size of a buffer to be used for data upload
inline UINT64 GetRequiredIntermediateSize(
    _In_ ID3D12Resource* pDestinationResource,
    UINT FirstSubresource,
    UINT NumSubresources)
{
    D3D12_RESOURCE_DESC Desc = pDestinationResource->GetDesc();
    UINT64 RequiredSize = 0;
    
    ID3D12Device* pDevice;
    pDestinationResource->GetDevice(&pDevice);
    pDevice->GetCopyableLayout(&Desc, FirstSubresource, NumSubresources, 0, nullptr, nullptr, nullptr, &RequiredSize);
    pDevice->Release();
    
    return RequiredSize;
}

// All arrays must be populated (e.g. by calling GetCopyableLayout)
inline UINT64 UpdateSubresources(
    _In_ ID3D12CommandList* pCmdList,
    _In_ ID3D12Resource* pDestinationResource,
    _In_ ID3D12Resource* pIntermediate,
    UINT FirstSubresource,
    UINT NumSubresources,
    UINT64 RequiredSize,
    _In_reads_(NumSubresources) const D3D12_PLACED_PITCHED_SUBRESOURCE_DESC* pLayouts,
    _In_reads_(NumSubresources) const UINT* pNumRows,
    _In_reads_(NumSubresources) const UINT64* pRowSizesInBytes,
    _In_reads_(NumSubresources) const D3D12_SUBRESOURCE_DATA* pSrcData)
{
    // Minor validation
    D3D12_RESOURCE_DESC IntermediateDesc = pIntermediate->GetDesc();
    D3D12_RESOURCE_DESC DestinationDesc = pDestinationResource->GetDesc();
    if (IntermediateDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER || 
        IntermediateDesc.Width < RequiredSize + pLayouts[0].Offset || 
        RequiredSize > (SIZE_T)-1 || 
        (DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER && 
            (FirstSubresource != 0 || NumSubresources != 1)))
    {
        return 0;
    }
    
    BYTE* pData;
    HRESULT hr = pIntermediate->Map(NULL, reinterpret_cast<void**>(&pData));
    if (FAILED(hr))
    {
        return 0;
    }
    
    for (UINT i = 0; i < NumSubresources; ++i)
    {
        if (pRowSizesInBytes[i] > (SIZE_T)-1) return 0;
        D3D12_MEMCPY_DEST DestData = { pData + pLayouts[i].Offset, pLayouts[i].Placement.RowPitch, pLayouts[i].Placement.RowPitch * pNumRows[i] };
        MemcpySubresource(&DestData, &pSrcData[i], (SIZE_T)pRowSizesInBytes[i], pNumRows[i], pLayouts[i].Placement.Depth);
    }
    pIntermediate->Unmap(NULL);
    
    if (DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
    {
        CD3D12_BOX SrcBox( UINT( pLayouts[0].Offset ), UINT( pLayouts[0].Offset + pLayouts[0].Placement.Width ) );
        D3D12_SELECT_SUBRESOURCE SubresourceZero = { 0 };
        pCmdList->CopySubresourceRegion(
            pDestinationResource, D3D12_SUBRESOURCE_VIEW_SELECT_SUBRESOURCE, &SubresourceZero, 0, 0, 0,
            pIntermediate, D3D12_SUBRESOURCE_VIEW_SELECT_SUBRESOURCE, &SubresourceZero, &SrcBox, D3D12_COPY_NO_OVERWRITE);
    }
    else
    {
        for (UINT i = 0; i < NumSubresources; ++i)
        {
            D3D12_SELECT_SUBRESOURCE DestSubresource = { i + FirstSubresource };
            pCmdList->CopySubresourceRegion(
                pDestinationResource, D3D12_SUBRESOURCE_VIEW_SELECT_SUBRESOURCE, &DestSubresource, 0, 0, 0,
                pIntermediate, D3D12_SUBRESOURCE_VIEW_PLACED_PITCHED_SUBRESOURCE, &pLayouts[i], nullptr, D3D12_COPY_NO_OVERWRITE);
        }
    }
    return RequiredSize;
}

// Heap-allocating UpdateSubresources implementation
inline UINT64 UpdateSubresources(
    _In_ ID3D12CommandList* pCmdList,
    _In_ ID3D12Resource* pDestinationResource,
    _In_ ID3D12Resource* pIntermediate,
    UINT64 IntermediateOffset,
    UINT FirstSubresource,
    UINT NumSubresources,
    _In_reads_(NumSubresources) D3D12_SUBRESOURCE_DATA* pSrcData)
{
    UINT64 RequiredSize = 0;
    D3D12_PLACED_PITCHED_SUBRESOURCE_DESC* pLayouts = new D3D12_PLACED_PITCHED_SUBRESOURCE_DESC[NumSubresources];
    UINT* pNumRows = new UINT[NumSubresources];
    UINT64* pRowSizesInBytes = new UINT64[NumSubresources];
    
    D3D12_RESOURCE_DESC Desc = pDestinationResource->GetDesc();
    ID3D12Device* pDevice;
    pDestinationResource->GetDevice(&pDevice);
    pDevice->GetCopyableLayout(&Desc, FirstSubresource, NumSubresources, IntermediateOffset, pLayouts, pNumRows, pRowSizesInBytes, &RequiredSize);
    pDevice->Release();
    
    UINT64 Result = UpdateSubresources(pCmdList, pDestinationResource, pIntermediate, FirstSubresource, NumSubresources, RequiredSize, pLayouts, pNumRows, pRowSizesInBytes, pSrcData);
    delete[] pLayouts;
    delete[] pNumRows;
    delete[] pRowSizesInBytes;
    return Result;
}

// Stack-allocating UpdateSubresources implementation
template <UINT MaxSubresources>
inline UINT64 UpdateSubresources(
    _In_ ID3D12CommandList* pCmdList,
    _In_ ID3D12Resource* pDestinationResource,
    _In_ ID3D12Resource* pIntermediate,
    UINT64 IntermediateOffset,
    _In_range_(0, MaxSubresources) UINT FirstSubresource,
    _In_range_(1, MaxSubresources - FirstSubresource) UINT NumSubresources,
    _In_reads_(NumSubresources) D3D12_SUBRESOURCE_DATA* pSrcData)
{
    UINT64 RequiredSize = 0;
    D3D12_PLACED_PITCHED_SUBRESOURCE_DESC Layouts[MaxSubresources];
    UINT NumRows[MaxSubresources];
    UINT64 RowSizesInBytes[MaxSubresources];
    
    D3D12_RESOURCE_DESC Desc = pDestinationResource->GetDesc();
    ID3D12Device* pDevice;
    pDestinationResource->GetDevice(&pDevice);
    pDevice->GetCopyableLayout(&Desc, FirstSubresource, NumSubresources, IntermediateOffset, Layouts, NumRows, RowSizesInBytes, &RequiredSize);
    pDevice->Release();
    
    return UpdateSubresources(pCmdList, pDestinationResource, pIntermediate, FirstSubresource, NumSubresources, RequiredSize, Layouts, NumRows, RowSizesInBytes, pSrcData);
}
extern "C"{
#endif
DEFINE_GUID(IID_ID3D12Object,0xc4fec28f,0x7966,0x4e95,0x9f,0x94,0xf4,0x31,0xcb,0x56,0xc3,0xb8);
DEFINE_GUID(IID_ID3D12DeviceChild,0x905db94b,0xa00c,0x4140,0x9d,0xf5,0x2b,0x64,0xca,0x9e,0xa3,0x57);
DEFINE_GUID(IID_ID3D12RootSignature,0xc54a6b66,0x72df,0x4ee8,0x8b,0xe5,0xa9,0x46,0xa1,0x42,0x92,0x14);
DEFINE_GUID(IID_ID3D12RootSignatureDeserializer,0x34AB647B,0x3CC8,0x46AC,0x84,0x1B,0xC0,0x96,0x56,0x45,0xC0,0x46);
DEFINE_GUID(IID_ID3D12Pageable,0x63ee58fb,0x1268,0x4835,0x86,0xda,0xf0,0x08,0xce,0x62,0xf0,0xd6);
DEFINE_GUID(IID_ID3D12Heap,0xf98f3ba4,0xe608,0x4cf6,0x9e,0x29,0xa1,0x4e,0x50,0xd4,0x92,0x9d);
DEFINE_GUID(IID_ID3D12Resource,0x696442be,0xa72e,0x4059,0xbc,0x79,0x5b,0x5c,0x98,0x04,0x0f,0xad);
DEFINE_GUID(IID_ID3D12CommandAllocator,0x6102dee4,0xaf59,0x4b09,0xb9,0x99,0xb4,0x4d,0x73,0xf0,0x9b,0x24);
DEFINE_GUID(IID_ID3D12Fence,0x0a753dcf,0xc4d8,0x4b91,0xad,0xf6,0xbe,0x5a,0x60,0xd9,0x5a,0x76);
DEFINE_GUID(IID_ID3D12PipelineState,0x765a30f3,0xf624,0x4c6f,0xa8,0x28,0xac,0xe9,0x48,0x62,0x24,0x45);
DEFINE_GUID(IID_ID3D12DescriptorHeap,0x3c2922d5,0xf9eb,0x41a8,0x9a,0x15,0x9e,0x70,0x76,0x10,0x98,0x4a);
DEFINE_GUID(IID_ID3D12CommandList,0x168b7173,0xba23,0x4bc8,0x9f,0xf9,0xee,0x1d,0xb8,0x8a,0xfb,0x20);
DEFINE_GUID(IID_ID3D12CommandQueue,0xc93c8c3d,0xbf95,0x441a,0xb0,0xcd,0x56,0xea,0x3c,0x32,0x87,0xf1);
DEFINE_GUID(IID_ID3D12Asynchronous,0x5d1f22cd,0x756f,0x46f4,0x9c,0x5c,0x86,0x91,0x6e,0xae,0x4e,0x02);
DEFINE_GUID(IID_ID3D12Library,0x7579364f,0x41b3,0x455a,0x87,0x85,0xed,0x83,0x28,0x1e,0xd1,0x44);
DEFINE_GUID(IID_ID3D12Callback,0xcbb5d674,0xcc35,0x4481,0x8b,0x09,0xbe,0x17,0x50,0x81,0x3f,0x3e);
DEFINE_GUID(IID_ID3D12Device,0x13f7b9bd,0x7113,0x46ff,0xac,0x01,0xb3,0xe1,0x09,0x21,0xf2,0x3d);


extern RPC_IF_HANDLE __MIDL_itf_d3d12_0000_0017_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d12_0000_0017_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


