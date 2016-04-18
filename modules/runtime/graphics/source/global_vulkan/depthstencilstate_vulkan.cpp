
#include "tiki/graphics/depthstencilstate.hpp"

#include "graphicssystem_internal_vulkan.hpp"

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

	void DepthStencilState::dispose( GraphicsSystem& graphicsSystem )
	{
		m_platformData = DepthStencilStatePlatformData();
		GraphicsStateObject::dispose();
	}
}