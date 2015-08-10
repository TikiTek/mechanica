
#include "tiki/graphics/depthstencilstate.hpp"

#include "tiki/base/memory.hpp"

#include "graphicssystem_internal_d3d12.hpp"

namespace tiki
{
	static const D3D12_COMPARISON_FUNC s_aComparsionFunctionMapping[] =
	{
		D3D12_COMPARISON_NEVER,				// ComparsionFunction_Never
		D3D12_COMPARISON_LESS,				// ComparsionFunction_Less
		D3D12_COMPARISON_EQUAL,				// ComparsionFunction_Equal
		D3D12_COMPARISON_LESS_EQUAL,		// ComparsionFunction_LessOrEqual
		D3D12_COMPARISON_GREATER,			// ComparsionFunction_Greater
		D3D12_COMPARISON_NOT_EQUAL,			// ComparsionFunction_NotEqual
		D3D12_COMPARISON_GREATER_EQUAL,		// ComparsionFunction_GreaterOrEqual
		D3D12_COMPARISON_ALWAYS				// ComparsionFunction_Always
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aComparsionFunctionMapping ) == ComparsionFunction_Count );

	static const D3D12_STENCIL_OP s_aStencilOperationMapping[] =
	{
		D3D12_STENCIL_OP_KEEP,		// StencilOperation_Keep
		D3D12_STENCIL_OP_ZERO,		// StencilOperation_Zero
		D3D12_STENCIL_OP_REPLACE,	// StencilOperation_Replace
		D3D12_STENCIL_OP_INCR,		// StencilOperation_Increase
		D3D12_STENCIL_OP_DECR		// StencilOperation_Decrease
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aStencilOperationMapping ) == StencilOperation_Count );

	bool DepthStencilState::isCreated() const
	{
		return m_platformData.isCreated;
	}

	bool DepthStencilState::create( GraphicsSystem& graphicsSystem, const DepthStencilStateParamters& creationParamter )
	{
		memory::zero( m_platformData.depthStencilDesc );
		m_platformData.depthStencilDesc.DepthEnable			= creationParamter.depthEnabled;
		m_platformData.depthStencilDesc.DepthWriteMask		= creationParamter.depthWriteEnabled ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
		m_platformData.depthStencilDesc.DepthFunc			= s_aComparsionFunctionMapping[ creationParamter.depthFunction ];

		m_platformData.depthStencilDesc.StencilEnable		= creationParamter.stencilEnabled;
		m_platformData.depthStencilDesc.StencilReadMask		= creationParamter.stencilReadMask;
		m_platformData.depthStencilDesc.StencilWriteMask	= creationParamter.stencilWriteMask;
		
		m_platformData.depthStencilDesc.FrontFace.StencilFunc			= s_aComparsionFunctionMapping[ creationParamter.frontFace.stencilFunction ];
		m_platformData.depthStencilDesc.FrontFace.StencilDepthFailOp	= s_aStencilOperationMapping[ creationParamter.frontFace.depthFailOperation ];
		m_platformData.depthStencilDesc.FrontFace.StencilFailOp			= s_aStencilOperationMapping[ creationParamter.frontFace.stencilFailOperation ];
		m_platformData.depthStencilDesc.FrontFace.StencilPassOp			= s_aStencilOperationMapping[ creationParamter.frontFace.stencilPassOperation ];

		m_platformData.depthStencilDesc.BackFace.StencilFunc			= s_aComparsionFunctionMapping[ creationParamter.backFace.stencilFunction ];
		m_platformData.depthStencilDesc.BackFace.StencilDepthFailOp		= s_aStencilOperationMapping[ creationParamter.backFace.depthFailOperation ];
		m_platformData.depthStencilDesc.BackFace.StencilFailOp			= s_aStencilOperationMapping[ creationParamter.backFace.stencilFailOperation ];
		m_platformData.depthStencilDesc.BackFace.StencilPassOp			= s_aStencilOperationMapping[ creationParamter.backFace.stencilPassOperation ];

		m_platformData.stencilRef = creationParamter.stencilReference;
		m_platformData.isCreated = true;

		return true;
	}

	void DepthStencilState::dispose( GraphicsSystem& /*graphicsSystem*/ )
	{
		m_platformData.isCreated = false;
		GraphicsStateObject::dispose();
	}
}