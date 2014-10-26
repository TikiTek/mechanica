#ifndef TIKI_GRAPHICSCONTEXT_D3D12_HPP__INCLUDED__
#define TIKI_GRAPHICSCONTEXT_D3D12_HPP__INCLUDED__

#include "graphicstypes_d3d12.hpp"

namespace tiki
{
	struct GraphicsContextPlatformData
	{
		GraphicsContextPlatformData()
		{
			pCommandList		= nullptr;
			pDescriptionHeap	= nullptr;
		}

		ID3D12CommandList*		pCommandList;
		ID3D12DescriptorHeap*	pDescriptionHeap;
	};
}

#endif // TIKI_GRAPHICSCONTEXT_D3D12_HPP__INCLUDED__
