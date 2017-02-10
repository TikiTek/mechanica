#include "tiki/base/string_tools.hpp"

#include "tiki/base/float16.hpp"

#include <stdio.h>
#include <cstdlib>
#include <math.h>

namespace tiki
{
	string string_tools::toString( sint8 value )
	{
		char buffer[ 64u ];
		sprintf( buffer, "%i", value );
		return string( buffer );
	}

	string string_tools::toString( sint16 value )
	{
		char buffer[ 64u ];
		sprintf( buffer, "%i", value );
		return string( buffer );
	}

	string string_tools::toString( sint32 value )
	{
		char buffer[ 64u ];
		sprintf( buffer, "%i", value );
		return string( buffer );
	}

	string string_tools::toString( sint64 value )
	{
		char buffer[ 64u ];
		sprintf( buffer, "%lli", value );
		return string( buffer );
	}

	string string_tools::toString( uint8 value )
	{
		char buffer[ 64u ];
		sprintf( buffer, "%u", value );
		return string( buffer );
	}

	string string_tools::toString( uint16 value )
	{
		char buffer[ 64u ];
		sprintf( buffer, "%u", value );
		return string( buffer );
	}

	string string_tools::toString( uint32 value )
	{
		char buffer[ 64u ];
		sprintf( buffer, "%u", value );
		return string( buffer );
	}

	string string_tools::toString( uint64 value )
	{
		char buffer[ 64u ];
		sprintf( buffer, "%llu", value );
		return string( buffer );
	}

	string string_tools::toString( float32 value )
	{
		char buffer[ 64u ];
		sprintf( buffer, "%f", value );
		return string( buffer );
	}

	string string_tools::toString( float64 value )
	{
		char buffer[ 64u ];
		sprintf( buffer, "%f", value );
		return string( buffer );
	}

	sint8 string_tools::parseSInt8( const char* string )
	{
		return (sint8)atoi( string );
	}

	sint16 string_tools::parseSInt16( const char* string )
	{
		return (sint16)atoi( string );
	}

	sint32 string_tools::parseSInt32( const char* string )
	{
		return (sint32)atoi( string );
	}

	sint64 string_tools::parseSInt64( const char* string )
	{
		return (sint64)atoll( string );
	}

	uint8 string_tools::parseUInt8( const char* string )
	{
		return (uint8)atoi( string );
	}

	uint16 string_tools::parseUInt16( const char* string )
	{
		return (uint16)atoi( string );
	}

	uint32 string_tools::parseUInt32( const char* string )
	{
		return (uint32)atoi( string );
	}

	uint64 string_tools::parseUInt64( const char* string )
	{
		return (uint64)atoll( string );
	}

	uint8 string_tools::parseUInt8Hex( const char* string )
	{
		return (uint8)strtoll( string, nullptr, 16 );
	}

	uint16 string_tools::parseUInt16Hex( const char* string )
	{
		return (uint16)strtoll( string, nullptr, 16 );
	}

	uint32 string_tools::parseUInt32Hex( const char* string )
	{
		return (uint32)strtoll( string, nullptr, 16 );
	}

	uint64 string_tools::parseUInt64Hex( const char* string )
	{
		return (uint64)strtoll( string, nullptr, 16 );
	}

	float16 string_tools::parseFloat16( const char* string )
	{
		return f16::convertFloat32to16( (float32)atof( string ) );
	}

	float32 string_tools::parseFloat32( const char* string )
	{
		return (float32)atof( string );
	}

	float64 string_tools::parseFloat64( const char* string )
	{
		return (double)atof( string );
	}
}
