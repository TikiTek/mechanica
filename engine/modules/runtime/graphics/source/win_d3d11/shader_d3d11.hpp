#pragma once

#include "graphics_types_d3d11.hpp"

namespace tiki
{
	class GraphicsSystem;
	class VertexFormat;
	struct ShaderPlatformData;

	struct ShaderPlatformData
	{
		union
		{
			ID3D11DeviceChild*		pShaderObject;

			ID3D11ComputeShader*	pComputeShader;
			ID3D11DomainShader*		pDomainShader;
			ID3D11GeometryShader*	pGeometryShader;
			ID3D11HullShader*		pHullShader;
			ID3D11PixelShader*		pPixelShader;
			ID3D11VertexShader*		pVertexShader;
		};

		const void*				pShaderCode;
		uint					shaderCodeLength;
	};

	namespace graphics
	{
		ID3D11InputLayout* createVertexInputLayout( GraphicsSystem& graphicsSystem, const ShaderPlatformData& shaderData, const D3D11_INPUT_ELEMENT_DESC* pElements, uint elementCount );
	}
}
