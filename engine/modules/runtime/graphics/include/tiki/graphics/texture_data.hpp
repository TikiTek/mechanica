#pragma once

#include "tiki/graphics/texture_description.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "../../../source/win_d3d11/texture_data_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_D3D12 )
#	include "../../../source/win_d3d12/texture_data_d3d12.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_VULKAN )
#	include "../../../source/global_vulkan/texture_data_vulkan.hpp"
#endif

namespace tiki
{
	class GraphicsSystem;

	class TextureData
	{
		TIKI_NONCOPYABLE_CLASS( TextureData );
		friend class GraphicsContext;
		friend class GraphicsSystem;
		friend class RenderTarget;

	public:

		TextureData();
		~TextureData();

		bool						create( GraphicsSystem& graphicsSystem, const TextureDescription& description, const void* pTextureData = nullptr, const char* pDebugName = nullptr );
		void						dispose( GraphicsSystem& graphicsSystem );

		uint						getWidth() const		{ return m_description.width; }
		uint						getHeight() const		{ return m_description.height; }

		const TextureDescription&	getDescription() const	{ return m_description; }

	private:

		TextureDescription			m_description;
		TextureDataPlatformData		m_platformData;
	};
}
