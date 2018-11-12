#pragma once

#include "tiki/graphics/graphics_state_object.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "../../../source/win_d3d11/graphics_states_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_D3D12 )
#	include "../../../source/win_d3d12/graphics_states_d3d12.hpp"
#elif  TIKI_ENABLED( TIKI_GRAPHICS_VULKAN )
#	include "../../../source/global_vulkan/graphics_states_vulkan.hpp"
#else
#	error Platform not supported
#endif

namespace tiki
{
	enum FillMode
	{
		FillMode_Wireframe,
		FillMode_Solid,

		FillMode_Count
	};

	enum CullMode
	{
		CullMode_None,
		CullMode_Front,
		CullMode_Back,

		CullMode_Count
	};

	enum WindingOrder
	{
		WindingOrder_Clockwise,
		WindingOrder_CounterClockwise,

		WindingOrder_Count
	};

	struct RasterizerStateParamters
	{
		RasterizerStateParamters()
		{
			fillMode		= FillMode_Solid;
			cullMode		= CullMode_Back;
			windingOrder	= WindingOrder_Clockwise;
		}

		FillMode		fillMode;
		CullMode		cullMode;
		WindingOrder	windingOrder;
	};

	class RasterizerState : public GraphicsStateObject
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( RasterizerState );
		friend class GraphicsContext;
		friend class GraphicsSystem;

	public:

		bool						isCreated() const;

	private: // friends

		bool						create( GraphicsSystem& graphicsSystem, const RasterizerStateParamters& creationParamter );
		void						dispose( GraphicsSystem& graphicsSystem );

	private:

		RasterizerStatePlatformData	m_platformData;
	};
}
