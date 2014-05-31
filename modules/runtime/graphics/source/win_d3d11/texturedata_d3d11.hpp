#pragma once
#ifndef TIKI_TEXTUREDATA_D3D11_HPP__INCLUDED
#define TIKI_TEXTUREDATA_D3D11_HPP__INCLUDED

#include "../../../../source/win_d3d11/graphicstypes_d3d11.hpp"

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
			TGResource*			pResource;
			TGTexture1D*		pTexture1d;
			TGTexture2D*		pTexture2d;
			TGTexture3D*		pTexture3d;
		};
		TGShaderResourceView*	pShaderView;		
	};

	namespace graphics
	{
		TGFormat getD3dFormat( PixelFormat pixelFormat, TextureFlags flags );
	}
}

#endif // TIKI_TEXTUREDATA_D3D11_HPP__INCLUDED
