
#include "tiki/graphics/depthstencilstate.hpp"

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	static const GLenum s_aComparsionFunctionMapping[] =
	{
		GL_NEVER,		// ComparsionFunction_Never
		GL_LESS,		// ComparsionFunction_Less
		GL_EQUAL,		// ComparsionFunction_Equal
		GL_LEQUAL,		// ComparsionFunction_LessOrEqual
		GL_GREATER,		// ComparsionFunction_Greater
		GL_NOTEQUAL,	// ComparsionFunction_NotEqual
		GL_GEQUAL,		// ComparsionFunction_GreaterOrEqual
		GL_ALWAYS		// ComparsionFunction_Always
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aComparsionFunctionMapping ) == ComparsionFunction_Count );

	static const GLenum s_aStencilOperationMapping[] =
	{
		GL_KEEP,		// StencilOperation_Keep
		GL_ZERO,		// StencilOperation_Zero
		GL_REPLACE,		// StencilOperation_Replace
		GL_INCR,		// StencilOperation_Increase
		GL_DECR			// StencilOperation_Decrease
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aStencilOperationMapping ) == StencilOperation_Count );

	bool DepthStencilState::isCreated() const
	{
		return m_platformData.depthFunction != GL_INVALID_ENUM;
	}

	bool DepthStencilState::create( GraphicsSystem& graphicsSystem, const DepthStencilStateParamters& creationParamter )
	{
		m_platformData.depthEnabled			= ( creationParamter.depthEnabled ? GL_TRUE : GL_FALSE );
		m_platformData.depthWriteEnabled	= ( creationParamter.depthWriteEnabled ? GL_TRUE : GL_FALSE );
		m_platformData.stencilEnabled		= ( creationParamter.stencilEnabled ? GL_TRUE : GL_FALSE );

		m_platformData.depthFunction		= s_aComparsionFunctionMapping[ creationParamter.depthFunction ];

		m_platformData.stencilReadMask		= creationParamter.stencilReadMask;
		m_platformData.stencilWriteMask		= creationParamter.stencilWriteMask;
		m_platformData.stencilRefernce		= creationParamter.stencilReference;

		m_platformData.frontFace.stencilFunction		= s_aComparsionFunctionMapping[ creationParamter.frontFace.stencilFunction ];
		m_platformData.frontFace.depthFailOperation		= s_aStencilOperationMapping[ creationParamter.frontFace.depthFailOperation ];
		m_platformData.frontFace.stencilFailOperation	= s_aStencilOperationMapping[ creationParamter.frontFace.stencilFailOperation ];
		m_platformData.frontFace.stencilPassOperation	= s_aStencilOperationMapping[ creationParamter.frontFace.stencilPassOperation ];

		m_platformData.backFace.stencilFunction			= s_aComparsionFunctionMapping[ creationParamter.backFace.stencilFunction ];
		m_platformData.backFace.depthFailOperation		= s_aStencilOperationMapping[ creationParamter.backFace.depthFailOperation ];
		m_platformData.backFace.stencilFailOperation	= s_aStencilOperationMapping[ creationParamter.backFace.stencilFailOperation ];
		m_platformData.backFace.stencilPassOperation	= s_aStencilOperationMapping[ creationParamter.backFace.stencilPassOperation ];

		return true;
	}

	void DepthStencilState::dispose( GraphicsSystem& graphicsSystem )
	{
		m_platformData = DepthStencilStatePlatformData();

		GraphicsStateObject::dispose();
	}
}