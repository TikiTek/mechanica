
#include "tiki/graphics/depthstencilstate.hpp"

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	static const GLenum s_aDepthFunctionMapping[] =
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
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aDepthFunctionMapping ) == ComparsionFunction_Count );

	bool DepthStencilState::isCreated() const
	{
		return false;
	}

	bool DepthStencilState::create( GraphicsSystem& graphicsSystem, const DepthStencilStateParamters& creationParamter )
	{
		return false;
	}

	void DepthStencilState::dispose( GraphicsSystem& graphicsSystem )
	{
		GraphicsStateObject::dispose();
	}
}