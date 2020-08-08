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

		bool					create( GraphicsSystem& graphicsSystem, uint size, bool dynamic, BaseBufferTypes binding, const void* pInitData, const char* pDebugName );
		void					dispose( GraphicsSystem& graphicsSystem );

	private:

		ID3D12Resource*			m_pBuffer;
		mutable DynamicBuffer	m_dynamicBuffer;
		DynamicBufferTypes		m_dynamicBufferType;
		uint					m_dynamicBufferSize;

		bool					m_isDynamic;

	};
}

#endif // TIKI_BASEBUFFER_D3D12_HPP__INCLUDED__
