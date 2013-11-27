
#include "tiki/graphicsresources/shaderset.hpp"

namespace tiki
{
	const Shader* ShaderSet::getShader( ShaderType type, crc32 variantKey ) const
	{
		const uint32 shaderData[] = { type, variantKey };
		const crc32 shaderCrc = crcBytes( shaderData, sizeof( shaderData ) );

		const Shader* pShader = nullptr;
		m_shaders.findValue( &pShader, shaderCrc );

		return pShader;
	}

	ShaderSet::ShaderSet()
	{
	}

	ShaderSet::~ShaderSet()
	{
	}

	bool ShaderSet::create( GraphicsSystem& graphicsSystem, const void* pInitData, uint initDataSize )
	{
		return false;
	}

	void ShaderSet::dispose()
	{
		m_shaders.dispose();
	}

}