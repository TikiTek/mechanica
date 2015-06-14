
#include "tiki/graphics/blendstate.hpp"

#include "tiki/base/types.hpp"
#include "tiki/base/functions.hpp"

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	static const GLenum s_aBlendModeMapping[] =
	{
		GL_ZERO,					// Blend_Zero
		GL_ONE,						// Blend_One
		GL_SRC_COLOR,				// Blend_SourceColor
		GL_ONE_MINUS_SRC_COLOR,		// Blend_InverseSourceColor
		GL_SRC_ALPHA,				// Blend_SourceAlpha
		GL_ONE_MINUS_SRC_ALPHA,		// Blend_InverseSourceAlpha
		GL_DST_COLOR,				// Blend_DestinationColor
		GL_ONE_MINUS_DST_COLOR,		// Blend_InverseDestinationColor
		GL_DST_ALPHA,				// Blend_DestinationAlpha
		GL_ONE_MINUS_DST_ALPHA		// Blend_InverseDestinationAlpha
	};

	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aBlendModeMapping ) == Blend_Count );

	static const GLenum s_aBlendOperationMapping[] =
	{
		GL_FUNC_ADD,		// BlendOperation_Add
		GL_FUNC_SUBTRACT,	// BlendOperation_Subtract
		GL_MIN,				// BlendOperation_Min
		GL_MAX				// BlendOperation_Max
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aBlendOperationMapping ) == BlendOperation_Count );

	bool BlendState::isCreated() const
	{
		return m_platformData.blendOperation != GL_INVALID_ENUM;
	}

	bool BlendState::create( GraphicsSystem& graphicsSystem, const BlendStateParamters& creationParamter )
	{
		m_platformData.blendEnabled			= ( creationParamter.blendEnabled ? GL_TRUE : GL_FALSE );
		m_platformData.sourceBlend			= s_aBlendModeMapping[ creationParamter.sourceBlend ];
		m_platformData.destinationBlend		= s_aBlendModeMapping[ creationParamter.destinationBlend ];
		m_platformData.blendOperation		= s_aBlendOperationMapping[ creationParamter.operation ];
		m_platformData.colorWriteMask[ 0u ]	= isBitSet( creationParamter.colorWriteMask, ColorWriteMask_Red );
		m_platformData.colorWriteMask[ 1u ]	= isBitSet( creationParamter.colorWriteMask, ColorWriteMask_Green );
		m_platformData.colorWriteMask[ 2u ]	= isBitSet( creationParamter.colorWriteMask, ColorWriteMask_Blue );
		m_platformData.colorWriteMask[ 3u ]	= isBitSet( creationParamter.colorWriteMask, ColorWriteMask_Alpha );

		return true;
	}

	void BlendState::dispose()
	{
		m_platformData = BlendStatePlatformData();

		GraphicsStateObject::dispose();
	}
}
