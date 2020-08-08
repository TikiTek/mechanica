#include "tiki/graphics/depth_stencil_state.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/graphics/graphics_system.hpp"

#include <d3d11.h>

namespace tiki
{
	static const D3D11_COMPARISON_FUNC s_aComparsionFunctionMapping[ ComparsionFunction_Count ] =
	{
		D3D11_COMPARISON_NEVER,				// ComparsionFunction_Never
		D3D11_COMPARISON_LESS,				// ComparsionFunction_Less
		D3D11_COMPARISON_EQUAL,				// ComparsionFunction_Equal
		D3D11_COMPARISON_LESS_EQUAL,		// ComparsionFunction_LessOrEqual
		D3D11_COMPARISON_GREATER,			// ComparsionFunction_Greater
		D3D11_COMPARISON_NOT_EQUAL,			// ComparsionFunction_NotEqual
		D3D11_COMPARISON_GREATER_EQUAL,		// ComparsionFunction_GreaterOrEqual
		D3D11_COMPARISON_ALWAYS				// ComparsionFunction_Always
	};

	static const D3D11_STENCIL_OP s_aStencilOperationMapping[ StencilOperation_Count ] =
	{
		D3D11_STENCIL_OP_KEEP,		// StencilOperation_Keep
		D3D11_STENCIL_OP_ZERO,		// StencilOperation_Zero
		D3D11_STENCIL_OP_REPLACE,	// StencilOperation_Replace
		D3D11_STENCIL_OP_INCR,		// StencilOperation_Increase
		D3D11_STENCIL_OP_DECR		// StencilOperation_Decrease
	};

	bool DepthStencilState::isCreated() const
	{
		return m_platformData.pDepthStencilState != nullptr;
	}

	bool DepthStencilState::create( GraphicsSystem& graphicsSystem, const DepthStencilStateParamters& creationParamter )
	{
		TIKI_DECLARE_STACKANDZERO( D3D11_DEPTH_STENCIL_DESC, stateDesc );
		stateDesc.DepthEnable		= creationParamter.depthEnabled;
		stateDesc.DepthWriteMask	= creationParamter.depthWriteEnabled ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		stateDesc.DepthFunc			= s_aComparsionFunctionMapping[ creationParamter.depthFunction ];

		stateDesc.StencilEnable		= creationParamter.stencilEnabled;
		stateDesc.StencilReadMask	= creationParamter.stencilReadMask;
		stateDesc.StencilWriteMask	= creationParamter.stencilWriteMask;

		stateDesc.FrontFace.StencilFunc			= s_aComparsionFunctionMapping[ creationParamter.frontFace.stencilFunction ];
		stateDesc.FrontFace.StencilDepthFailOp	= s_aStencilOperationMapping[ creationParamter.frontFace.depthFailOperation ];
		stateDesc.FrontFace.StencilFailOp		= s_aStencilOperationMapping[ creationParamter.frontFace.stencilFailOperation ];
		stateDesc.FrontFace.StencilPassOp		= s_aStencilOperationMapping[ creationParamter.frontFace.stencilPassOperation ];

		stateDesc.BackFace.StencilFunc			= s_aComparsionFunctionMapping[ creationParamter.backFace.stencilFunction ];
		stateDesc.BackFace.StencilDepthFailOp	= s_aStencilOperationMapping[ creationParamter.backFace.depthFailOperation ];
		stateDesc.BackFace.StencilFailOp		= s_aStencilOperationMapping[ creationParamter.backFace.stencilFailOperation ];
		stateDesc.BackFace.StencilPassOp		= s_aStencilOperationMapping[ creationParamter.backFace.stencilPassOperation ];

		m_platformData.stencilRef = creationParamter.stencilReference;

		HRESULT result = GraphicsSystemPlatform::getDevice( graphicsSystem )->CreateDepthStencilState( &stateDesc, &m_platformData.pDepthStencilState );
		if( FAILED( result ) )
		{
			dispose( graphicsSystem );
			return false;
		}

		return m_platformData.pDepthStencilState != nullptr;
	}

	void DepthStencilState::dispose( GraphicsSystem& /*graphicsSystem*/ )
	{
		m_platformData.stencilRef = 0u;

		if ( m_platformData.pDepthStencilState != nullptr )
		{
			m_platformData.pDepthStencilState->Release();
			m_platformData.pDepthStencilState = nullptr;
		}

		GraphicsStateObject::dispose();
	}
}