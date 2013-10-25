#pragma once
#ifndef TIKI_SHADER_D3D11_HPP
#define TIKI_SHADER_D3D11_HPP

#include "..\..\..\..\source\win_d3d11\graphicstypes_d3d11.hpp"

namespace tiki
{
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

		TGBlob*					pDataBlob;
	};

	namespace graphics
	{
		TGInputLayout*	createVertexInputLayout( const ShaderPlatformData& shaderData );
	}
}

#endif // TIKI_SHADER_D3D11_HPP