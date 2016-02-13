#ifndef TIKI_BASEBUFFER_D3D12_HPP__INCLUDED__
#define TIKI_BASEBUFFER_D3D12_HPP__INCLUDED__

#include "tiki/base/types.hpp"

#include "tiki/container/fixedarray.hpp"

#include "graphicstypes_d3d12.hpp"

namespace tiki
{
	class GraphicsSystem;

	class BaseBuffer
	{
		friend class GraphicsSystem;
		friend class GraphicsContext;

	protected:

								BaseBuffer();
								~BaseBuffer();

		bool					create( GraphicsSystem& graphicsSystem, uint size, bool dynamic, GraphicsBufferType binding, const void* pInitData, const char* pDebugName );
		void					dispose( GraphicsSystem& graphicsSystem );

	private:

		typedef FixedArray< ID3D12Resource*, GraphicsSystemLimits_MaxFrameCount > BufferArray;

		BufferArray				m_buffers;
		bool					m_dynamic;

	};
}

#endif // TIKI_BASEBUFFER_D3D12_HPP__INCLUDED__
