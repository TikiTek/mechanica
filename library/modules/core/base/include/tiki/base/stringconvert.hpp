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
			return sintegerToString<sint16>(value);
		}

		static string ToString(sint32 value)
		{
			return sintegerToString<sint32>(value);
		}

		static string ToString(sint64 value)
		{
			return sintegerToString<sint64>(value);
		}

		static string ToString(uint16 value)
		{
			return sintegerToString<uint16>(value);
		}

		static string ToString(uint32 value)
		{
			return sintegerToString<uint32>(value);
		}

		static string ToString(uint64 value)
		{
			return sintegerToString<uint64>(value);
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
		static string sintegerToString(TInt value)
		{
			uint32 len = 0;
			sint32 valAbs = abs( (sint32)value );
			TInt val = valAbs;

			if (value < 0)
				len++;

			if (value == 0)
				return "0";

			TInt i = 1;
			TInt i2 = i;
			while (true)
			{
				if (i > val) break;

				len++;
				i2 = i;
				i *= 10;

				if (i < i2) break;
			}

			string str = string(len);

			uint32 a = 0;
			if (value < 0)
			{
				str[0] = string::numberMinus;
				a++;
			}

			i = 0;
			TInt i3 = 0;
			while (a < len)
			{
				i2 = val / (TInt)pow(10.0, (double)(len - a) - 1);
				i2 -= i3;
				i3 += i2;
				i3 *= 10;

				str[a] = string::numberZero + (char)i2;

				i++;
				a++;
			}

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