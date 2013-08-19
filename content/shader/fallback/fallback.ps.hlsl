
////////////////////////////////////////////////////////////////////////////////
// Type Defines
////////////////////////////////////////////////////////////////////////////////

struct PS_INPUT
{
	float4 Pos	  : SV_POSITION;
	float4 Color  : COLOR;
};

cbuffer WorldBuffer : register(b1)
{
	matrix WorldM;
	float4 vertexColor;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////

float4 PS_Main(PS_INPUT input) : SV_TARGET
{
	return saturate( input.Color * vertexColor );
}
