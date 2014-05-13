#pragma once

#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include <math.h>
#include <stdlib.h>

namespace tiki
{
	class ParseString
	{
	public:

		static sint16 parseInt16(const string& string)
		{			
			return parseSigedInteger<char, sint16>(string);
		}

		static sint32 parseInt32(const string& string)
		{			
			return parseSigedInteger<char, sint32>(string);
		}

		static sint64 parseInt64(const string& string)
		{			
			return parseSigedInteger<char, sint64>(string);
		}

		static uint16 parseUInt16(const string& string)
		{			
			return parseUnsigedInteger<char, uint16>(string);
		}

		static uint32 parseUInt32(const string& string)
		{
			return (uint32)atoi( string.cStr() );
			//return parseUnsigedInteger<char, uint32>(string);
		}

		static uint64 parseUInt64(const string& string)
		{			
			return parseUnsigedInteger<char, uint64>(string);
		}

		static float parseSingle(const string& string)
		{
			return (float)atof( string.cStr() );
			//return parseFloat<char, float>(string);
		}

		static double parseDouble(const string& string)
		{			
			return parseFloat<char, double>(string);
		}

		static sint16 wparseInt16(const wstring& string)
		{			
			return parseSigedInteger<wchar_t, sint16>(string);
		}

		static sint32 wparseInt32(const wstring& string)
		{			
			return parseSigedInteger<wchar_t, sint32>(string);
		}

		static sint64 wparseInt64(const wstring& string)
		{			
			return parseSigedInteger<wchar_t, sint64>(string);
		}

		static uint16 wparseUInt16(const wstring& string)
		{			
			return parseUnsigedInteger<wchar_t, uint16>(string);
		}

		static uint32 wparseUInt32(const wstring& string)
		{			
			return parseUnsigedInteger<wchar_t, uint32>(string);
		}

		static uint64 wparseUInt64(const wstring& string)
		{			
			return parseUnsigedInteger<wchar_t, uint64>(string);
		}

/*
		static float wparseSingle(const wstring& string)
		{			
			return parseFloat<wchar_t, float>(string);
		}

		static double wparseDouble(const wstring& string)
		{			
			return parseFloat<wchar_t, double>(string);
		}*/

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
					return (TUInt)-1;
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