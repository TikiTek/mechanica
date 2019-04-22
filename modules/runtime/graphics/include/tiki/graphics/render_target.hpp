#pragma  once

#include "tiki/base/types.hpp"
#include "tiki/container/fixed_sized_array.hpp"
#include "tiki/graphics/graphics_system_limits.hpp"
#include "tiki/graphics/pixel_format.hpp"
#include "tiki/math/vector.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "../../../source/win_d3d11/render_target_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_D3D12 )
#	include "../../../source/win_d3d12/render_target_d3d12.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_VULKAN )
#	include "../../../source/global_vulkan/render_target_vulkan.hpp"
#endif

namespace tiki
{
	class GraphicsContext;
	class GraphicsSystem;
	class TextureData;

	class RenderTarget
	{
		TIKI_NONCOPYABLE_CLASS( RenderTarget );
		friend class GraphicsContext;
		friend class GraphicsSystem;

	public:

		RenderTarget();
		~RenderTarget();

		bool						create( GraphicsSystem& graphicsSystem, uint16 width, uint16 height, const TextureData** ppColorBuffers, uint colorBufferCount, const TextureData* pDepthBuffer );
		void						dispose( GraphicsSystem& graphicsSystem );

		uint16						getWidth() const							{ return m_width; }
		uint16						getHeight() const							{ return m_height; }
		Vector2						getVectorSize() const						{ return vector::create( float( m_width ), float( m_height ) ); }

		const TextureData*			getColorTextureData( uint index ) const		{ return m_colorBuffers[ index ]; }
		uint						getColorBufferCount() const					{ return m_colorBuffers.getCount(); }

		const TextureData*			getDepthTextureData() const					{ return m_pDepthBuffer; }

	private:

		using ColorBufferArray = FixedSizedArray< const TextureData*, GraphicsSystemLimits_RenderTargetSlots >;

		uint16						m_width;
		uint16						m_height;

		ColorBufferArray			m_colorBuffers;
		const TextureData*			m_pDepthBuffer;

		RenderTargetPlatformData	m_platformData;
	};
}
