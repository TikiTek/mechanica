
#include "tiki/graphics/rasterizerstate.hpp"

#include "tiki/base/types.hpp"

#include "graphicssystem_internal_vulkan.hpp"

namespace tiki
{
	bool RasterizerState::isCreated() const
	{
		return false;
	}

	bool RasterizerState::create( GraphicsSystem& graphicsSystem, const RasterizerStateParamters& creationParamter )
	{
		return false;
	}

	void RasterizerState::dispose( GraphicsSystem& graphicsSystem )
	{
		m_platformData = RasterizerStatePlatformData();
		GraphicsStateObject::dispose();
	}
}