#ifndef TIKI_BASEBUFFER_D3D11_HPP__
#define TIKI_BASEBUFFER_D3D11_HPP__

#include "tiki/base/inline.hpp"
#include "tiki/base/types.hpp"

#include "../../../../source/win_d3d11/graphicstypes_d3d11.hpp"

namespace tiki
{
	class GraphicsSystem;

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

#endif // TIKI_BASEBUFFER_D3D11_HPP__