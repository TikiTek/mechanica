#pragma once
#ifndef TIKI_FONT_HPP__INCLUDED
#define TIKI_FONT_HPP__INCLUDED

#include "tiki/resource/resource.hpp"

#include "tiki/base/array.hpp"
#include "tiki/base/structs.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/graphicsbase/fontchar.hpp"

namespace tiki
{
	class FontFactory;

	class Font : public Resource
	{
		TIKI_DEFINE_RESOURCE( Font, TIKI_FOURCC( 'F', 'O', 'N', 'T' ) );
		TIKI_NONCOPYABLE_CLASS( Font );

	public:

		static void				registerResourceType( ResourceManager& resourceManager, GraphicsSystem& graphicsSystem );
		static void				unregisterResourceType( ResourceManager& resourceManager );

		const TextureData&		getTextureData() const { return m_textureData; }	
		void					fillVertices( FontChar* pChars, size_t capacity, cstring text, size_t textLength ) const;

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

#endif // TIKI_FONT_HPP__INCLUDED
