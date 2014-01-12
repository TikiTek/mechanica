#pragma once
#ifndef TIKI_SHADER_D3D11_HPP
#define TIKI_SHADER_D3D11_HPP

#include "..\..\..\..\source\win_d3d11\graphicstypes_d3d11.hpp"

namespace tiki
{
	class GraphicsSystem;
	class VertexFormat;
	struct ShaderPlatformData;

	struct ShaderPlatformData
	{
		union
		{
			TGDeviceChild*		pShaderObject;

			TGComputeShader*	pComputeShader;
			TGDomainShader*		pDomainShader;
			TGGeometryShader*	pGeometryShader;
			TGHullShader*		pHullShader;
			TGPixelShader*		pPixelShader;
			TGVertexShader*		pVertexShader;
		};

		const void*				pShaderCode;
		uint					shaderCodeLength;
	};

	namespace graphics
	{
		TGInputLayout*	createVertexInputLayout( GraphicsSystem& graphicsSystem, const ShaderPlatformData& shaderData, const TGInputElementDesc* pElements, uint elementCount );
	}
}

#endif // TIKI_SHADER_D3D11_HPP