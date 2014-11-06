#ifndef TIKI_TEXTUREDATA_D3D12_HPP__INCLUDED__
#define TIKI_TEXTUREDATA_D3D12_HPP__INCLUDED__

#include "graphicstypes_d3d12.hpp"

namespace tiki
{
	struct TextureDataPlatformData
	{
		TextureDataPlatformData()
		{
			pResource	= nullptr;
		}

		ID3D12Resource*	pResource;
	};

	namespace graphics
	{
		DXGI_FORMAT getD3dFormat( PixelFormat pixelFormat, TextureFlags flags );
	}
}

#endif // TIKI_TEXTUREDATA_D3D12_HPP__INCLUDED__
