#pragma once
#ifndef TIKI_NUMBERS_HPP__INCLUDED
#define TIKI_NUMBERS_HPP__INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	namespace u16
	{
		uint16 floatToUnorm( const float value )
		{
			TIKI_ASSERT( value >= 0.0f && value <= 1.0f );
			return (uint16)( value * 65535.0f );
		}

		float unormToFloat( const uint16 value )
		{
			const float mul = 1.0f / 65535.0f;
			return (float)value * mul;
		}
	}
	
	namespace s16
	{
		sint16 floatToSnorm( const float value )
		{
			TIKI_ASSERT( value >= -1.0f && value <= 1.0f );
			return (sint16)( value * 32667.0f );
		}

		float snormToFloat( const sint16 value )
		{
			const float mul = 1.0f / 32667.0f;
			return (float)value * mul;
		}
	}

	namespace u8
	{
		uint8 floatToUnorm( const float value )
		{
			TIKI_ASSERT( value >= 0.0f && value <= 1.0f );
			return (uint8)( value * 255.0f );
		}

		float unormToFloat( const uint16 value )
		{
			const float mul = 1.0f / 255.0f;
			return (float)value * mul;
		}
	}

	namespace s8
	{
		sint8 floatToSnorm( const float value )
		{
			TIKI_ASSERT( value >= -1.0f && value <= 1.0f );
			return (sint8)( value * 127.0f );
		}

		float snormToFloat( const sint8 value )
		{
			const float mul = 1.0f / 127.0f;
			return (float)value * mul;
		}
	}
}

#endif // TIKI_NUMBERS_HPP__INCLUDED
