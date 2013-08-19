#pragma once
#ifndef TIKI_FONT_HPP__INCLUDED
#define TIKI_FONT_HPP__INCLUDED

#include "tiki/resource/resource.hpp"

#include "tiki/base/array.hpp"
#include "tiki/base/structs.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/graphicsbase/fontchar.hpp"

#include "graphicstypes.hpp"

namespace tiki
{
	class FontFactory;

	class Font : public Resource
	{
		TIKI_DEFINE_RESOURCE( FontFactory, TIKI_FOURCC( 'F', 'O', 'N', 'T' ) );
		TIKI_NONCOPYABLE_CLASS( Font );

	public:

		friend class GpuContext;

		const TextureData&		getTextureData() const { return m_textureData; }	
		void					fillVertices( FontChar* pChars, size_t vertexStride, size_t repeateChar, cstring text, size_t capacity ) const;

	protected:

								Font();
		virtual					~Font();

		bool					initialize( GraphicsSystem& graphicsSystem, const void* pInitData );
		void					dispose();

	private:

		TextureData				m_textureData;
		Array< FontChar >		m_chars;

	};
}

#endif // TIKI_FONT_HPP__INCLUDED
