
#include "shaderlinker_opengl4.hpp"

namespace tiki
{
	bool ShaderLinker::create( uint maxProgrammCount )
	{
		if ( !m_programs.create( maxProgrammCount ) )
		{
			return false;
		}

		return true;
	}

	void ShaderLinker::dispose()
	{
		for (uint i = 0u; i < m_programs.getCount(); ++i)
		{
			const ProgramMap::Pair& kvp = m_programs.getPairAt( i );
			glDeleteProgram( kvp.value.programShaderId );
		}

		m_programs.dispose();
	}

	crc32 ShaderLinker::getProgramCrc( GLuint vertexShaderId, GLuint pixelShaderId ) const
	{
		const GLuint aShaderIds[] = { vertexShaderId, pixelShaderId };
		const crc32 key = crcBytes( aShaderIds, sizeof( aShaderIds ) );

		return key;
	}

	GLuint ShaderLinker::findOrCreateProgram( GLuint vertexShaderId, GLuint pixelShaderId )
	{
		const crc32 key = getProgramCrc( pixelShaderId, vertexShaderId );

		LinkedProgram program;
		if ( m_programs.findValue( &program, key ) )
		{
			return program.programShaderId;
		}

		TIKI_ASSERT( glIsShader( vertexShaderId ) );
		TIKI_ASSERT( glIsShader( pixelShaderId ) );

		program.shaderIds[ ShaderType_Effect ]			= 0u;
		program.shaderIds[ ShaderType_VertexShader ]	= vertexShaderId;
		program.shaderIds[ ShaderType_PixelShader ]		= pixelShaderId;
		program.shaderIds[ ShaderType_GeometrieShader ]	= 0u;
		program.shaderIds[ ShaderType_HullShader ]		= 0u;
		program.shaderIds[ ShaderType_DomainShader ]	= 0u;
		program.shaderIds[ ShaderType_ComputeShader ]	= 0u;

		program.programShaderId = glCreateProgram();
		glAttachShader( program.programShaderId, vertexShaderId );
		glAttachShader( program.programShaderId, pixelShaderId );
		glLinkProgram( program.programShaderId );

		int lenght = 0;
		glGetProgramiv( program.programShaderId, GL_INFO_LOG_LENGTH, &lenght );
		if ( lenght > 1 )
		{
			int charsWritten  = 0;
			char buffer[ 4096u ];
			glGetProgramInfoLog( program.programShaderId, sizeof( buffer ), &charsWritten, buffer );

			TIKI_TRACE_ERROR( "[graphics] Cound not Link program. Errors:\n%s\n", buffer );

			glDeleteProgram( program.programShaderId );
			return 0u;
		}

		if ( !graphics::checkError() )
		{
			glDeleteProgram( program.programShaderId );
			return 0u;		
		}
		
		m_programs.set( key, program );
		return program.programShaderId;
	}

	void ShaderLinker::freeShader( ShaderType type, GLuint shaderId )
	{
		for (uint i = 0u; i < m_programs.getCount(); ++i)
		{
			const ProgramMap::Pair& kvp = m_programs.getPairAt( i );

			if ( kvp.value.shaderIds[ type ] == shaderId )
			{
				glDeleteProgram( kvp.value.programShaderId );
				m_programs.remove( kvp.key );
				return;
			}
		} 
	}
}