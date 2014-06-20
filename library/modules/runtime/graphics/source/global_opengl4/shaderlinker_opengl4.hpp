#pragma once
#ifndef __TIKI_SHADERLINKER_OPENGL4_HPP_INCLUDED__
#define __TIKI_SHADERLINKER_OPENGL4_HPP_INCLUDED__

#include "tiki/base/sortedsizedmap.hpp"

namespace tiki
{
	class ShaderLinker
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( ShaderLinker );

	public:

		bool		create( uint maxProgrammCount );
		void		dispose();

		GLuint		findOrCreateProgram( GLuint vertexShaderId, GLuint pixelShaderId );
		void		freeShader( ShaderType type, GLuint shaderId );

	private:

		struct LinkedProgram
		{
			GLuint	shaderIds[ ShaderType_Count ];

			GLuint	programShaderId;
		};
		typedef SortedSizedMap< crc32, LinkedProgram > ProgramMap;

		ProgramMap	m_programs;

	};
}

#endif // __TIKI_SHADERLINKER_OPENGL4_HPP_INCLUDED__
