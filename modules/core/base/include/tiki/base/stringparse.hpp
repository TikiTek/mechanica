#pragma once

#include "tiki/base/basicstring.hpp"
#include "tiki/base/types.hpp"

#include <math.h>
#include <stdlib.h>

namespace tiki
{
	class ParseString
	{
	public:

		static sint16 parseInt16( const char* string )
		{
			return (uint16)atoi( string );
			//return parseSigedInteger<char, sint16>(string);
		}

		static sint32 parseInt32( const char* string )
		{
			return (sint32)atoi( string );
			//return parseSigedInteger<char, sint32>(string);
		}

		static sint64 parseInt64( const char* string )
		{
			if ( string[ 0u ] == '0' && string[ 1u ] == 'x' )
			{
				return parseSingedHex( string );
			}

			return (sint64)atoll( string );
			//return parseSigedInteger<char, sint64>(string);
		}

		static uint16 parseUInt16( const char* string )
		{
			return (uint16)atoi( string );
			//return parseUnsigedInteger<char, uint16>(string);
		}

		static uint32 parseUInt32( const char* string )
		{
			return (uint32)atoi( string );
			//return parseUnsigedInteger<char, uint32>(string);
		}

		static uint64 parseUInt64( const char* string )
		{
			if ( string[ 0u ] == '0' && string[ 1u ] == 'x' )
			{
				return parseUnsingedHex( string );
			}

			return (uint64)atoll( string );
			//return parseUnsigedInteger<char, uint64>(string);
		}

		static sint64 parseSingedHex(const char* string)
		{
			return (sint64)strtoll( string, nullptr, 16 );
		}

		static uint64 parseUnsingedHex(const char* string)
		{
			return (uint64)strtoll( string, nullptr, 16 );
		}

		static float parseSingle( const char* string )
		{
			return (float)atof( string );
			//return parseFloat<char, float>(string);
		}

		static double parseDouble( const char* string )
		{
			return (double)atof( string );
			//return parseFloat<char, double>(string);
		}

	private:

		static bool getSiged(string& str)
		{
			if (str[0] == '+')
			{
				str = str.subString( 1u );
			}

			if (str[0] == '-')
			{
				str = str.subString( 1u );
				return false;
			}

			return true;
		}

		template<typename TUInt>
		static TUInt parseUnsigedInteger(const string& str)
		{
			TUInt num = 0;

			uint32 i = 0;
			while (i < str.data->stringLength)
			{
				if (str[i] >= '0' && str[i] <= '9')
				{
					int p = int( str.data->stringLength - ( i + 1 ) );
					TUInt c = str[i] - '0';

					num += c * (TUInt)pow(10.0, p);
				}
				else
				{
					return num;
				}

				i++;
			}

			return num;
		}

		template<typename TInt>
		static TInt parseSigedInteger(const string& inStr)
		{
			string str = inStr;
			TInt mul = (getSiged(str) ? 1 : -1);

			return parseUnsigedInteger<TInt>(str) * mul;
		}

		template <typename TFloat>
		static TFloat parseFloat(const string& str)
		{
			return (TFloat)atof( str.cStr() );

			//BasicString<TString> str = string;
			//TFloat mul = (TFloat)(getSiged(str) ? 1.0 : -1.0);
			//TFloat num = 0;

			//uint32 i = 0;
			//uint32 dotIndex = str.indexOf(BasicString<TString>::numberDot);
			//while ( i < str.data->stringLength )
			//{
			//	if (str[i] >= BasicString<TString>::numberZero && str[i] <= BasicString<TString>::numberNine)
			//	{
			//		int p = dotIndex - (i < dotIndex ? i + 1 : i);
			//		TFloat c = (TFloat)(str[i] - BasicString<TString>::numberZero);

			//		num += c * (TFloat)pow(10.0, p);
			//	}
			//	else
			//	{
			//		return (TFloat)-1.0;
			//	}

			//	i++;

			//	if (i == dotIndex)
			//		i++;
			//}

			//return num * mul;
		}
	};
}
