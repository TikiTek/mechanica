
#include "tiki/graphics/depthstencilstate.hpp"

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	bool DepthStencilState::isCreated() const
	{
		return false;
	}

	bool DepthStencilState::create( GraphicsSystem& graphicsSystem, const DepthStencilStateParamters& creationParamter )
	{
		return false;
	}

	void DepthStencilState::dispose()
	{
		GraphicsStateObject::dispose();
	}
}