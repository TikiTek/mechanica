#ifndef TIKI_TEXTUREDATA_D3D12_HPP__INCLUDED__
#define TIKI_TEXTUREDATA_D3D12_HPP__INCLUDED__

#include "graphicstypes_d3d12.hpp"

namespace tiki
{
	struct TextureDataPlatformData
	{
		TextureDataPlatformData()
		{
			pResource		= nullptr;
			pDescriptorHeap	= nullptr;
		}

		ID3D12Resource*			pResource;
		ID3D12DescriptorHeap*	pDescriptorHeap;
	};
}

#endif // TIKI_TEXTUREDATA_D3D12_HPP__INCLUDED__
