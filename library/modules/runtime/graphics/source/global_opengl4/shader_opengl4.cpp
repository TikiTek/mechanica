
#include "tiki/graphics/shader.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/fixedsizedarray.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/shadertype.hpp"

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	struct ShaderInitializationData
	{
		uint32				constantCount;
		ShaderConstantInfo	constantInfo[ 1u ];
	};

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

		m_platformData.shaderId			= GL_INVALID_ENUM;
		m_platformData.constantCount	= 0u;

		for (uint i = 0u; i < TIKI_COUNT( m_platformData.apConstants ); ++i)
		{
			m_platformData.apConstants[ i ] = nullptr;
		}
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

		const ShaderInitializationData* pTypedInitData = static_cast< const ShaderInitializationData* >( pInitData );
		const void* pSourceCodeData = pInitData;

		m_platformData.constantCount = pTypedInitData->constantCount;
		if ( m_platformData.constantCount < TIKI_COUNT( m_platformData.apConstants ) )
		{
			const ShaderConstantInfo* pConatantInfo = pTypedInitData->constantInfo;
			for (uint i = 0u; i < m_platformData.constantCount; ++i)
			{
				m_platformData.apConstants[ i ] = pConatantInfo;

				pConatantInfo = addPtr( pConatantInfo, sizeof( pConatantInfo->slotIndex ) + sizeof( pConatantInfo->nameLenght ) + pConatantInfo->nameLenght + 1u );
			}

			pSourceCodeData = pConatantInfo;
		}
		
		m_platformData.shaderId = glCreateShader( s_aShaderTypeMapping[ type ] );
		
		const char* pSourceCode = static_cast< const char* >( pSourceCodeData );
		glShaderSource( m_platformData.shaderId, 1, &pSourceCode, nullptr );
		glCompileShader( m_platformData.shaderId );

		GLint status = 0;
		glGetShaderiv( m_platformData.shaderId, GL_COMPILE_STATUS, &status );
		if ( status == GL_FALSE )
		{
			GLint lenght = 0;
			glGetShaderiv( m_platformData.shaderId, GL_INFO_LOG_LENGTH, &lenght );
			if ( lenght > 1 )
			{
				int charsWritten  = 0;
				char aBuffer[ 4096u ];
				glGetShaderInfoLog( m_platformData.shaderId, sizeof( aBuffer ), &charsWritten, aBuffer );
				aBuffer[ charsWritten ] = '\0';

				TIKI_TRACE_ERROR( "Cound not compile Shader. Source Code:\n%s\nErrors:\n%s\n", pSourceCode, aBuffer );
			}

			dispose( graphicsSystem );
			return false;
		}

		return true;
	}

	void Shader::dispose( GraphicsSystem& graphicsSystem )
	{
		m_type = ShaderType_Invalid;

		if ( m_platformData.shaderId != GL_INVALID_ENUM )
		{
			graphics::getShaderLinker( graphicsSystem ).freeShader( m_type, m_platformData.shaderId );

			glDeleteShader( m_platformData.shaderId );
			m_platformData.shaderId = GL_INVALID_ENUM;
		}
	}

	void graphics::fillShaderConstantMapping( GLuint* pMapping, uint capacity, GLuint programId, const ShaderPlatformData& platformData )
	{
		for (uint i = 0u; i < platformData.constantCount; ++i)
		{
			const ShaderConstantInfo* pInfo = platformData.apConstants[ i ];
			TIKI_ASSERT( pInfo != nullptr );
			TIKI_ASSERT( pInfo->slotIndex < capacity );
			
			pMapping[ pInfo->slotIndex ] = glGetUniformBlockIndex( programId, pInfo->aName );
			TIKI_ASSERT( pMapping[ pInfo->slotIndex ] != GL_INVALID_INDEX );
		}		
	}
}