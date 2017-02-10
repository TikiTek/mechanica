#pragma once
#ifndef TIKI_STRING_TOOLS_HPP_INCLUDED
#define TIKI_STRING_TOOLS_HPP_INCLUDED

#include "tiki/base/dynamic_string.hpp"

namespace tiki
{
	namespace string_tools
	{
		string	toString( sint8 value );
		string	toString( sint16 value );
		string	toString( sint32 value );
		string	toString( sint64 value );
		string	toString( uint8 value );
		string	toString( uint16 value );
		string	toString( uint32 value );
		string	toString( uint64 value );
		string	toString( float32 value );
		string	toString( float64 value );

		sint8	parseSInt8( const char* string );
		sint16	parseSInt16( const char* string );
		sint32	parseSInt32( const char* string );
		sint64	parseSInt64( const char* string );

		uint8	parseUInt8( const char* string );
		uint16	parseUInt16( const char* string );
		uint32	parseUInt32( const char* string );
		uint64	parseUInt64( const char* string );

		uint8	parseUInt8Hex( const char* string );
		uint16	parseUInt16Hex( const char* string );
		uint32	parseUInt32Hex( const char* string );
		uint64	parseUInt64Hex( const char* string );

		float16	parseFloat16( const char* string );
		float32	parseFloat32( const char* string );
		float64	parseFloat64( const char* string );
	}
}

#endif // TIKI_STRING_TOOLS_HPP_INCLUDED
