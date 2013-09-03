#pragma once
#ifndef TIKI_GRAPHICSTYPES_HPP
#define TIKI_GRAPHICSTYPES_HPP

struct ID3D10Blob;

struct ID3D11Device;
struct ID3D11DeviceContext;

struct ID3D11Resource;

struct ID3D11Texture1D;
struct D3D11_TEXTURE1D_DESC;

struct ID3D11Texture2D;
struct D3D11_TEXTURE2D_DESC;

struct ID3D11Texture3D;
struct D3D11_TEXTURE3D_DESC;

struct ID3D11InputLayout;

struct D3D11_INPUT_ELEMENT_DESC;

struct ID3D11VertexShader;
struct ID3D11PixelShader;

struct ID3D11Buffer;

struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;
struct D3D11_DEPTH_STENCIL_VIEW_DESC;
struct D3D11_RENDER_TARGET_VIEW_DESC;

struct ID3D11ShaderResourceView;

struct ID3D11SamplerState;

namespace tiki
{
	// TG = TikiGraphics
	typedef	ID3D10Blob						TGBlob;

	typedef	ID3D11Device					TGDevice;
	typedef	ID3D11DeviceContext				TGContext;

	typedef	ID3D11Resource					TGResource;

	typedef	ID3D11Texture1D					TGTexture1D;
	typedef	D3D11_TEXTURE1D_DESC			TGTexture1DDesc;

	typedef	ID3D11Texture2D					TGTexture2D;
	typedef	D3D11_TEXTURE2D_DESC			TGTexture2DDesc;

	typedef	ID3D11Texture3D					TGTexture3D;
	typedef	D3D11_TEXTURE3D_DESC			TGTexture3DDesc;

	typedef D3D11_INPUT_ELEMENT_DESC		TGInputElementDesc;
	typedef ID3D11InputLayout				TGInputLayout;

	typedef ID3D11VertexShader				TGVertexShader;
	typedef ID3D11PixelShader				TGPixelShader;

	typedef ID3D11Buffer					TGBuffer;

	typedef ID3D11DepthStencilView			TGDepthStencil;
	typedef ID3D11RenderTargetView			TGRenderTarget;
	typedef D3D11_DEPTH_STENCIL_VIEW_DESC	TGDepthStencilDescription;
	typedef D3D11_RENDER_TARGET_VIEW_DESC	TGRenderTargetDescription;

	typedef ID3D11ShaderResourceView		TGShaderResource;

	typedef ID3D11SamplerState				TGSamplerState;

	// TODO: wrap
	//enum D3D11_BIND_FLAG
	//{	D3D11_BIND_VERTEX_BUFFER	= 0x1L,
	//D3D11_BIND_INDEX_BUFFER	= 0x2L,
	//D3D11_BIND_CONSTANT_BUFFER	= 0x4L,
	//D3D11_BIND_SHADER_RESOURCE	= 0x8L,
	//D3D11_BIND_STREAM_OUTPUT	= 0x10L,
	//D3D11_BIND_RENDER_TARGET	= 0x20L,
	//D3D11_BIND_DEPTH_STENCIL	= 0x40L,
	//D3D11_BIND_UNORDERED_ACCESS	= 0x80L
	//} 	D3D11_BIND_FLAG;
}

#endif // TIKI_GRAPHICSTYPES_HPP
