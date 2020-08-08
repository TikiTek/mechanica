#pragma once

#include "tiki/base/types.hpp"

#include "graphics_types_d3d11.hpp"

namespace tiki
{
	class GraphicsSystem;

	class BaseBuffer
	{
		friend class GraphicsSystem;
		friend class GraphicsContext;

	public:

								~BaseBuffer();

	protected:

								BaseBuffer();

		bool					create( GraphicsSystem& graphicsSystem, uint size, bool dynamic, BaseBufferTypes binding, const void* pInitData, const char* pDebugName );
		void					dispose( GraphicsSystem& graphicsSystem );

		uint					getSize() const;

	private:

		ID3D11Buffer*			m_pBuffer;
		uint					m_size;
		bool					m_dynamic;
	};
}
