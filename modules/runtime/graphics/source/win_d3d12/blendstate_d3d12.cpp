
#include "tiki/graphics/blendstate.hpp"

#include "graphicssystem_internal_d3d12.hpp"

namespace tiki
{
	static const D3D11_BLEND s_aBlendMapping[ Blend_Count ] =
	{
		D3D11_BLEND_ZERO,			// Blend_Zero
		D3D11_BLEND_ONE,			// Blend_One
		D3D11_BLEND_SRC_COLOR,		// Blend_SourceColor
		D3D11_BLEND_INV_SRC_COLOR,	// Blend_InverseSourceColor
		D3D11_BLEND_SRC_ALPHA,		// Blend_SourceAlpha
		D3D11_BLEND_INV_SRC_ALPHA,	// Blend_InverseSourceAlpha
		D3D11_BLEND_DEST_COLOR,		// Blend_DestinationColor
		D3D11_BLEND_INV_DEST_COLOR,	// Blend_InverseDestinationColor
		D3D11_BLEND_DEST_ALPHA,		// Blend_DestinationAlpha
		D3D11_BLEND_INV_DEST_ALPHA,	// Blend_InverseDestinationAlpha
	};

	static const D3D11_BLEND_OP s_aBlendOperationMapping[ BlendOperation_Count ] =
	{
		D3D11_BLEND_OP_ADD,			// BlendOperation_Add
		D3D11_BLEND_OP_SUBTRACT,	// BlendOperation_Subtract
		D3D11_BLEND_OP_MIN,			// BlendOperation_Min
		D3D11_BLEND_OP_MAX			// BlendOperation_Max
	};

	TIKI_COMPILETIME_ASSERT( ColorWriteMask_Red		== D3D11_COLOR_WRITE_ENABLE_RED );
	TIKI_COMPILETIME_ASSERT( ColorWriteMask_Green	== D3D11_COLOR_WRITE_ENABLE_GREEN );
	TIKI_COMPILETIME_ASSERT( ColorWriteMask_Blue	== D3D11_COLOR_WRITE_ENABLE_BLUE );
	TIKI_COMPILETIME_ASSERT( ColorWriteMask_Alpha	== D3D11_COLOR_WRITE_ENABLE_ALPHA );

	bool BlendState::isCreated() const
	{
		return m_platformData.pBlendState != nullptr;
	}

	bool BlendState::create( GraphicsSystem& graphicsSystem, const BlendStateParamters& creationParamter )
	{
		TIKI_DECLARE_STACKANDZERO( D3D11_BLEND_DESC, stateDesc );
		stateDesc.AlphaToCoverageEnable		= FALSE;
		stateDesc.IndependentBlendEnable	= FALSE;

		stateDesc.RenderTarget[ 0u ].BlendEnable			= creationParamter.blendEnabled;
		stateDesc.RenderTarget[ 0u ].SrcBlend				= s_aBlendMapping[ creationParamter.sourceBlend ];
		stateDesc.RenderTarget[ 0u ].DestBlend				= s_aBlendMapping[ creationParamter.destinationBlend ];
		stateDesc.RenderTarget[ 0u ].BlendOp				= s_aBlendOperationMapping[ creationParamter.operation ];

		stateDesc.RenderTarget[ 0u ].SrcBlendAlpha			= D3D11_BLEND_ONE;
		stateDesc.RenderTarget[ 0u ].DestBlendAlpha			= D3D11_BLEND_ZERO;
		stateDesc.RenderTarget[ 0u ].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
		
		stateDesc.RenderTarget[ 0u ].RenderTargetWriteMask	= (UINT8)creationParamter.colorWriteMask;

		HRESULT result = graphics::getDevice( graphicsSystem )->CreateBlendState( &stateDesc, &m_platformData.pBlendState );
		if( FAILED( result ) )
		{
			dispose();
			return false;
		}

		return m_platformData.pBlendState != nullptr;
	}

	void BlendState::dispose()
	{
		if ( m_platformData.pBlendState != nullptr )
		{
			m_platformData.pBlendState->Release();
			m_platformData.pBlendState = nullptr;
		}

		GraphicsStateObject::dispose();
	}
}