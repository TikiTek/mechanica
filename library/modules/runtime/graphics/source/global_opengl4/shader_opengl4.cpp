
#include "tiki/graphics/shader.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/fixedsizedarray.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/shadertype.hpp"

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	static const GLenum s_aShaderTypeMapping[] =
	{
		GL_INVALID_ENUM,			// ShaderType_Effect
		GL_VERTEX_SHADER,			// ShaderType_VertexShader
		GL_FRAGMENT_SHADER,			// ShaderType_PixelShader
		GL_GEOMETRY_SHADER,			// ShaderType_GeometrieShader
		GL_TESS_EVALUATION_SHADER,	// ShaderType_HullShader
		GL_TESS_CONTROL_SHADER,		// ShaderType_DomainShader
		GL_COMPUTE_SHADER			// ShaderType_ComputeShader
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aShaderTypeMapping ) == ShaderType_Count );

	Shader::Shader()
	{
		m_type = ShaderType_Invalid;

		m_platformData.shaderId = GL_INVALID_ENUM;
	}

	Shader::~Shader()
	{
		TIKI_ASSERT( m_type == ShaderType_Invalid );

		TIKI_ASSERT( m_platformData.shaderId == GL_INVALID_ENUM );
	}

	bool Shader::create( GraphicsSystem& graphicsSystem, ShaderType type, const void* pInitData, uint dataSize )
	{
		TIKI_ASSERT( pInitData != nullptr );
		TIKI_ASSERT( dataSize > 0u );
		TIKI_ASSERT( m_platformData.shaderId == GL_INVALID_ENUM );

		m_type	= type;
		m_hash	= crcBytes( pInitData, dataSize );
		
		m_platformData.shaderId = glCreateShader( s_aShaderTypeMapping[ type ] );
		
		const char* pSourceCode = static_cast< const char* >( pInitData );
		glShaderSource( m_platformData.shaderId, 1, &pSourceCode, nullptr );
		glCompileShader( m_platformData.shaderId );

		int lenght = 0;
		glGetShaderiv( m_platformData.shaderId, GL_INFO_LOG_LENGTH, &lenght );

		if ( lenght > 1 )
		{
			int charsWritten  = 0;
			char buffer[ 4096u ];
			glGetShaderInfoLog( m_platformData.shaderId, sizeof( buffer ), &charsWritten, buffer );

			TIKI_TRACE_ERROR( pSourceCode );
			TIKI_TRACE_ERROR( buffer );

			return true; // todo
		}

		return true;
	}

	void Shader::dispose( GraphicsSystem& graphicsSystem )
	{
		m_type = ShaderType_Invalid;

		if ( m_platformData.shaderId != GL_INVALID_ENUM )
		{
			glDeleteShader( m_platformData.shaderId );
			m_platformData.shaderId = GL_INVALID_ENUM;
		}
	}
}