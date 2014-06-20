#ifndef __TIKI_PLATFORM_FXH_INCLUDED__
#define __TIKI_PLATFORM_FXH_INCLUDED__

#define M_PI		3.14159265f
#define M_TWOPI		6.28318530f
#define M_PIOVER2	1.57079632f
#define M_PIOVER4	0.78539816f

#define TIKI_CONCAT_HELPER( x1, x2 ) x1 ## x2
#define TIKI_CONCAT( x1, x2 ) TIKI_CONCAT_HELPER( x1, x2 )


#if TIKI_ENABLED( TIKI_HLSL4 )

// parameters

#define TIKI_IN
#define TIKI_OUT
#define TIKI_INOUT

// types

#define TIKI_SAMPLER		SamplerState
#define TIKI_TEXTURE1D		Texture1D
#define TIKI_TEXTURE2D		Texture2D
#define TIKI_TEXTURE3D		Texture3D
#define TIKI_TEXTURECUBE	TextureCube

// constants
#define TIKI_DEFINE_CONSTANT( slot, type, name ) cbuffer constant_var ## name : register( TIKI_CONCAT( b, slot ) ) { type name; };

#define TIKI_DEFINE_TEXTURE1D( slot, name )		Texture1D name : register( TIKI_CONCAT( t, slot ) );
#define TIKI_DEFINE_TEXTURE2D( slot, name )		Texture2D name : register( TIKI_CONCAT( t, slot ) );
#define TIKI_DEFINE_TEXTURE3D( slot, name )		Texture3D name : register( TIKI_CONCAT( t, slot ) );
#define TIKI_DEFINE_TEXTURECUBE( slot, name )	TextureCube name : register( TIKI_CONCAT( t, slot ) );

#define TIKI_DEFINE_SAMPLER( slot, name )		SamplerState name : register( TIKI_CONCAT( s, slot ) );

// functions

#define TIKI_TEX1D( tex, sam, u )		tex.Sample( sam, u.x )
#define TIKI_TEX2D( tex, sam, uv )		tex.Sample( sam, uv.xy )
#define TIKI_TEX3D( tex, sam, uvw )		tex.Sample( sam, uvw.xyz )
#define TIKI_TEXCUBE( tex, sam, uvw )	tex.Sample( sam, uvw.xyz )

#define TIKI_MUL( a, b )				mul( a, b )
#define TIKI_SATURATE( a )				saturate( a )

// semantics input
#define TIKI_INPUT_POSITION0	POSITION0
#define TIKI_INPUT_POSITION1	POSITION1
#define TIKI_INPUT_POSITION2	POSITION2
#define TIKI_INPUT_POSITION3	POSITION3

#define TIKI_OUTPUT_POSITION0	SV_POSITION0
#define TIKI_OUTPUT_POSITION1	SV_POSITION1
#define TIKI_OUTPUT_POSITION2	SV_POSITION2
#define TIKI_OUTPUT_POSITION3	SV_POSITION3

#define TIKI_TEXCOORD0			TEXCOORD0
#define TIKI_TEXCOORD1			TEXCOORD1
#define TIKI_TEXCOORD2			TEXCOORD2
#define TIKI_TEXCOORD3			TEXCOORD3
#define TIKI_TEXCOORD4			TEXCOORD4
#define TIKI_TEXCOORD5			TEXCOORD5
#define TIKI_TEXCOORD6			TEXCOORD6
#define TIKI_TEXCOORD7			TEXCOORD7

#define TIKI_COLOR0				COLOR0
#define TIKI_COLOR1				COLOR1
#define TIKI_COLOR2				COLOR2
#define TIKI_COLOR3				COLOR3

#define TIKI_NORMAL0			NORMAL0
#define TIKI_NORMAL1			NORMAL1
#define TIKI_NORMAL2			NORMAL2
#define TIKI_NORMAL3			NORMAL3

#define TIKI_BINORMAL0			BINORMAL0
#define TIKI_BINORMAL1			BINORMAL1
#define TIKI_BINORMAL2			BINORMAL2
#define TIKI_BINORMAL3			BINORMAL3

#define TIKI_TANGENT0			TANGENT0
#define TIKI_TANGENT1			TANGENT1
#define TIKI_TANGENT2			TANGENT2
#define TIKI_TANGENT3			TANGENT3

#define TIKI_JOINTINDICES0		BLENDINDICES0
#define TIKI_JOINTINDICES1		BLENDINDICES1
#define TIKI_JOINTINDICES2		BLENDINDICES2
#define TIKI_JOINTINDICES3		BLENDINDICES3

#define TIKI_JOINTWEIGHTS0		BLENDWEIGHT0
#define TIKI_JOINTWEIGHTS1		BLENDWEIGHT1
#define TIKI_JOINTWEIGHTS2		BLENDWEIGHT2
#define TIKI_JOINTWEIGHTS3		BLENDWEIGHT3

