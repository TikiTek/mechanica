#pragma once
#ifndef __TIKI_SHADERSET_HPP_INCLUDED__
#define __TIKI_SHADERSET_HPP_INCLUDED__

#include "tiki/base/sortedsizedmap.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/shadertype.hpp"
#include "tiki/resource/resource.hpp"

namespace tiki
{
	class GraphicsSystem;
	class Shader;

	class ShaderSet : public Resource
	{
		TIKI_DEFINE_RESOURCE( ShaderSet, TIKI_FOURCC( 'T', 'G', 'S', 'S' ) );

	public:

		static void								registerResourceType( ResourceManager& resourceManager, GraphicsSystem& graphicsSystem );
		static void								unregisterResourceType( ResourceManager& resourceManager );

		const Shader*							getShader( ShaderType type, crc32 variantKey ) const;

	protected:

		virtual bool							createInternal( const ResourceInitData& initData, const FactoryContext& factoryContext );
		virtual void							disposeInternal( const FactoryContext& factoryContext );

	private:

												ShaderSet();
		virtual									~ShaderSet();

		Array< Shader >							m_shaders;
		SortedSizedMap< crc32, const Shader* >	m_shaderMap;

	};
}

#endif // __TIKI_SHADERSET_HPP_INCLUDED__
