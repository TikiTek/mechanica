#pragma once
#ifndef TIKI_STRUCTS_HPP
#define TIKI_STRUCTS_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/inline.hpp"
#include "tiki/base/reflection.hpp"

namespace tiki
{
	// byte
	TIKI_REFLECTION_STRUCT(
		byte2,
		TIKI_REFLECTION_FIELD( uint8, x )
		TIKI_REFLECTION_FIELD( uint8, y )
	);

	TIKI_REFLECTION_STRUCT(
		byte3,
		TIKI_REFLECTION_FIELD( uint8, x )
		TIKI_REFLECTION_FIELD( uint8, y )
		TIKI_REFLECTION_FIELD( uint8, z )
	);

	TIKI_REFLECTION_STRUCT(
		byte4,
		TIKI_REFLECTION_FIELD( uint8, x )
		TIKI_REFLECTION_FIELD( uint8, y )
		TIKI_REFLECTION_FIELD( uint8, z )
		TIKI_REFLECTION_FIELD( uint8, w )
	);

	TIKI_FORCE_INLINE void createByte2( byte2& value, uint8 x, uint8 y ) { value.x = x; value.y = y; }
	TIKI_FORCE_INLINE void createByte3( byte3& value, uint8 x, uint8 y, uint8 z ) { value.x = x; value.y = y; value.z = z; }
	TIKI_FORCE_INLINE void createByte4( byte4& value, uint8 x, uint8 y, uint8 z, uint8 w ) { value.x = x; value.y = y; value.z = z; value.w = w; }


	// uint
	TIKI_REFLECTION_STRUCT(
		uint2,
		TIKI_REFLECTION_FIELD( uint32, x )
		TIKI_REFLECTION_FIELD( uint32, y )
	);

	TIKI_REFLECTION_STRUCT(
		uint3,
		TIKI_REFLECTION_FIELD( uint32, x )
		TIKI_REFLECTION_FIELD( uint32, y )
		TIKI_REFLECTION_FIELD( uint32, z )
	);

	TIKI_REFLECTION_STRUCT(
		uint4,
		TIKI_REFLECTION_FIELD( uint32, x )
		TIKI_REFLECTION_FIELD( uint32, y )
		TIKI_REFLECTION_FIELD( uint32, z )
		TIKI_REFLECTION_FIELD( uint32, w )
	);

	TIKI_FORCE_INLINE void createUint2( uint2& value, uint32 x, uint32 y ) { value.x = x; value.y = y; }
	TIKI_FORCE_INLINE void createUint3( uint3& value, uint32 x, uint32 y, uint32 z ) { value.x = x; value.y = y; value.z = z; }
	TIKI_FORCE_INLINE void createUint4( uint4& value, uint32 x, uint32 y, uint32 z, uint32 w ) { value.x = x; value.y = y; value.z = z; value.w = w; }


	// sint
	TIKI_REFLECTION_STRUCT(
		sint2,
		TIKI_REFLECTION_FIELD( sint32, x )
		TIKI_REFLECTION_FIELD( sint32, y )
	);

	TIKI_REFLECTION_STRUCT(
		sint3,
		TIKI_REFLECTION_FIELD( sint32, x )
		TIKI_REFLECTION_FIELD( sint32, y )
		TIKI_REFLECTION_FIELD( sint32, z )
	);

	TIKI_REFLECTION_STRUCT(
		sint4,
		TIKI_REFLECTION_FIELD( sint32, x )
		TIKI_REFLECTION_FIELD( sint32, y )
		TIKI_REFLECTION_FIELD( sint32, z )
		TIKI_REFLECTION_FIELD( sint32, w )
	);

	TIKI_FORCE_INLINE void createSint2( sint2& value, sint32 x, sint32 y ) { value.x = x; value.y = y; }
	TIKI_FORCE_INLINE void createSint3( sint3& value, sint32 x, sint32 y, sint32 z ) { value.x = x; value.y = y; value.z = z; }
	TIKI_FORCE_INLINE void createSint4( sint4& value, sint32 x, sint32 y, sint32 z, sint32 w ) { value.x = x; value.y = y; value.z = z; value.w = w; }


	// half
	TIKI_REFLECTION_STRUCT(
		half2,
		TIKI_REFLECTION_FIELD( float16, x )
		TIKI_REFLECTION_FIELD( float16, y )
	);

	TIKI_REFLECTION_STRUCT(
		half3,
		TIKI_REFLECTION_FIELD( float16, x )
		TIKI_REFLECTION_FIELD( float16, y )
		TIKI_REFLECTION_FIELD( float16, z )
	);

	TIKI_REFLECTION_STRUCT(
		half4,
		TIKI_REFLECTION_FIELD( float16, x )
		TIKI_REFLECTION_FIELD( float16, y )
		TIKI_REFLECTION_FIELD( float16, z )
		TIKI_REFLECTION_FIELD( float16, w )
	);

	TIKI_FORCE_INLINE void createHalf2( half2& value, float16 x, float16 y ) { value.x = x; value.y = y; }
	TIKI_FORCE_INLINE void createHalf3( half3& value, float16 x, float16 y, float16 z ) { value.x = x; value.y = y; value.z = z; }
	TIKI_FORCE_INLINE void createHalf4( half4& value, float16 x, float16 y, float16 z, float16 w ) { value.x = x; value.y = y; value.z = z; value.w = w; }


	// float
	TIKI_REFLECTION_STRUCT(
		float2,
		TIKI_REFLECTION_FIELD( float, x )
		TIKI_REFLECTION_FIELD( float, y )
	);

	TIKI_REFLECTION_STRUCT(
		float3,
		TIKI_REFLECTION_FIELD( float, x )
		TIKI_REFLECTION_FIELD( float, y )
		TIKI_REFLECTION_FIELD( float, z )
	);

	TIKI_REFLECTION_STRUCT(
		float4,
		TIKI_REFLECTION_FIELD( float, x )
		TIKI_REFLECTION_FIELD( float, y )
		TIKI_REFLECTION_FIELD( float, z )
		TIKI_REFLECTION_FIELD( float, w )
	);

	TIKI_FORCE_INLINE void createFloat2( float2& value, float x, float y ) { value.x = x; value.y = y; }
	TIKI_FORCE_INLINE void createFloat3( float3& value, float x, float y, float z ) { value.x = x; value.y = y; value.z = z; }
	TIKI_FORCE_INLINE void createFloat4( float4& value, float x, float y, float z, float w ) { value.x = x; value.y = y; value.z = z; value.w = w; }
}

#endif // TIKI_STRUCTS_HPP
