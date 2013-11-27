#pragma once
#ifndef __TIKI_SHADERSET_HPP_INCLUDED__
#define __TIKI_SHADERSET_HPP_INCLUDED__

#include "tiki/base/sortedsizedmap.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphicsbase/shadertype.hpp"
#include "tiki/resource/resource.hpp"

namespace tiki
{
	class GraphicsSystem;
	class Shader;

	class ShaderSet : public Resource
	{
		TIKI_DEFINE_RESOURCE( ShaderSetFactory, TIKI_FOURCC( 'T', 'G', 'S', 'S' ) );

		const Shader*	getShader( ShaderType type, crc32 variantKey ) const;

	private:

		ShaderSet();
		~ShaderSet();

		bool	create( GraphicsSystem& graphicsSystem, const void* pInitData, uint initDataSize );
		void	dispose();

	private:

		SortedSizedMap<crc32, const Shader*>	m_shaders;

	};
}

#endif // __TIKI_SHADERSET_HPP_INCLUDED__
