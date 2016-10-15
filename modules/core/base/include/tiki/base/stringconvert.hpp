#pragma once

#include "tiki/base/string.hpp"

#include <stdio.h>
#include <cstdlib>
#include <math.h>

namespace tiki
{
	class StringConvert
	{
	public:

		static string ToString(sint16 value)
		{
			return integerToString<sint16>(value);
		}

		static string ToString(sint32 value)
		{
			return integerToString<sint32>(value);
		}

		static string ToString(sint64 value)
		{
			return integerToString<sint64>(value);
		}

		static string ToString(uint16 value)
		{
			return integerToString<uint16>(value);
		}

		static string ToString(uint32 value)
		{
			return integerToString<uint32>(value);
		}

		static string ToString(uint64 value)
		{
			return integerToString<uint64>(value);
		}

		static string ToString(float value)
		{
			return floatToString<float>(value);
		}

		static string ToString(double value)
		{
			return floatToString<double>(value);
		}

	private:

		template <typename TInt>
		static string integerToString(TInt value)
		{
			if( value == 0 )
			{
				return "0";
			}

			const TInt absoluteValue = (TInt)abs( (sint64)value );
			const TInt valLog10 = (TInt)log10( (double)absoluteValue );
			const TInt stringLength = valLog10 + 1 + (value < 0 ? 1 : 0);
			TInt i2 = (TInt)pow( 10.0, (double)valLog10 );

			string str = string( stringLength );

			TInt a = 0;
			if (value < 0)
			{
				str[0] = '-';
				a++;
			}

			TInt i = 0;
			TInt i3 = 0;
			while (a < stringLength )
			{
				i2 = absoluteValue / (TInt)pow(10.0, (double)(stringLength - i) - 1);
				i2 -= i3;
				i3 += i2;
				i3 *= 10;

				str[a] = '0' + (char)i2;

				i++;
				a++;
			}
			str[ stringLength - 1u ] = '\0';

			return str;
		}

		template <typename TFloat>
		static string floatToString(TFloat value)
		{
			char buffer[ 64u ];
			sprintf( buffer, "%f", value);

			return string(buffer);
		}

	};
}