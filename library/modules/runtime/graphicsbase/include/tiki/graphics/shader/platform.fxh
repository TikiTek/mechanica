#ifndef __TIKI_PLATFORM_FXH_INCLUDED__
#define __TIKI_PLATFORM_FXH_INCLUDED__

#define M_PI		3.14159265f
#define M_TWOPI		6.28318530f
#define M_PIOVER2	1.57079632f
#define M_PIOVER4	0.78539816f

#define TIKI_CONCAT_HELPER( x1, x2 ) x1 ## x2
#define TIKI_CONCAT( x1, x2 ) TIKI_CONCAT_HELPER( x1, x2 )

#define TIKI_VAR_IN( name )		TIKI_CONCAT( in, name )
#define TIKI_VAR_OUT( name )	TIKI_CONCAT( out, name )

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
#define TIKI_CAST( targetType, a )		(targetType)( a )

// entry point
#define TIKI_ENTRY_POINT( inputType, outputType, name ) outputType name( inputType input )

// vertex input
#if TIKI_ENABLED( TIKI_VERTEX_SHADER )

#	define TIKI_VERTEX_INPUT_DEFINITION_BEGIN( typeName ) struct typeName {
#	define TIKI_VERTEX_INPUT_DEFINITION_END( typeName ) };
#	define TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( slot, type, semantic )	type TIKI_VAR_IN( semantic ) : semantic;

#	define TIKI_VERTEX_INPUT_GET( semantic ) input. ## TIKI_VAR_IN( semantic )

#endif

// vertex to pixel
#if TIKI_ENABLED( TIKI_VERTEX_SHADER )

#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_BEGIN( typeName ) struct typeName {
#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_END( typeName ) };
#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( type, semantic ) type TIKI_VAR_OUT( semantic ) : semantic;

#	define TIKI_VERTEX_TO_PIXEL_BEGIN( typeName ) typeName output = (typeName)0
#	define TIKI_VERTEX_TO_PIXEL_END( typeName ) return output
#	define TIKI_VERTEX_TO_PIXEL_SET_POSITION( semantic, value ) TIKI_CONCAT( output., TIKI_VAR_OUT( semantic ) ) = value
#	define TIKI_VERTEX_TO_PIXEL_SET( semantic, value ) TIKI_CONCAT( output., TIKI_VAR_OUT( semantic ) ) = value

#elif TIKI_ENABLED( TIKI_PIXEL_SHADER )

#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_BEGIN( typeName ) struct typeName {
#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_END( typeName ) };
#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( type, semantic ) type TIKI_VAR_IN( semantic ) : semantic;

#	define TIKI_VERTEX_TO_PIXEL_GET( semantic ) TIKI_CONCAT( input., TIKI_VAR_IN( semantic ) )

#endif

// pixel output
#if TIKI_ENABLED( TIKI_PIXEL_SHADER )

#	define TIKI_PIXEL_OUTPUT_DEFINITION_BEGIN( typeName ) struct typeName {
#	define TIKI_PIXEL_OUTPUT_DEFINITION_END( typeName ) };
#	define TIKI_PIXEL_OUTPUT_DEFINITION_ELEMENT( slot, type, semantic ) type TIKI_VAR_OUT( semantic ) : semantic;

#	define TIKI_PIXEL_OUTPUT_BEGIN( typeName ) typeName output = (typeName)0
#	define TIKI_PIXEL_OUTPUT_END( typeName ) return output
#	define TIKI_PIXEL_OUTPUT_SET( semantic, value ) TIKI_CONCAT( output., TIKI_VAR_OUT( semantic ) ) = value

#endif

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

#define int1		int
#define int2		ivec2
#define int3		ivec3
#define int4		ivec4

#define uint1		uint
#define uint2		uvec2
#define uint3		uvec3
#define uint4		uvec4

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
#define TIKI_CAST( targetType, a )		targetType( a )

// entry point
#define TIKI_ENTRY_POINT( inputType, outputType, name ) void name()

// vertex input
#if TIKI_ENABLED( TIKI_VERTEX_SHADER )

#	define TIKI_VERTEX_INPUT_DEFINITION_BEGIN( typeName )
#	define TIKI_VERTEX_INPUT_DEFINITION_END( typeName )
#	define TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( slot, type, semantic )	layout(location = slot) in type TIKI_VAR_IN(  semantic );

