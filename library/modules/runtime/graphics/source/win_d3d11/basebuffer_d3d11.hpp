#ifndef TIKI_BASEBUFFER_D3D11_HPP__
#define TIKI_BASEBUFFER_D3D11_HPP__

#include "tiki/base/types.hpp"

#include "graphicstypes_d3d11.hpp"

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

		bool					create( GraphicsSystem& graphicsSystem, uint size, bool dynamic, GraphicsBufferType binding, const void* pInitData, const char* pDebugName );
		void					dispose( GraphicsSystem& graphicsSystem );
		
		uint					getSize() const;

	private:

		ID3D11Buffer*			m_pBuffer;
		uint					m_size;
		bool					m_dynamic;

	};
}

#endif // TIKI_BASEBUFFER_D3D11_HPP__
