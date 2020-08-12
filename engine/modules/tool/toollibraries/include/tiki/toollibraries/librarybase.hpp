#pragma once
#ifndef __TIKI_LIBRARYBASE_HPP_INCLUDED__
#define __TIKI_LIBRARYBASE_HPP_INCLUDED__

#include "tiki/base/types.hpp"

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
#	if TIKI_ENABLED( TIKI_BUILD_LIBRARY )
#		define TIKI_LIBRARY_METHOD	__declspec( dllexport )
#	else
#		define TIKI_LIBRARY_METHOD	extern __declspec( dllimport )
#	endif
#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )
#	if TIKI_ENABLED( TIKI_BUILD_LIBRARY )
#		define TIKI_LIBRARY_METHOD	extern
#	else
#		define TIKI_LIBRARY_METHOD	extern
#	endif
#else
#	error "Platform not supported"
#endif

#endif // __TIKI_LIBRARYBASE_HPP_INCLUDED__
