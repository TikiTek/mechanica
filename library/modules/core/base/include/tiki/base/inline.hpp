#pragma once
#ifndef TIKI_INLINE_HPP
#define TIKI_INLINE_HPP

#include "tiki/base/types.hpp"

#define TIKI_USE_INLINE TIKI_ON

#if TIKI_ENABLED( TIKI_USE_INLINE )

#	if TIKI_ENABLED( TIKI_BUILD_MSVC )

#		define TIKI_INLINE			inline
#		define TIKI_FORCE_INLINE	__forceinline
#		define TIKI_NO_INLINE		__declspec(noinline)

#	elif TIKI_ENABLED( TIKI_BUILD_MINGW )

#		define TIKI_INLINE			inline
#		define TIKI_FORCE_INLINE	inline //__attribute__((always_inline))
#		define TIKI_NO_INLINE		__attribute__((noinline))

#	else

#		error Platform not implemented

#	endif

#else

#	define TIKI_INLINE
#	define TIKI_FORCE_INLINE
#	define TIKI_NO_INLINE

#endif

#endif // TIKI_INLINE_HPP
