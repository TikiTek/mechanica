
#include "tiki/graphics/blendstate.hpp"

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	bool BlendState::isCreated() const
	{
		return false;
	}

	bool BlendState::create( GraphicsSystem& graphicsSystem, const BlendStateParamters& creationParamter )
	{
		return false;
	}

	void BlendState::dispose()
	{

		GraphicsStateObject::dispose();
	}
}