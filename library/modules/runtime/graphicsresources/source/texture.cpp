
#include "tiki/graphics/texture.hpp"

#include <d3d11.h>

namespace tiki
{
	Texture::Texture()
	{
	}

	Texture::~Texture()
	{
	}

	bool Texture::initialize( GraphicsSystem& graphicsSystem, const TextureDescription& description, const void* pInitData )
	{
		return m_textureData.create( graphicsSystem, description, pInitData );
	}

	void Texture::dispose()
	{
		m_textureData.dispose();
	}

}
