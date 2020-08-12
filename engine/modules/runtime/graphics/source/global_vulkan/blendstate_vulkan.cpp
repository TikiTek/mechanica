
#include "tiki/graphics/blendstate.hpp"

#include "tiki/base/types.hpp"
#include "tiki/base/functions.hpp"

#include "graphicssystem_internal_vulkan.hpp"

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
		m_platformData = BlendStatePlatformData();
		GraphicsStateObject::dispose();
	}
}
