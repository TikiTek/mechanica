#pragma once

#include "graphics_types_d3d11.hpp"

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
		DXGI_FORMAT getD3dFormat( PixelFormat pixelFormat, TextureFlags flags, bool swapChain );
	}
}
