#pragma once
#ifndef TIKI_ASSERT_HPP
#define TIKI_ASSERT_HPP

#include "tiki/base/debug.hpp"
#include "stdio.h"

#if TIKI_BUILD_DEBUG || 1
#	define TIKI_USE_ASSERT 1
#	define TIKI_USE_BREAK 1
#else
#	define TIKI_USE_ASSERT 0
#	define TIKI_USE_BREAK 0
#endif

#if TIKI_USE_ASSERT
#	define TIKI_ASSERT(expr) if (!(expr)) ::tiki::debug::breakPoint()
#else
#	define TIKI_ASSERT(expr)
#endif

#if TIKI_USE_BREAK
#	define TIKI_BREAK( message ) ::tiki::debug::traceError( message ); ::tiki::debug::breakPoint()
#	define TIKI_BREAK2( message, ... ) ::tiki::debug::traceError( message, __VA_ARGS__ ); ::tiki::debug::breakPoint()
#else
#	define TIKI_BREAK( message )
#	define TIKI_BREAK2( message, ... )
#endif

#if TIKI_USE_ASSERT
#	define TIKI_VERIFY( expr ) if ( !(expr) ) ::tiki::debug::breakPoint()
#	define TIKI_VERIFY0( expr ) if ( (expr) != 0 ) ::tiki::debug::breakPoint()
#else
#	define TIKI_VERIFY( expr ) expr
#	define TIKI_VERIFY0( expr ) expr
#endif

#define TIKI_TRACE( format, ... ) ::tiki::debug::trace( format, ##__VA_ARGS__ )
#define TIKI_TRACE_INFO( format, ... ) ::tiki::debug::traceInfo( format, ##__VA_ARGS__ )
#define TIKI_TRACE_WARNING( format, ... ) ::tiki::debug::traceWarning( format, ##__VA_ARGS__ )
#define TIKI_TRACE_ERROR( format, ... ) ::tiki::debug::traceError( format, ##__VA_ARGS__ )

#if TIKI_BUILD_DEBUG
#	define TIKI_TRACE_DEBUG( format, ... ) TIKI_TRACE( format, ##__VA_ARGS__ )
#else
#	define TIKI_TRACE_DEBUG( format, ... )
#endif

#define TIKI_COMPILETIME_ASSERT( expr ) typedef struct { int x[ ( ( expr ? 1 : -1 ) ) ]; } __tiki__assert__##__COUNTER__

#endif // TIKI_ASSERT_HPP
