#pragma once
#ifndef TIKI_INLINE_HPP
#define TIKI_INLINE_HPP

#define TIKI_USE_INLINE 1

#if TIKI_USE_INLINE
#	if TIKI_PLATFORM_WIN
#		define TIKI_INLINE	inline
#		define TIKI_FORCE_INLINE __forceinline
#		define TIKI_NO_INLINE __declspec(noinline)
#	elif TIKI_PLATFORM_LINUX
#		define TIKI_INLINE	inline
#		define TIKI_FORCE_INLINE __attribute__((always_inline))
#		define TIKI_NO_INLINE __attribute__((noinline))
#	endif
#else
#	define TIKI_INLINE
#	define TIKI_FORCE_INLINE
#	define TIKI_NO_INLINE
#endif

#endif // TIKI_INLINE_HPP
