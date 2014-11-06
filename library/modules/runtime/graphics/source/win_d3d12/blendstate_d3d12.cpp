
#include "tiki/graphics/blendstate.hpp"

#include "graphicssystem_internal_d3d12.hpp"

namespace tiki
{
	static const D3D12_BLEND s_aBlendMapping[] =
	{
		D3D12_BLEND_ZERO,			// Blend_Zero
		D3D12_BLEND_ONE,			// Blend_One
		D3D12_BLEND_SRC_COLOR,		// Blend_SourceColor
		D3D12_BLEND_INV_SRC_COLOR,	// Blend_InverseSourceColor
		D3D12_BLEND_SRC_ALPHA,		// Blend_SourceAlpha
		D3D12_BLEND_INV_SRC_ALPHA,	// Blend_InverseSourceAlpha
		D3D12_BLEND_DEST_COLOR,		// Blend_DestinationColor
		D3D12_BLEND_INV_DEST_COLOR,	// Blend_InverseDestinationColor
		D3D12_BLEND_DEST_ALPHA,		// Blend_DestinationAlpha
		D3D12_BLEND_INV_DEST_ALPHA,	// Blend_InverseDestinationAlpha
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aBlendMapping ) == Blend_Count );

	static const D3D12_BLEND_OP s_aBlendOperationMapping[] =
	{
		D3D12_BLEND_OP_ADD,			// BlendOperation_Add
		D3D12_BLEND_OP_SUBTRACT,	// BlendOperation_Subtract
		D3D12_BLEND_OP_MIN,			// BlendOperation_Min
		D3D12_BLEND_OP_MAX			// BlendOperation_Max
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aBlendOperationMapping ) == BlendOperation_Count );

	TIKI_COMPILETIME_ASSERT( ColorWriteMask_Red		== D3D12_COLOR_WRITE_ENABLE_RED );
	TIKI_COMPILETIME_ASSERT( ColorWriteMask_Green	== D3D12_COLOR_WRITE_ENABLE_GREEN );
	TIKI_COMPILETIME_ASSERT( ColorWriteMask_Blue	== D3D12_COLOR_WRITE_ENABLE_BLUE );
	TIKI_COMPILETIME_ASSERT( ColorWriteMask_Alpha	== D3D12_COLOR_WRITE_ENABLE_ALPHA );

	bool BlendState::isCreated() const
	{
		return m_platformData.isCreated;
	}

	bool BlendState::create( GraphicsSystem& graphicsSystem, const BlendStateParamters& creationParamter )
	{
		memory::zero( m_platformData.blendDesc );
		m_platformData.blendDesc.AlphaToCoverageEnable	= FALSE;
		m_platformData.blendDesc.IndependentBlendEnable	= FALSE;

		m_platformData.blendDesc.RenderTarget[ 0u ].BlendEnable				= creationParamter.blendEnabled;
		m_platformData.blendDesc.RenderTarget[ 0u ].SrcBlend				= s_aBlendMapping[ creationParamter.sourceBlend ];
		m_platformData.blendDesc.RenderTarget[ 0u ].DestBlend				= s_aBlendMapping[ creationParamter.destinationBlend ];
		m_platformData.blendDesc.RenderTarget[ 0u ].BlendOp					= s_aBlendOperationMapping[ creationParamter.operation ];

		m_platformData.blendDesc.RenderTarget[ 0u ].SrcBlendAlpha			= D3D12_BLEND_ONE;
		m_platformData.blendDesc.RenderTarget[ 0u ].DestBlendAlpha			= D3D12_BLEND_ZERO;
		m_platformData.blendDesc.RenderTarget[ 0u ].BlendOpAlpha			= D3D12_BLEND_OP_ADD;
		
		m_platformData.blendDesc.RenderTarget[ 0u ].RenderTargetWriteMask	= (UINT8)creationParamter.colorWriteMask;

		m_platformData.isCreated = true;

		return true;
	}

	void BlendState::dispose()
	{
		m_platformData.isCreated = false;
		GraphicsStateObject::dispose();
	}
}