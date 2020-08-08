#include "tiki/graphics/shader.hpp"

#include "tiki/base/assert.hpp"

#include "graphicssystem_internal_vulkan.hpp"

namespace tiki
{
	struct ShaderInitializationData
	{
	};

	Shader::Shader()
	{
		m_type = ShaderType_Invalid;
	}

	Shader::~Shader()
	{
		TIKI_ASSERT( m_type == ShaderType_Invalid );
	}

	bool Shader::create( GraphicsSystem& graphicsSystem, ShaderType type, const void* pInitData, uint dataSize )
	{
		return false;
	}

	void Shader::dispose( GraphicsSystem& graphicsSystem )
	{
		m_type = ShaderType_Invalid;
	}
}