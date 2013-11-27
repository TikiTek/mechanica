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
	class VertexFormat;
	class VertexInputBinding;

	class Shader
	{
		friend class GraphicsContext;
		friend class GraphicsSystem;

	public:
		
		ShaderType				getShaderType() const { return m_type; }

	protected:

		bool					create( GraphicsSystem& graphicsSystem, const void* pData, uint dataSize );
		void					dispose();

		bool					createInputBinding( VertexInputBinding& inputBinding, const VertexFormat* pVertexFormat ) const;

	private:

								Shader();
		virtual					~Shader();

		ShaderType				m_type;
		ShaderPlatformData		m_platformData;

	};
}

#endif // TIKI_SHADER_HPP