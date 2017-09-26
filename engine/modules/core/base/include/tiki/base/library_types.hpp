#pragma once
#ifndef TIKI_LIBRARY_TYPES_HPP_INCLUDED
#define TIKI_LIBRARY_TYPES_HPP_INCLUDED

#include "tiki/base/types.hpp"

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
#	define TIKI_LIBRARY_EXPORT_METHOD	__declspec( dllexport )
#	define TIKI_LIBRARY_IMPORT_METHOD	extern __declspec( dllimport )
#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )
#	define TIKI_LIBRARY_EXPORT_METHOD	extern
#	define TIKI_LIBRARY_IMPORT_METHOD	extern
#else
#	error "Platform not supported"
#endif

#endif // TIKI_LIBRARY_TYPES_HPP_INCLUDED
