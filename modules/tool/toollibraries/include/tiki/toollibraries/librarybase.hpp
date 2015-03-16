#pragma once
#ifndef __TIKI_LIBRARYBASE_HPP_INCLUDED__
#define __TIKI_LIBRARYBASE_HPP_INCLUDED__

#include "tiki/base/types.hpp"

#if TIKI_ENABLED( TIKI_BUILD_LIBRARY )
#	define TIKI_LIBRARY_METHOD	__declspec( dllexport )
#else
#	define TIKI_LIBRARY_METHOD	extern __declspec( dllimport )
#endif

#endif // __TIKI_LIBRARYBASE_HPP_INCLUDED__
