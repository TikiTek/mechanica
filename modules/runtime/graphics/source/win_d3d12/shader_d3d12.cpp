
#include "tiki/graphics/shader.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/fixedsizedarray.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/shadertype.hpp"

#include "graphicssystem_internal_d3d12.hpp"

namespace tiki
{
	Shader::Shader()
	{
		m_type = ShaderType_Invalid;

		m_platformData.shaderCode.pShaderBytecode	= nullptr;
		m_platformData.shaderCode.BytecodeLength	= 0u;
	}

	Shader::~Shader()
	{
		TIKI_ASSERT( m_type == ShaderType_Invalid );
		TIKI_ASSERT( m_platformData.shaderCode.pShaderBytecode == nullptr );
	}

	bool Shader::create( GraphicsSystem& graphicsSystem, ShaderType type, const void* pInitData, uint dataSize )
	{
		TIKI_ASSERT( pInitData != nullptr );
		TIKI_ASSERT( dataSize > 0u );

		m_platformData.shaderCode.pShaderBytecode	= static_cast<const BYTE*>(pInitData);
		m_platformData.shaderCode.BytecodeLength	= dataSize;

		m_type	= type;
		m_hash	= crcBytes( m_platformData.pShaderCode, m_platformData.shaderCodeLength );

		return true;
	}

	void Shader::dispose( GraphicsSystem& graphicsSystem )
	{
		m_type = ShaderType_Invalid;

		m_platformData.shaderCode.pShaderBytecode	= nullptr;
		m_platformData.shaderCode.BytecodeLength	= 0u;
	}

	ID3D12RootSignature* graphics::createRootSignature( GraphicsSystem& graphicsSystem, const ShaderPlatformData& shaderData, const D3D12_INPUT_ELEMENT_DESC* pElements, uint elementCount )
	{

	}
}