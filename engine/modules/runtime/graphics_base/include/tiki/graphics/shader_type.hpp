#pragma once

namespace tiki
{
	enum ShaderType
	{
		ShaderType_Invalid = -1,

		ShaderType_VertexShader,
		ShaderType_PixelShader,
		ShaderType_GeometrieShader,
		ShaderType_HullShader,
		ShaderType_DomainShader,
		ShaderType_ComputeShader,

		ShaderType_Count
	};
}
