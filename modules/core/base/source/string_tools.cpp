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

		uint index = getStringLength( buffer ) - 1u;
		while( index > 0 )
		{
			if( buffer[ index - 1u ] == '.' || buffer[ index ] != '0' )
			{
				break;
			}
			else if( buffer[ index ] == '0' )
			{
				buffer[ index ] = '\0';
			}

			index--;
		}

		return string( buffer );
	}

	string string_tools::toString( float64 value )
	{
		char buffer[ 64u ];
		sprintf( buffer, "%f", value );

		uint index = getStringLength( buffer ) - 1u;
		while( index > 0 )
		{
			if( buffer[ index - 1u ] == '.' || buffer[ index ] != '0' )
			{
				break;
			}
			else if( buffer[ index ] == '0' )
			{
				buffer[ index ] = '\0';
			}

			index--;
		}

		return string( buffer );
	}

	sint8 string_tools::parseSInt8( const char* pString )
	{
		return (sint8)atoi( pString );
	}

	sint16 string_tools::parseSInt16( const char* pString )
	{
		return (sint16)atoi( pString );
	}

	sint32 string_tools::parseSInt32( const char* pString )
	{
		return (sint32)atoi( pString );
	}

	sint64 string_tools::parseSInt64( const char* pString )
	{
		return (sint64)atoll( pString );
	}

	uint8 string_tools::parseUInt8( const char* pString )
	{
		return (uint8)atoi( pString );
	}

	uint16 string_tools::parseUInt16( const char* pString )
	{
		return (uint16)atoi( pString );
	}

	uint32 string_tools::parseUInt32( const char* pString )
	{
		return (uint32)atoi( pString );
	}

	uint64 string_tools::parseUInt64( const char* pString )
	{
		return (uint64)atoll( pString );
	}

	uint8 string_tools::parseUInt8Hex( const char* pString )
	{
		return (uint8)strtoll( pString, nullptr, 16 );
	}

	uint16 string_tools::parseUInt16Hex( const char* pString )
	{
		return (uint16)strtoll( pString, nullptr, 16 );
	}

	uint32 string_tools::parseUInt32Hex( const char* pString )
	{
		return (uint32)strtoll( pString, nullptr, 16 );
	}

	uint64 string_tools::parseUInt64Hex( const char* pString )
	{
		return (uint64)strtoll( pString, nullptr, 16 );
	}

	float16 string_tools::parseFloat16( const char* pString )
	{
		return f16::convertFloat32to16( (float32)atof( pString ) );
	}

	float32 string_tools::parseFloat32( const char* pString )
	{
		return (float32)atof( pString );
	}

	float64 string_tools::parseFloat64( const char* pString )
	{
		return atof( pString );
	}

	bool string_tools::tryParseSInt8( sint8& target, const char* pString )
	{
		char* pEnd = nullptr;
		const sint64 result = strtoll( pString, &pEnd, 10 );
		if( pEnd == pString || !rangeCheckCast( target, result ) )
		{
			return false;
		}

		return true;
	}

	bool string_tools::tryParseSInt16( sint16& target, const char* pString )
	{
		char* pEnd = nullptr;
		const sint64 result = strtoll( pString, &pEnd, 10 );
		if( pEnd == pString || !rangeCheckCast( target, result ) )
		{
			return false;
		}

		return true;
	}

	bool string_tools::tryParseSInt32( sint32& target, const char* pString )
	{
		char* pEnd = nullptr;
		const sint64 result = strtoll( pString, &pEnd, 10 );
		if( pEnd == pString || !rangeCheckCast( target, result ) )
		{
			return false;
		}

		return true;
	}

	bool string_tools::tryParseSInt64( sint64& target, const char* pString )
	{
		char* pEnd = nullptr;
		const sint64 result = strtoll( pString, &pEnd, 10 );
		if( pEnd == pString || !rangeCheckCast( target, result ) )
		{
			return false;
		}

		return true;
	}

	bool string_tools::tryParseUInt8( uint8& target, const char* pString )
	{
		char* pEnd = nullptr;
		const uint64 result = (uint64)strtoll( pString, &pEnd, 10 );
		if( pEnd == pString || !rangeCheckCast( target, result ) )
		{
			return false;
		}

		return true;
	}

	bool string_tools::tryParseUInt16( uint16& target, const char* pString )
	{
		char* pEnd = nullptr;
		const uint64 result = (uint64)strtoll( pString, &pEnd, 10 );
		if( pEnd == pString || !rangeCheckCast( target, result ) )
		{
			return false;
		}

		return true;
	}

	bool string_tools::tryParseUInt32( uint32& target, const char* pString )
	{
		char* pEnd = nullptr;
		const uint64 result = (uint64)strtoll( pString, &pEnd, 10 );
		if( pEnd == pString || !rangeCheckCast( target, result ) )
		{
			return false;
		}

		return true;
	}

	bool string_tools::tryParseUInt64( uint64& target, const char* pString )
	{
		char* pEnd = nullptr;
		const uint64 result = (uint64)strtoll( pString, &pEnd, 10 );
		if( pEnd == pString || !rangeCheckCast( target, result ) )
		{
			return false;
		}

		return true;
	}

	bool string_tools::tryParseUInt8Hex( uint8& target, const char* pString )
	{
		char* pEnd = nullptr;
		const uint64 result = (uint64)strtoll( pString, &pEnd, 16 );
		if( pEnd == pString || !rangeCheckCast( target, result ) )
		{
			return false;
		}

		return true;
	}

	bool string_tools::tryParseUInt16Hex( uint16& target, const char* pString )
	{
		char* pEnd = nullptr;
		const uint64 result = (uint64)strtoll( pString, &pEnd, 16 );
		if( pEnd == pString || !rangeCheckCast( target, result ) )
		{
			return false;
		}

		return true;
	}

	bool string_tools::tryParseUInt32Hex( uint32& target, const char* pString )
	{
		char* pEnd = nullptr;
		const uint64 result = (uint64)strtoll( pString, &pEnd, 16 );
		if( pEnd == pString || !rangeCheckCast( target, result ) )
		{
			return false;
		}

		return true;
	}

	bool string_tools::tryParseUInt64Hex( uint64& target, const char* pString )
	{
		char* pEnd = nullptr;
		const uint64 result = (uint64)strtoll( pString, &pEnd, 16 );
		if( pEnd == pString || !rangeCheckCast( target, result ) )
		{
			return false;
		}

		return true;
	}

	bool string_tools::tryParseFloat16( float16& target, const char* pString )
	{
		char* pEnd = nullptr;
		const float64 result = strtof( pString, &pEnd );
		if( pEnd == pString || pEnd == pString )
		{
			return false;
		}

		target = f16::convertFloat32to16( (float32)result );
		return true;
	}

	bool string_tools::tryParseFloat32( float32& target, const char* pString )
	{
		char* pEnd = nullptr;
		const float64 result = strtof( pString, &pEnd );
		if( pEnd == pString || pEnd == pString )
		{
			return false;
		}

		target = (float32)result;
		return true;
	}

	bool string_tools::tryParseFloat64( float64& target, const char* pString )
	{
		char* pEnd = nullptr;
		const float64 result = strtof( pString, &pEnd );
		if( pEnd == pString || pEnd == pString )
		{
			return false;
		}

		target = result;
		return true;
	}
}