#	define TIKI_VERTEX_INPUT_GET( semantic ) TIKI_VAR_IN( semantic )

#endif

// vertex to pixel
#if TIKI_ENABLED( TIKI_VERTEX_SHADER )

#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_BEGIN( typeName )
#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_END( typeName )
#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( type, semantic ) out type TIKI_VAR_OUT( semantic );

#	define TIKI_VERTEX_TO_PIXEL_BEGIN( typeName )
#	define TIKI_VERTEX_TO_PIXEL_END( typeName )
#	define TIKI_VERTEX_TO_PIXEL_SET_POSITION( semantic, value ) gl_Position = value
#	define TIKI_VERTEX_TO_PIXEL_SET( semantic, value ) TIKI_VAR_OUT( semantic ) = value

#elif TIKI_ENABLED( TIKI_PIXEL_SHADER )

#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_BEGIN( typeName )
#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_END( typeName )
#	define TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( type, semantic ) in type TIKI_VAR_IN( semantic );

#	define TIKI_VERTEX_TO_PIXEL_GET( semantic ) TIKI_VAR_IN( semantic )

#endif

// pixel output
#if TIKI_ENABLED( TIKI_PIXEL_SHADER )

#	define TIKI_PIXEL_OUTPUT_DEFINITION_BEGIN( typeName )
#	define TIKI_PIXEL_OUTPUT_DEFINITION_END( typeName )
#	define TIKI_PIXEL_OUTPUT_DEFINITION_ELEMENT( slot, type, semantic ) layout(location = slot) out type TIKI_VAR_OUT( semantic );

#	define TIKI_PIXEL_OUTPUT_BEGIN( typeName )
#	define TIKI_PIXEL_OUTPUT_END( typeName )
#	define TIKI_PIXEL_OUTPUT_SET( semantic, value ) TIKI_VAR_OUT( semantic ) = value

#endif

// semantics input
#define TIKI_INPUT_POSITION0
#define TIKI_INPUT_POSITION1
#define TIKI_INPUT_POSITION2
#define TIKI_INPUT_POSITION3

#define TIKI_OUTPUT_POSITION0
#define TIKI_OUTPUT_POSITION1
#define TIKI_OUTPUT_POSITION2
#define TIKI_OUTPUT_POSITION3

#define TIKI_TEXCOORD0
#define TIKI_TEXCOORD1
#define TIKI_TEXCOORD2
#define TIKI_TEXCOORD3
#define TIKI_TEXCOORD4
#define TIKI_TEXCOORD5
#define TIKI_TEXCOORD6
#define TIKI_TEXCOORD7

#define TIKI_COLOR0
#define TIKI_COLOR1
#define TIKI_COLOR2
#define TIKI_COLOR3

#define TIKI_NORMAL0
#define TIKI_NORMAL1
#define TIKI_NORMAL2
#define TIKI_NORMAL3

#define TIKI_BINORMAL0
#define TIKI_BINORMAL1
#define TIKI_BINORMAL2
#define TIKI_BINORMAL3

#define TIKI_TANGENT0
#define TIKI_TANGENT1
#define TIKI_TANGENT2
#define TIKI_TANGENT3

#define TIKI_JOINTINDICES0
#define TIKI_JOINTINDICES1
#define TIKI_JOINTINDICES2
#define TIKI_JOINTINDICES3

#define TIKI_JOINTWEIGHTS0
#define TIKI_JOINTWEIGHTS1
#define TIKI_JOINTWEIGHTS2
#define TIKI_JOINTWEIGHTS3

// semantics output
#define TIKI_OUTPUT_COLOR0
#define TIKI_OUTPUT_COLOR1
#define TIKI_OUTPUT_COLOR2
#define TIKI_OUTPUT_COLOR3
#define TIKI_OUTPUT_COLOR4
#define TIKI_OUTPUT_COLOR5
#define TIKI_OUTPUT_COLOR6
#define TIKI_OUTPUT_COLOR7

#define TIKI_OUTPUT_DEPTH

#endif // TIKI_ENABLED( TIKI_OPENGL4 )

#endif // __TIKI_PLATFORM_FXH_INCLUDED__