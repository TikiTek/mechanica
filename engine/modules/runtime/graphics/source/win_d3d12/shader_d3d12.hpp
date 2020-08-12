#ifndef TIKI_SHADER_D3D12_HPP__INCLUDED__
#define TIKI_SHADER_D3D12_HPP__INCLUDED__

#include "tiki/base/types.hpp"

#include <d3d12.h>

namespace tiki
{
	class GraphicsSystem;
	class VertexFormat;
	struct ShaderPlatformData;

	struct ShaderPlatformData
	{
		D3D12_SHADER_BYTECODE	shaderCode;
	};

	namespace graphics
	{
		ID3D12RootSignature*	createRootSignature( GraphicsSystem& graphicsSystem, const ShaderPlatformData& shaderData, const D3D12_INPUT_ELEMENT_DESC* pElements, uint elementCount );
	}
}

#endif // TIKI_SHADER_D3D12_HPP__INCLUDED__
