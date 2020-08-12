#pragma once
#ifndef TIKI_NUMBERS_HPP__INCLUDED
#define TIKI_NUMBERS_HPP__INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	namespace u32
	{
		TIKI_FORCE_INLINE uint32 floatToUnorm( const float value )
		{
			TIKI_ASSERT( value >= 0.0f && value <= 1.0f );
			return (uint32)( value * 4294967295.0f );
		}

		TIKI_FORCE_INLINE float unormToFloat( const uint32 value )
		{
			const float mul = 1.0f / 4294967295.0f;
			return (float)value * mul;
		}
	}

	namespace s32
	{
		TIKI_FORCE_INLINE sint32 floatToSnorm( const float value )
		{
			TIKI_ASSERT( value >= -1.0f && value <= 1.0f );
			return (sint32)( value * 2147483647.0f );
		}

		TIKI_FORCE_INLINE float snormToFloat( const sint32 value )
		{
			const float mul = 1.0f / 2147483647.0f;
			return (float)value * mul;
		}
	}

	namespace u16
	{
		TIKI_FORCE_INLINE uint16 floatToUnorm( const float value )
		{
			TIKI_ASSERT( value >= 0.0f && value <= 1.0f );
			return (uint16)( value * 65535.0f );
		}

		TIKI_FORCE_INLINE float unormToFloat( const uint16 value )
		{
			const float mul = 1.0f / 65535.0f;
			return (float)value * mul;
		}
	}
	
	namespace s16
	{
		TIKI_FORCE_INLINE sint16 floatToSnorm( const float value )
		{
			TIKI_ASSERT( value >= -1.0f && value <= 1.0f );
			return (sint16)( value * 32667.0f );
		}

		TIKI_FORCE_INLINE float snormToFloat( const sint16 value )
		{
			const float mul = 1.0f / 32667.0f;
			return (float)value * mul;
		}
	}

	namespace u8
	{
		TIKI_FORCE_INLINE uint8 floatToUnorm( const float value )
		{
			TIKI_ASSERT( value >= 0.0f && value <= 1.0f );
			return (uint8)( value * 255.0f );
		}

		TIKI_FORCE_INLINE float unormToFloat( const uint16 value )
		{
			const float mul = 1.0f / 255.0f;
			return (float)value * mul;
		}
	}

	namespace s8
	{
		TIKI_FORCE_INLINE sint8 floatToSnorm( const float value )
		{
			TIKI_ASSERT( value >= -1.0f && value <= 1.0f );
			return (sint8)( value * 127.0f );
		}

		TIKI_FORCE_INLINE float snormToFloat( const sint8 value )
		{
			const float mul = 1.0f / 127.0f;
			return (float)value * mul;
		}
	}
}

#endif // TIKI_NUMBERS_HPP__INCLUDED
