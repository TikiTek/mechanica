#pragma once

#include "tiki/resource/resource.hpp"

#include "tiki/graphics/texture_data.hpp"

namespace tiki
{
	class Texture : public Resource
	{
		friend class GraphicsContext;
		TIKI_DEFINE_RESOURCE( Texture, TIKI_FOURCC( 'T', 'E', 'X', 'R' ) );
		TIKI_NONCOPYABLE_CLASS( Texture );

	public:

		static void				registerResourceType( ResourceManager& resourceManager, GraphicsSystem& graphicsSystem );
		static void				unregisterResourceType( ResourceManager& resourceManager );

		const TextureData&		getTextureData() const { return m_textureData; }

	protected:

		virtual bool			createInternal( const ResourceInitData& initData, const FactoryContext& factoryContext );
		virtual void			disposeInternal( const FactoryContext& factoryContext );

	private:

								Texture();
		virtual					~Texture();

		TextureData				m_textureData;
	};
}
