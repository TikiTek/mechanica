#pragma once

#include "tiki/resource/resource.hpp"

#include "tiki/container/array.hpp"
#include "tiki/graphics/texture_data.hpp"
#include "tiki/graphics/font_char.hpp"

namespace tiki
{
    class ResourceManager;

	struct Vector2;

	class Font : public Resource
	{
		TIKI_DEFINE_RESOURCE( Font, TIKI_FOURCC( 'F', 'O', 'N', 'T' ) );
		TIKI_NONCOPYABLE_CLASS( Font );

	public:

		static void				registerResourceType( ResourceManager& resourceManager, GraphicsSystem& graphicsSystem );
		static void				unregisterResourceType( ResourceManager& resourceManager );

		const TextureData&		getTextureData() const { return m_textureData; }

		const FontChar&			getChar( char c ) const;

		void					calcuateTextSize( Vector2& textSize, const char* pText, uint textLength ) const;
		void					fillVertices( FontChar* pChars, uint capacity, const char* pText, uint textLength ) const;

	protected:

		virtual bool			createInternal( const ResourceInitData& initData, const FactoryContext& factoryContext );
		virtual void			disposeInternal( const FactoryContext& factoryContext );

	private:

								Font();
		virtual					~Font();

		TextureData				m_textureData;
		Array< FontChar >		m_chars;
	};
}
