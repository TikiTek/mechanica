#pragma once

#include "tiki/base/types.hpp"
#include "tiki/graphics/shader_type.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "../../../source/win_d3d11/shader_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_D3D12 )
#	include "../../../source/win_d3d12/shader_d3d12.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_VULKAN )
#	include "../../../source/global_vulkan/shader_vulkan.hpp"
#endif

namespace tiki
{
    class GraphicsSystem;

	class Shader
	{
		TIKI_NONCOPYABLE_CLASS( Shader );
		friend class GraphicsContext;
		friend class GraphicsSystem;
		friend class VertexInputBinding;

	public:

								Shader();
								~Shader();

		bool					create( GraphicsSystem& graphicsSystem, ShaderType type, const void* pData, uint dataSize );
		void					dispose( GraphicsSystem& graphicsSystem );

		ShaderType				getShaderType() const { return m_type; }
		crc32					getShaderHash() const { return m_hash; }

	private:

		ShaderType				m_type;
		crc32					m_hash;

		ShaderPlatformData		m_platformData;
	};
}
