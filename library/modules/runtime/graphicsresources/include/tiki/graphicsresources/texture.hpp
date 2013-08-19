#pragma once
#ifndef TIKI_TEXTURE_HPP
#define TIKI_TEXTURE_HPP

#include "tiki/resource/resource.hpp"

#include "tiki/graphics/texturedata.hpp"

namespace tiki
{
	class TextureFactory;

	class Texture : public Resource
	{
		friend class GpuContext;

		TIKI_DEFINE_RESOURCE( TextureFactory, TIKI_FOURCC( 'T', 'E', 'X', 'R' ) );
		TIKI_NONCOPYABLE_CLASS( Texture );

	public:

		const TextureData&		getTextureData() const { return m_textureData; }

	protected:

		Texture();
		virtual					~Texture();

		bool					initialize( GraphicsSystem& graphicsSystem, const TextureDescription& description, const void* pInitData );
		void					dispose();

	private:

		TextureData				m_textureData;

	};
}

#endif // TIKI_TEXTURE_HPP
