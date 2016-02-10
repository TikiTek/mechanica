#ifndef TIKI_BASEBUFFER_D3D12_HPP__INCLUDED__
#define TIKI_BASEBUFFER_D3D12_HPP__INCLUDED__

#include "tiki/base/types.hpp"

#include "graphicstypes_d3d12.hpp"

namespace tiki
{
	class GraphicsSystem;

	class BaseBuffer
	{
		friend class GraphicsContext;

	protected:

								BaseBuffer();
								~BaseBuffer();

		bool					create( GraphicsSystem& graphicsSystem, uint size, bool dynamic, GraphicsBufferType binding, const void* pInitData = nullptr );
		void					dispose( GraphicsSystem& graphicsSystem );
		
	private:

		ID3D12Resource*			m_pBuffer;
		DescriptorHandle		m_descriptorHandle;
		bool					m_dynamic;

	};
}

#endif // TIKI_BASEBUFFER_D3D12_HPP__INCLUDED__
