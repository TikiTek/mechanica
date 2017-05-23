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

		sint8	parseSInt8( const char* pString );
		sint16	parseSInt16( const char* pString );
		sint32	parseSInt32( const char* pString );
		sint64	parseSInt64( const char* pString );

		uint8	parseUInt8( const char* pString );
		uint16	parseUInt16( const char* pString );
		uint32	parseUInt32( const char* pString );
		uint64	parseUInt64( const char* pString );

		uint8	parseUInt8Hex( const char* pString );
		uint16	parseUInt16Hex( const char* pString );
		uint32	parseUInt32Hex( const char* pString );
		uint64	parseUInt64Hex( const char* pString );

		float16	parseFloat16( const char* pString );
		float32	parseFloat32( const char* pString );
		float64	parseFloat64( const char* pString );

		bool	tryParseSInt8( sint8& target, const char* pString );
		bool	tryParseSInt16( sint16& target, const char* pString );
		bool	tryParseSInt32( sint32& target, const char* pString );
		bool	tryParseSInt64( sint64& target, const char* pString );

		bool	tryParseUInt8( uint8& target, const char* pString );
		bool	tryParseUInt16( uint16& target, const char* pString );
		bool	tryParseUInt32( uint32& target, const char* pString );
		bool	tryParseUInt64( uint64& target, const char* pString );

		bool	tryParseUInt8Hex( uint8& target, const char* pString );
		bool	tryParseUInt16Hex( uint16& target, const char* pString );
		bool	tryParseUInt32Hex( uint32& target, const char* pString );
		bool	tryParseUInt64Hex( uint64& target, const char* pString );

		bool	tryParseFloat16( float16& target, const char* pString );
		bool	tryParseFloat32( float32& target, const char* pString );
		bool	tryParseFloat64( float64& target, const char* pString );
	}
}

#endif // TIKI_STRING_TOOLS_HPP_INCLUDED
