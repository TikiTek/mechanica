#pragma once

#include "tiki/base/types.hpp"
#include "tiki/graphics/graphics_state_object.hpp"
#include "tiki/graphics/graphics_system_limits.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "../../../source/win_d3d11/graphics_states_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_D3D12 )
#	include "../../../source/win_d3d12/graphics_states_d3d12.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_VULKAN )
#	include "../../../source/global_vulkan/graphics_states_vulkan.hpp"
#else
#	error Platform not supported
#endif

namespace tiki
{
	class Shader;
	class VertexFormat;

	struct VertexInputBindingParameters
	{
		const VertexFormat*	pVertexFormat;
		const Shader*		pShader;
	};

	class VertexInputBinding : public GraphicsStateObject
	{
		TIKI_NONCOPYABLE_CLASS( VertexInputBinding );
		friend class GraphicsContext;
		friend class GraphicsSystem;

	public:

										VertexInputBinding();
		virtual							~VertexInputBinding();

		bool							isCreated() const { return m_pShader != nullptr; }

		const VertexFormat*				getVertexFormat() const { return m_pVertexFormat; }

	private: // friends

		bool							create( GraphicsSystem& graphicsSystem, const VertexInputBindingParameters& creationParameters );
		void							dispose( GraphicsSystem& graphicsSystem );

	private:

		const VertexFormat*				m_pVertexFormat;
		const Shader*					m_pShader;
		VertexInputBindingPlatformData	m_platformData;
	};
}
