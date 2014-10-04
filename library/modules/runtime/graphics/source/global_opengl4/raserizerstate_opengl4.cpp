
#include "tiki/graphics/rasterizerstate.hpp"

#include "tiki/base/types.hpp"

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	static const GLenum s_aFillModeMapping[] =
	{
		GL_LINE,	// FillMode_Wireframe
		GL_FILL		// FillMode_Solid
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aFillModeMapping ) == FillMode_Count );

	static const GLenum s_aCullModeMapping[] =
	{
		GL_BACK,	// CullMode_None
		GL_FRONT,	// CullMode_Front
		GL_BACK		// CullMode_Back
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aCullModeMapping ) == CullMode_Count );

	static const BOOL s_aWindingOrderMapping[] =
	{
		GL_CW,	// WindingOrder_Clockwise
		GL_CCW	// WindingOrder_CounterClockwise
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aWindingOrderMapping ) == WindingOrder_Count );

	bool RasterizerState::isCreated() const
	{
		return m_platformData.cullMode != GL_INVALID_ENUM;
	}

	bool RasterizerState::create( GraphicsSystem& graphicsSystem, const RasterizerStateParamters& creationParamter )
	{
		m_platformData.cullEnabled	= ( creationParamter.cullMode != CullMode_None ? GL_TRUE : GL_FALSE );
		m_platformData.cullMode		= s_aCullModeMapping[ creationParamter.cullMode ];
		m_platformData.fillMode		= s_aFillModeMapping[ creationParamter.fillMode ];
		m_platformData.windingOrder	= s_aWindingOrderMapping[ creationParamter.windingOrder ];

		return true;
	}

	void RasterizerState::dispose( GraphicsSystem& graphicsSystem )
	{
		m_platformData = RasterizerStatePlatformData();

		GraphicsStateObject::dispose();
	}
}