#pragma once

#include "tiki/base/string.hpp"
#include <stdio.h>

namespace tiki
{
	class StringConvert
	{
	public:

		static string ToString(sint16 value)
		{
			return sintegerToString<char, sint16>(value);
		}

		static string ToString(sint32 value)
		{
			return sintegerToString<char, sint32>(value);
		}

		static string ToString(sint64 value)
		{
			return sintegerToString<char, sint64>(value);
		}

		static string ToString(uint16 value)
		{
			return sintegerToString<char, uint16>(value);
		}

		static string ToString(uint32 value)
		{
			return sintegerToString<char, uint32>(value);
		}

		static string ToString(uint64 value)
		{
			return sintegerToString<char, uint64>(value);
		}

		static string ToString(float value)
		{
			return floatToString<char, float>(value);
		}

		static string ToString(double value)
		{
			return floatToString<char, double>(value);
		}

	private:

		template <typename TString, typename TInt>
		static BasicString<TString> sintegerToString(TInt value)
		{
			uint32 len = 0;
			TInt val = (TInt)abs((sint32)value);

			if (value < 0)
				len++;

			if (value == 0)
				return BasicString<TString>::numberZero;

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

			BasicString<TString> str = BasicString<TString>(len);

			uint32 a = 0;
			if (value < 0)
			{
				str[0] = BasicString<TString>::numberMinus;
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

				str[a] = BasicString<TString>::numberZero + (TString)i2;

				i++;
				a++;
			}

			return str;
		}

		template <typename TString, typename TFloat>
		static BasicString<TString> floatToString(TFloat value)
		{
			TString buffer[ 64u ];

			if (sizeof(TString) == 1)
			{
				_snprintf_s((char*)buffer, 64, 64, "%f", value);
			}
			else
			{
				_snwprintf_s((wchar_t*)buffer, 64, 64, L"%f", value);
			}

			return BasicString<TString>(buffer);
		}

	};
}