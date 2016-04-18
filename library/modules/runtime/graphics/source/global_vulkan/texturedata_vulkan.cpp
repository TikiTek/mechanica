
#include "tiki/graphics/texturedata.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/base/memory.hpp"

#include "graphicssystem_internal_vulkan.hpp"

namespace tiki
{
	TextureData::TextureData()
	{
	}

	TextureData::~TextureData()
	{
	}

	bool TextureData::create( GraphicsSystem& graphicsSystem, const TextureDescription& description, const void* pTextureData /* = nullptr */, const char* pDebugName /* = nullptr */ )
	{
		return false;
	}

	void TextureData::dispose( GraphicsSystem& graphicsSystem )
	{
	}
}