#pragma once
#ifndef TIKI_SHADER_HPP
#define TIKI_SHADER_HPP

#include "tiki/base/types.hpp"
#include "tiki/graphicsbase/shadertype.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "win_d3d11/shader_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_OPENGL4 )
#	include "global_opengl4/shader_opengl4.hpp"
#endif

namespace tiki
{
	class Shader
	{
		friend class GraphicsContext;
		friend class GraphicsSystem;
		friend class VertexInputBinding;
		template<typename T>
		friend class PoolAllocator;

	public:
		
		ShaderType				getShaderType() const { return m_type; }
		crc32					getShaderHash() const;

	private: // friend

		bool					create( GraphicsSystem& graphicsSystem, ShaderType type, const void* pData, uint dataSize );
		void					dispose();

	private:

								Shader();
		virtual					~Shader();

		ShaderType				m_type;
		ShaderPlatformData		m_platformData;

	};
}

#endif // TIKI_SHADER_HPP