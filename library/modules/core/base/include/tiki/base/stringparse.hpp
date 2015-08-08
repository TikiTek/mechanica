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

		static sint16 parseInt16( cstring string )
		{
			return (uint16)atoi( string );
			//return parseSigedInteger<char, sint16>(string);
		}

		static sint32 parseInt32( cstring string )
		{
			return (sint32)atoi( string );
			//return parseSigedInteger<char, sint32>(string);
		}

		static sint64 parseInt64( cstring string )
		{
			if ( string[ 0u ] == '0' && string[ 1u ] == 'x' )
			{
				return parseSingedHex( string );
			}

			return (sint64)atol( string );
			//return parseSigedInteger<char, sint64>(string);
		}

		static uint16 parseUInt16( cstring string )
		{
			return (uint16)atoi( string );
			//return parseUnsigedInteger<char, uint16>(string);
		}

		static uint32 parseUInt32( cstring string )
		{
			return (uint32)atoi( string );
			//return parseUnsigedInteger<char, uint32>(string);
		}

		static uint64 parseUInt64( cstring string )
		{
			if ( string[ 0u ] == '0' && string[ 1u ] == 'x' )
			{
				return parseUnsingedHex( string );
			}

			return (uint64)atol( string );
			//return parseUnsigedInteger<char, uint64>(string);
		}

		static sint64 parseSingedHex(cstring string)
		{
			return (sint64)strtol( string, nullptr, 16 );
		}

		static uint64 parseUnsingedHex(cstring string)
		{
			return (uint64)strtol( string, nullptr, 16 );
		}

		static float parseSingle( cstring string )
		{
			return (float)atof( string );
			//return parseFloat<char, float>(string);
		}

		static double parseDouble( cstring string )
		{
			return (double)atof( string );
			//return parseFloat<char, double>(string);
		}

	private:

		template <typename TString>
		static bool getSiged(BasicString<TString>& string)
		{
			if (string[0] == BasicString<TString>::numberPlus)
			{
				string = string.subString( 1u );
			}

			if (string[0] == BasicString<TString>::numberMinus)
			{
				string = string.subString( 1u );
				return false;
			}

			return true;
		}

		template<typename TString, typename TUInt>
		static TUInt parseUnsigedInteger(const BasicString<TString>& string)
		{
			TUInt num = 0;

			uint32 i = 0;
			while (i < string.data->stringLength)
			{
				if (string[i] >= BasicString<TString>::numberZero && string[i] <= BasicString<TString>::numberNine)
				{
					int p = int( string.data->stringLength - ( i + 1 ) );
					TUInt c = string[i] - BasicString<TString>::numberZero;

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

		template<typename TString, typename TInt>
		static TInt parseSigedInteger(const BasicString<TString>& string)
		{
			BasicString<TString> str = string;
			TInt mul = (getSiged(str) ? 1 : -1);

			return parseUnsigedInteger<TString, TInt>(str) * mul;
		}

		template <typename TString, typename TFloat>
		static TFloat parseFloat(const BasicString<TString>& string)
		{
			return (TFloat)atof( string.cStr() );

			BasicString<TString> str = string;
			TFloat mul = (TFloat)(getSiged(str) ? 1.0 : -1.0);
			TFloat num = 0;

			uint32 i = 0;
			uint32 dotIndex = str.indexOf(BasicString<TString>::numberDot);
			while ( i < str.data->stringLength )
			{
				if (str[i] >= BasicString<TString>::numberZero && str[i] <= BasicString<TString>::numberNine)
				{
					int p = dotIndex - (i < dotIndex ? i + 1 : i);
					TFloat c = (TFloat)(str[i] - BasicString<TString>::numberZero);

					num += c * (TFloat)pow(10.0, p);
				}
				else
				{
					return (TFloat)-1.0;
				}

				i++;

				if (i == dotIndex)
					i++;
			}

			return num * mul;
		}
	};
}
