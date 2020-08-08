#include "tiki/graphics/rasterizer_state.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/graphics/graphics_system.hpp"

#include <d3d11.h>

namespace tiki
{
	static const D3D11_FILL_MODE s_aFillModeMapping[ FillMode_Count ] =
	{
		D3D11_FILL_WIREFRAME,	// FillMode_Wireframe
		D3D11_FILL_SOLID		// FillMode_Solid
	};

	static const D3D11_CULL_MODE s_aCullModeMapping[ CullMode_Count ] =
	{
		D3D11_CULL_NONE,	// CullMode_None
		D3D11_CULL_FRONT,	// CullMode_Front
		D3D11_CULL_BACK,	// CullMode_Back
	};

	static const BOOL s_aWindingOrderMapping[ WindingOrder_Count ] =
	{
		FALSE,	// WindingOrder_Clockwise
		TRUE	// WindingOrder_CounterClockwise
	};

	bool RasterizerState::isCreated() const
	{
		return m_platformData.pRasterizerState != nullptr;
	}

	bool RasterizerState::create( GraphicsSystem& graphicsSystem, const RasterizerStateParamters& creationParamter )
	{
		TIKI_DECLARE_STACKANDZERO( D3D11_RASTERIZER_DESC, stateDesc );
		stateDesc.FillMode				= s_aFillModeMapping[ creationParamter.fillMode ];
		stateDesc.CullMode				= s_aCullModeMapping[ creationParamter.cullMode ];
		stateDesc.FrontCounterClockwise	= s_aWindingOrderMapping[ creationParamter.windingOrder ];
		stateDesc.DepthClipEnable		= TRUE;
		stateDesc.ScissorEnable			= TRUE;

		HRESULT result = GraphicsSystemPlatform::getDevice( graphicsSystem )->CreateRasterizerState( &stateDesc, &m_platformData.pRasterizerState );
		if( FAILED( result ) )
		{
			dispose( graphicsSystem );
			return false;
		}

		return m_platformData.pRasterizerState != nullptr;
	}

	void RasterizerState::dispose( GraphicsSystem& /*graphicsSystem*/ )
	{
		if ( m_platformData.pRasterizerState != nullptr )
		{
			m_platformData.pRasterizerState->Release();
			m_platformData.pRasterizerState = nullptr;
		}

		GraphicsStateObject::dispose();
	}
}
