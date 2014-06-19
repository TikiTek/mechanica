#pragma once
#ifndef __TIKI_BASEBUFFER_OPENGL4_HPP_INCLUDED__
#define __TIKI_BASEBUFFER_OPENGL4_HPP_INCLUDED__

#include "tiki/base/inline.hpp"
#include "tiki/base/types.hpp"

#include "graphicstypes_opengl4.hpp"

namespace tiki
{
	class GraphicsSystem;

	class BaseBuffer
	{
		friend class GraphicsContext;

	protected:

								BaseBuffer();
								~BaseBuffer();

		bool					create( GraphicsSystem& graphicsSystem, size_t size, bool dynamic, GraphicsBufferType binding, const void* pInitData = nullptr );
		void					dispose( GraphicsSystem& graphicsSystem );
		
	private:

		GLuint					m_bufferId;
		GLenum					m_bufferType;
		GLenum					m_bufferUsage;

	};
}

#endif // __TIKI_BASEBUFFER_OPENGL4_HPP_INCLUDED__
