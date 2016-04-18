
#include "tiki/graphics/samplerstate.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/graphics/graphicssystem.hpp"

#include "graphicssystem_internal_vulkan.hpp"

namespace tiki
{
	bool SamplerState::isCreated() const
	{
		return false;
	}

	bool SamplerState::create( GraphicsSystem& graphicsSystem, const SamplerStateParamters& creationParamter )
	{
		return false;
	}

	void SamplerState::dispose( GraphicsSystem& graphicsSystem )
	{
		m_platformData = SamplerStatePlatformData();
		GraphicsStateObject::dispose();
	}
}