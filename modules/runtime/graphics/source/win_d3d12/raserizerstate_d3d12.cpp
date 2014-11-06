
#include "tiki/graphics/blendstate.hpp"

#include "graphicssystem_internal_d3d12.hpp"

namespace tiki
{
	static const D3D12_FILL_MODE s_aFillModeMapping[] =
	{
		D3D12_FILL_WIREFRAME,	// FillMode_Wireframe
		D3D12_FILL_SOLID		// FillMode_Solid
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aFillModeMapping ) == FillMode_Count );

	static const D3D12_CULL_MODE s_aCullModeMapping[] =
	{
		D3D12_CULL_NONE,	// CullMode_None
		D3D12_CULL_FRONT,	// CullMode_Front
		D3D12_CULL_BACK,	// CullMode_Back
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aCullModeMapping ) == CullMode_Count );

	static const BOOL s_aWindingOrderMapping[] =
	{
		FALSE,	// WindingOrder_Clockwise
		TRUE	// WindingOrder_CounterClockwise
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aWindingOrderMapping ) == WindingOrder_Count );

	bool RasterizerState::isCreated() const
	{
		return m_platformData.isCreated;
	}

	bool RasterizerState::create( GraphicsSystem& graphicsSystem, const RasterizerStateParamters& creationParamter )
	{
		memory::zero( m_platformData.rasterizerDesc );
		m_platformData.rasterizerDesc.FillMode				= s_aFillModeMapping[ creationParamter.fillMode ];
		m_platformData.rasterizerDesc.CullMode				= s_aCullModeMapping[ creationParamter.cullMode ];
		m_platformData.rasterizerDesc.FrontCounterClockwise	= s_aWindingOrderMapping[ creationParamter.windingOrder ];
		m_platformData.rasterizerDesc.DepthClipEnable		= TRUE;

		m_platformData.isCreated = true;

		return true;
	}

	void RasterizerState::dispose( GraphicsSystem& /*graphicsSystem*/ )
	{
		m_platformData.isCreated = false;
		GraphicsStateObject::dispose( );
	}
}