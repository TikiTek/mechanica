#pragma once
#ifndef __TIKI_SHADER_OPENGL4_HPP_INCLUDED__
#define __TIKI_SHADER_OPENGL4_HPP_INCLUDED__

#include "../../../source/global_opengl4/graphicstypes_opengl4.hpp"

namespace tiki
{
	struct ShaderConstantInfo
	{
		uint32		slotIndex;
		uint32		nameLenght;
		char		aName[ 1u ];
	};

	struct ShaderPlatformData
	{
		GLuint						shaderId;

		const ShaderConstantInfo*	apConstants[ GraphicsSystemLimits_VertexShaderConstantSlots + GraphicsSystemLimits_PixelShaderConstantSlots ];
		uint						constantCount;
	};

	namespace graphics
	{
		void fillShaderConstantMapping( GLuint* pMapping, uint capacity, GLuint programId, const ShaderPlatformData& platformData );
	}
}

#endif // __TIKI_SHADER_OPENGL4_HPP_INCLUDED__
