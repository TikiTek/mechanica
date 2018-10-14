#pragma once

#ifndef TIKI_ON
#	define TIKI_ON 2
#endif

#ifndef TIKI_OFF
#	define TIKI_OFF 1
#endif

#define TIKI_ENABLED( value ) ( ( 0 + value ) == 2 )
#define TIKI_DISABLED( value ) ( ( 0 + value ) != 2 )

#ifndef TIKI_BUILD_DEBUG
#	define TIKI_BUILD_DEBUG TIKI_OFF
#endif

#ifndef TIKI_BUILD_RELEASE
#	define TIKI_BUILD_RELEASE TIKI_OFF
#endif

#ifndef TIKI_BUILD_MASTER
#	define TIKI_BUILD_MASTER TIKI_OFF
#endif

#ifndef TIKI_PLATFORM_WIN
#	define TIKI_PLATFORM_WIN TIKI_OFF
#endif

#ifndef TIKI_PLATFORM_LINUX
#	define TIKI_PLATFORM_LINUX TIKI_OFF
#endif

#ifndef TIKI_BUILD_32BIT
#	define TIKI_BUILD_32BIT TIKI_OFF
#endif

#ifndef TIKI_BUILD_64BIT
#	define TIKI_BUILD_64BIT TIKI_OFF
#endif

#ifndef TIKI_BUILD_MSVC
#	define TIKI_BUILD_MSVC TIKI_OFF
#endif

#ifndef TIKI_BUILD_GCC
#	define TIKI_BUILD_GCC TIKI_OFF
#endif

#ifndef TIKI_BUILD_CLANG
#	define TIKI_BUILD_CLANG TIKI_OFF
#endif
