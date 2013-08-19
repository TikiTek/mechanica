
////////////////////////////////////////////////////////////////////////////////
// Type Defines
////////////////////////////////////////////////////////////////////////////////

struct VS_INPUT
{
	float4 Pos	  : POSITION;
	float4 Color  : COLOR;
};

struct PS_INPUT
{
	float4 Pos	  : SV_POSITION;
	float4 Color  : COLOR;
};

cbuffer ViewProjBuffer : register(b0)
{
	matrix ViewM;
	matrix ProjM;
};

cbuffer WorldBuffer : register(b1)
{
	matrix WorldM;
	float4 vertexColor;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////

PS_INPUT VS_Main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;

    output.Pos = input.Pos;
    output.Pos = mul(output.Pos, WorldM);
    output.Pos = mul(output.Pos, ViewM);
    output.Pos = mul(output.Pos, ProjM);
    
    output.Color = input.Color;
    
    return output;
}
