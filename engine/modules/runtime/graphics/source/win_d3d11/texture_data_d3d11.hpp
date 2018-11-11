#pragma once
#ifndef TIKI_TEXTUREDATA_D3D11_HPP__INCLUDED
#define TIKI_TEXTUREDATA_D3D11_HPP__INCLUDED

#include "graphicstypes_d3d11.hpp"

namespace tiki
{
	struct TextureDataPlatformData
	{
		TextureDataPlatformData()
		{
			pResource	= nullptr;
			pShaderView	= nullptr;
		}

		union
		{
			ID3D11Resource*			pResource;
			ID3D11Texture1D*		pTexture1d;
			ID3D11Texture2D*		pTexture2d;
			ID3D11Texture3D*		pTexture3d;
		};

		ID3D11ShaderResourceView*	pShaderView;		
	};

	namespace graphics
	{
		DXGI_FORMAT getD3dFormat( PixelFormat pixelFormat, TextureFlags flags );
	}
}

#endif // TIKI_TEXTUREDATA_D3D11_HPP__INCLUDED
