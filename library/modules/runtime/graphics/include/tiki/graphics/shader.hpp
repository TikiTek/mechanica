#pragma once
#ifndef TIKI_SHADER_HPP
#define TIKI_SHADER_HPP

#include "tiki/base/types.hpp"
#include "tiki/graphics/shadertype.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "../../../source/win_d3d11/shader_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_D3D12 )
#	include "../../../source/win_d3d12/shader_d3d12.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_OPENGL4 )
#	include "../../../source/global_opengl4/shader_opengl4.hpp"
#endif

namespace tiki
{
    class GraphicsSystem;

	class Shader
	{
		TIKI_NONCOPYABLE_CLASS( Shader );
		friend class GraphicsContext;
		friend class GraphicsSystem;
		friend class VertexInputBinding;
		template<typename T>
		friend class PoolAllocator;

	public:

								Shader();
								~Shader();

		bool					create( GraphicsSystem& graphicsSystem, ShaderType type, const void* pData, uint dataSize );
		void					dispose( GraphicsSystem& graphicsSystem );

		ShaderType				getShaderType() const { return m_type; }
		crc32					getShaderHash() const { return m_hash; }

	private:

		ShaderType				m_type;
		crc32					m_hash;

		ShaderPlatformData		m_platformData;

	};
}

#endif // TIKI_SHADER_HPP
