#ifndef TIKI_BUFFER_HPP__
#define TIKI_BUFFER_HPP__

#include "tiki/base/inline.hpp"
#include "tiki/base/types.hpp"

#include "graphicstypes.hpp"

namespace tiki
{
	class GraphicsSystem;

	enum GraphicsBufferType
	{
		GraphicsBufferType_ConstantBuffer,
		GraphicsBufferType_IndexBuffer,
		GraphicsBufferType_VertexBuffer
	};

	class BaseBuffer
	{
		friend class GraphicsContext;

	protected:

								BaseBuffer();
								~BaseBuffer();

		void					create( GraphicsSystem& graphicsSystem, size_t size, bool dynamic, GraphicsBufferType binding, const void* pInitData = nullptr );
		void					dispose();
		
	private:

		TGBuffer*				m_pBuffer;
		bool					m_dynamic;

	};
}

#endif // TIKI_BUFFER_HPP__
