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

namespace tiki
{
	// TG = TikiGraphics
	typedef	DXGI_FORMAT						TGFormat;

	typedef	ID3D10Blob						TGBlob;

	typedef	IDXGISwapChain					TGSwapChain;
	typedef	ID3D11Device					TGDevice;
	typedef	ID3D11DeviceContext				TGContext;

	typedef	ID3D11Resource					TGResource;
	typedef ID3D11Buffer					TGBuffer;
	typedef ID3D11DeviceChild				TGDeviceChild;
	typedef ID3D11ShaderResourceView		TGShaderResourceView;

	typedef	D3D11_TEXTURE1D_DESC			TGTexture1DDesc;
	typedef	D3D11_TEXTURE2D_DESC			TGTexture2DDesc;
	typedef	D3D11_TEXTURE3D_DESC			TGTexture3DDesc;
	typedef	ID3D11Texture1D					TGTexture1D;
	typedef	ID3D11Texture2D					TGTexture2D;
	typedef	ID3D11Texture3D					TGTexture3D;

	typedef D3D11_DEPTH_STENCIL_VIEW_DESC	TGDepthStencilDescription;
	typedef D3D11_RENDER_TARGET_VIEW_DESC	TGRenderTargetDescription;
	typedef ID3D11DepthStencilView			TGDepthStencilView;
	typedef ID3D11RenderTargetView			TGRenderTargetView;

	typedef D3D11_INPUT_ELEMENT_DESC		TGInputElementDesc;
	typedef ID3D11InputLayout				TGInputLayout;

	typedef ID3D11ComputeShader				TGComputeShader;
	typedef ID3D11DomainShader				TGDomainShader;
	typedef ID3D11GeometryShader			TGGeometryShader;
	typedef ID3D11HullShader				TGHullShader;
	typedef ID3D11PixelShader				TGPixelShader;
	typedef ID3D11VertexShader				TGVertexShader;
	
	typedef ID3D11BlendState				TGBlendState;
	typedef ID3D11DepthStencilState			TGDepthStencilState;
	typedef ID3D11RasterizerState			TGRasterizerState;
	typedef ID3D11SamplerState				TGSamplerState;
	
}

#endif // TIKI_GRAPHICSTYPES_HPP