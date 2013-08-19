
////////////////////////////////////////////////////////////////////////////////
// Type Defines
////////////////////////////////////////////////////////////////////////////////

struct VertexInput
{
	float2 position	: POSITION;
	float4 vertexId	: TEXCOORD0;
	float2 size		: TEXCOORD1;
	float4 texcoord	: TEXCOORD2;
};

struct VertexToFragment
{
	float4 position	: SV_POSITION;
	float2 texcoord	: TEXCOORD;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////

VertexToFragment VS_Main( VertexInput input )
{
    VertexToFragment output = (VertexToFragment)0;

	float2 clipPosition = input.position * float2( 2.0f, - 2.0f ) + float2( -1.0f, 1.0f );
	
    output.position = float4( clipPosition, 0.0f, 1.0f );
    
    output.texcoord = float2(
		( input.texcoord.x * input.vertexId.x ) + ( input.texcoord.z * input.vertexId.z ),
		( input.texcoord.y * input.vertexId.y ) + ( input.texcoord.w * input.vertexId.w )
	);
    
    return output;
}