// semantics output
#define TIKI_OUTPUT_COLOR0		SV_TARGET0
#define TIKI_OUTPUT_COLOR1		SV_TARGET1
#define TIKI_OUTPUT_COLOR2		SV_TARGET2
#define TIKI_OUTPUT_COLOR3		SV_TARGET3
#define TIKI_OUTPUT_COLOR4		SV_TARGET4
#define TIKI_OUTPUT_COLOR5		SV_TARGET5
#define TIKI_OUTPUT_COLOR6		SV_TARGET6
#define TIKI_OUTPUT_COLOR7		SV_TARGET7

#define TIKI_OUTPUT_DEPTH		SV_DEPTH

#elif TIKI_ENABLED( TIKI_OPENGL4 )

// parameters

#define TIKI_IN				in
#define TIKI_OUT			out
#define TIKI_INOUT			inout

// types

#define TIKI_SAMPLER		float
#define TIKI_TEXTURE1D		sampler1D
#define TIKI_TEXTURE2D		sampler2D
#define TIKI_TEXTURE3D		sampler3D
#define TIKI_TEXTURECUBE	samplerCube

#define float1		float
#define float2		vec2
#define float3		vec3
#define float4		vec4

#define half		float
#define half1		float
#define half2		vec2
#define half3		vec3
#define half4		vec4

#define float3x3	mat3x3
#define float4x4	mat4x4

// constants
#define TIKI_DEFINE_CONSTANT( slot, type, name ) layout(location = slot) uniform type ## name { type name; };

#define TIKI_DEFINE_TEXTURE1D( slot, name )		uniform sampler1D name;
#define TIKI_DEFINE_TEXTURE2D( slot, name )		uniform sampler2D name;
#define TIKI_DEFINE_TEXTURE3D( slot, name )		uniform sampler3D name;
#define TIKI_DEFINE_TEXTURECUBE( slot, name )	uniform samplerCube name;

#define TIKI_DEFINE_SAMPLER( slot, name )		const float name = 42.0;

// functions

#define TIKI_TEX1D( tex, sam, u )		texture( tex, u )
#define TIKI_TEX2D( tex, sam, uv )		texture( tex, uv )
#define TIKI_TEX3D( tex, sam, uvw )		texture( tex, uvw )
#define TIKI_TEXCUBE( tex, sam, uvw )	texture( tex, uvw )

#define TIKI_MUL( a, b )				a * b
#define TIKI_SATURATE( a )				clamp( a, 0.0, 1.0 )

// entry point
#define TIKI_ENTRY_POINT( inputType, outputType, name ) void name()

// vertex input
#if TIKI_ENABLED( TIKI_VERTEX_SHADER )

#	define TIKI_VERTEX_INPUT_DEFINITION_BEGIN( typeName )
#	define TIKI_VERTEX_INPUT_DEFINITION_END( typeName )
#	define TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( slot, type, name, semantic )	layout(location = slot) in type in ## name;

#	define TIKI_VERTEX_INPUT_GET( name ) in ## name

#else

#	define TIKI_VERTEX_INPUT_DEFINITION_BEGIN( typeName )
#	define TIKI_VERTEX_INPUT_DEFINITION_END( typeName )
#	define TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( slot, type, name, semantic )

#	define TIKI_VERTEX_INPUT_GET( name )

#endif

// vertex to pixel
#if TIKI_ENABLED( TIKI_VERTEX_SHADER )

#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_BEGIN( name )
#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_END( name )
#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( type, name, semantic ) out type out ## name;

#	define TIKI_VERTEX_TO_PIXEL_BEGIN( typeName )
#	define TIKI_VERTEX_TO_PIXEL_END( typeName )
#	define TIKI_VERTEX_TO_PIXEL_SET_POSITION( name, value ) gl_Position = value
#	define TIKI_VERTEX_TO_PIXEL_SET( name, value ) out ## name = value

#elif TIKI_ENABLED( TIKI_PIXEL_SHADER )

#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_BEGIN( name )
#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_END( name )
#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( type, name, semantic ) in type in ## name;

#	define TIKI_VERTEX_TO_PIXEL_GET( name ) in ## name

#else

#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_BEGIN( name )
#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_END( name )
#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( type, name, semantic )

#endif

// pixel output
#if TIKI_ENABLED( TIKI_PIXEL_SHADER )

#	define TIKI_PIXEL_OUTPUT_DEFINITION_BEGIN( typeName )
#	define TIKI_PIXEL_OUTPUT_DEFINITION_END( typeName )
#	define TIKI_PIXEL_OUTPUT_DEFINITION_ELEMENT( slot, type, name, semantic ) layout(location = slot) out type out ## name;

