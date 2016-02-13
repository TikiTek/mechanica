#pragma once
#ifndef TIKI_GRAPHICSTYPES_HPP
#define TIKI_GRAPHICSTYPES_HPP

struct ID3D10Blob;

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;

struct ID3D11Buffer;
struct ID3D11DeviceChild;
struct ID3D11Resource;
struct ID3D11ShaderResourceView;

struct D3D11_TEXTURE1D_DESC;
struct D3D11_TEXTURE2D_DESC;
struct D3D11_TEXTURE3D_DESC;
struct ID3D11Texture1D;
struct ID3D11Texture2D;
struct ID3D11Texture3D;

struct D3D11_DEPTH_STENCIL_VIEW_DESC;
struct D3D11_RENDER_TARGET_VIEW_DESC;
struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;

struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;

struct ID3D11ComputeShader;
struct ID3D11DomainShader;
struct ID3D11GeometryShader;
struct ID3D11HullShader;
struct ID3D11PixelShader;
struct ID3D11VertexShader;

struct ID3D11BlendState;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
struct ID3D11SamplerState;

enum DXGI_FORMAT;

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
#	define TIKI_SET_DX_OBJECT_NAME( pObject, pName ) D3D_SET_OBJECT_NAME_A( pObject, pName )
#else
#	define TIKI_SET_DX_OBJECT_NAME( pObject, pName )
#endif

#endif // TIKI_GRAPHICSTYPES_HPP
