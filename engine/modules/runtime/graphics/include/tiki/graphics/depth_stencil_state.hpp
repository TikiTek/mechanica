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
	enum ComparsionFunction
	{
		ComparsionFunction_Never,
		ComparsionFunction_Less,
		ComparsionFunction_Equal,
		ComparsionFunction_LessOrEqual,
		ComparsionFunction_Greater,
		ComparsionFunction_NotEqual,
		ComparsionFunction_GreaterOrEqual,
		ComparsionFunction_Always,

		ComparsionFunction_Count
	};

	enum StencilOperation
	{
		StencilOperation_Keep,
		StencilOperation_Zero,
		StencilOperation_Replace,
		StencilOperation_Increase,
		StencilOperation_Decrease,

		StencilOperation_Count
	};

	struct StencilParameters
	{
		StencilParameters()
		{
			stencilFunction			= ComparsionFunction_Never;

			stencilPassOperation	= StencilOperation_Keep;
			stencilFailOperation	= StencilOperation_Keep;
			depthFailOperation		= StencilOperation_Keep;
		}

		ComparsionFunction	stencilFunction;

		StencilOperation	stencilFailOperation;
		StencilOperation	stencilPassOperation;
		StencilOperation	depthFailOperation;
	};

	struct DepthStencilStateParamters
	{
		DepthStencilStateParamters()
		{
			depthEnabled		= false;
			depthWriteEnabled	= false;
			depthFunction		= ComparsionFunction_LessOrEqual;

			stencilEnabled		= false;
			stencilReadMask		= 0u;
			stencilWriteMask	= 0u;
			stencilReference	= 0u;
		}

		bool				depthEnabled;
		bool				depthWriteEnabled;
		ComparsionFunction	depthFunction;

		bool				stencilEnabled;
		uint8				stencilReadMask;
		uint8				stencilWriteMask;
		uint8				stencilReference;

		StencilParameters	frontFace;
		StencilParameters	backFace;
	};

	class DepthStencilState : public GraphicsStateObject
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DepthStencilState );
		friend class GraphicsContext;
		friend class GraphicsSystem;

	public:

		bool							isCreated() const;

	private: // friends

		bool							create( GraphicsSystem& graphicsSystem, const DepthStencilStateParamters& creationParamter );
		void							dispose( GraphicsSystem& graphicsSystem );

	private:

		DepthStencilStatePlatformData	m_platformData;
	};
}