#	define TIKI_PIXEL_OUTPUT_BEGIN( typeName )
#	define TIKI_PIXEL_OUTPUT_END( typeName )
#	define TIKI_PIXEL_OUTPUT_SET( name, value ) out ## name = value

#else

#	define TIKI_PIXEL_OUTPUT_DEFINITION_BEGIN( name )
#	define TIKI_PIXEL_OUTPUT_DEFINITION_END( name )
#	define TIKI_PIXEL_OUTPUT_DEFINITION_ELEMENT( slot, type, name, semantic )

#	define TIKI_PIXEL_OUTPUT_BEGIN( typeName )
#	define TIKI_PIXEL_OUTPUT_END( typeName )
#	define TIKI_PIXEL_OUTPUT_SET( name, value )

#endif


// semantics input
#define TIKI_INPUT_POSITION0	POSITION0
#define TIKI_INPUT_POSITION1	POSITION1
#define TIKI_INPUT_POSITION2	POSITION2
#define TIKI_INPUT_POSITION3	POSITION3

#define TIKI_OUTPUT_POSITION0	POSITION0
#define TIKI_OUTPUT_POSITION1	POSITION1
#define TIKI_OUTPUT_POSITION2	POSITION2
#define TIKI_OUTPUT_POSITION3	POSITION3

#define TIKI_TEXCOORD0			TEXCOORD0
#define TIKI_TEXCOORD1			TEXCOORD1
#define TIKI_TEXCOORD2			TEXCOORD2
#define TIKI_TEXCOORD3			TEXCOORD3
#define TIKI_TEXCOORD4			TEXCOORD4
#define TIKI_TEXCOORD5			TEXCOORD5
#define TIKI_TEXCOORD6			TEXCOORD6
#define TIKI_TEXCOORD7			TEXCOORD7

#define TIKI_COLOR0				COLOR0
#define TIKI_COLOR1				COLOR1
#define TIKI_COLOR2				COLOR2
#define TIKI_COLOR3				COLOR3

#define TIKI_NORMAL0			NORMAL0
#define TIKI_NORMAL1			NORMAL1
#define TIKI_NORMAL2			NORMAL2
#define TIKI_NORMAL3			NORMAL3

#define TIKI_BINORMAL0			BINORMAL0
#define TIKI_BINORMAL1			BINORMAL1
#define TIKI_BINORMAL2			BINORMAL2
#define TIKI_BINORMAL3			BINORMAL3

#define TIKI_TANGENT0			TANGENT0
#define TIKI_TANGENT1			TANGENT1
#define TIKI_TANGENT2			TANGENT2
#define TIKI_TANGENT3			TANGENT3

#define TIKI_JOINTINDICES0		BLENDINDICES0
#define TIKI_JOINTINDICES1		BLENDINDICES1
#define TIKI_JOINTINDICES2		BLENDINDICES2
#define TIKI_JOINTINDICES3		BLENDINDICES3

#define TIKI_JOINTWEIGHTS0		BLENDWEIGHT0
#define TIKI_JOINTWEIGHTS1		BLENDWEIGHT1
#define TIKI_JOINTWEIGHTS2		BLENDWEIGHT2
#define TIKI_JOINTWEIGHTS3		BLENDWEIGHT3

// semantics output
#define TIKI_OUTPUT_COLOR0		COLOR0
#define TIKI_OUTPUT_COLOR1		COLOR1
#define TIKI_OUTPUT_COLOR2		COLOR2
#define TIKI_OUTPUT_COLOR3		COLOR3
#define TIKI_OUTPUT_COLOR4		COLOR4
#define TIKI_OUTPUT_COLOR5		COLOR5
#define TIKI_OUTPUT_COLOR6		COLOR6
#define TIKI_OUTPUT_COLOR7		COLOR7

#define TIKI_OUTPUT_DEPTH		DEPTH

#endif

// default semantics
#define TIKI_INPUT_POSITION		TIKI_INPUT_POSITION0
#define TIKI_OUTPUT_POSITION	TIKI_OUTPUT_POSITION0

#define TIKI_TEXCOORD			TIKI_TEXCOORD0
#define TIKI_COLOR				TIKI_COLOR0
#define TIKI_NORMAL				TIKI_NORMAL0
#define TIKI_BINORMAL			TIKI_BINORMAL0
#define TIKI_TANGENT			TIKI_TANGENT0
#define TIKI_JOINTINDICES		TIKI_JOINTINDICES0
#define TIKI_JOINTWEIGHTS		TIKI_JOINTWEIGHTS0

#define TIKI_OUTPUT_COLOR		TIKI_OUTPUT_COLOR0

#endif // __TIKI_PLATFORM_FXH_INCLUDED__