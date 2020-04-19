
#include "tiki/base/string.hpp"

#include <cstdarg>
#include <cstdio>

#if TIKI_ENABLED( TIKI_BUILD_MSVC )
#   include <windows.h>
#endif

namespace tiki
{
	void formatStringBuffer( char* pTargetBuffer, uintreg bufferSize, const char* pFormat, ... )
	{
		va_list argptr;
		va_start( argptr, pFormat );
		formatStringBufferArgs( pTargetBuffer, bufferSize, pFormat, argptr );
		va_end( argptr );
	}

#if TIKI_ENABLED( TIKI_BUILD_MSVC )

	bool convertUtf8ToWidecharString( wchar_t* pTargetBuffer, uintreg targetLengthInCharacters, const char* pSourceBuffer )
	{
		const int result = MultiByteToWideChar(
			CP_UTF8,
			0,
			pSourceBuffer,
			-1,
			pTargetBuffer,
			(int)targetLengthInCharacters
		);

		return result != 0;
	}

	bool convertWidecharToUtf8String( char* pTargetBuffer, uintreg targetLengthInCharacters, const wchar_t* pSourceBuffer, uintreg length /*= 0u*/ )
	{
		const int sourceCount = (length == 0u ? -1 : int(length));
		const int result = WideCharToMultiByte(
			CP_UTF8, 0,
			pSourceBuffer,
			sourceCount,
			pTargetBuffer,
			(int)targetLengthInCharacters,
			nullptr,
			nullptr
		);
		pTargetBuffer[ result ] = '\0';

		return result != 0;
	}

	void formatStringBufferArgs( char* pTargetBuffer, uintreg targetLength, const char* pFormat, va_list args )
	{
		_vsprintf_s_l(
			pTargetBuffer,
			targetLength,
			pFormat,
			nullptr,
			args
		);
	}

#elif TIKI_ENABLED( TIKI_BUILD_GCC ) || TIKI_ENABLED( TIKI_BUILD_CLANG )

	void formatStringBufferArgs( char* pTargetBuffer, uintreg targetLength, const char* pFormat, va_list args )
	{
		vsnprintf( pTargetBuffer, bufferSize, pFormat, args );
	}

#endif
}