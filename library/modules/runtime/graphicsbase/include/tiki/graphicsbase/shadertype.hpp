#pragma once
#ifndef TIKI_SHADERTYPE_HPP__INCLUDED
#define TIKI_SHADERTYPE_HPP__INCLUDED

namespace tiki
{
	enum ShaderType
	{
		ShaderType_Effect,
		ShaderType_VertexShader,
		ShaderType_PixelShader,
		ShaderType_GeometrieShader,
		ShaderType_HullShader,
		ShaderType_DomainShader,
		ShaderType_ComputeShader,
		ShaderType_Count
	};
}

#endif // TIKI_SHADERTYPE_HPP__INCLUDED
