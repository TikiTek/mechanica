#pragma once
#ifndef TIKI_ASSERT_HPP
#define TIKI_ASSERT_HPP

#include "tiki/base/debug.hpp"

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
#	define TIKI_USE_ASSERT	TIKI_ON
#	define TIKI_USE_BREAK	TIKI_ON
#else
#	define TIKI_USE_ASSERT	TIKI_OFF
#	define TIKI_USE_BREAK	TIKI_OFF
#endif

#if TIKI_ENABLED( TIKI_USE_ASSERT )
#	define TIKI_ASSERT( expr ) if ( !(expr) ) ::tiki::debug::breakPoint()
#	define TIKI_VERIFY( expr ) if ( !(expr) ) ::tiki::debug::breakPoint()
#	define TIKI_VERIFY0( expr ) if ( (expr) != 0 ) ::tiki::debug::breakPoint()
#else
#	define TIKI_ASSERT( expr )
#	define TIKI_VERIFY( expr ) expr
#	define TIKI_VERIFY0( expr ) expr
#endif

#if TIKI_ENABLED( TIKI_USE_BREAK )
#	define TIKI_BREAK( message, ... ) ::tiki::debug::traceError( message, ## __VA_ARGS__ ); ::tiki::debug::breakPoint()
#else
#	define TIKI_BREAK( message, ... ) for(;;)
#endif

#if TIKI_DISABLED( TIKI_BUILD_MASTER )

#	define TIKI_TRACE_INFO		::tiki::debug::traceInfo
#	define TIKI_TRACE_WARNING	::tiki::debug::traceWarning
#	define TIKI_TRACE_ERROR		::tiki::debug::traceError

#	define TIKI_NOT_IMPLEMENTED	::tiki::debug::trace( "%s(%u): not implemented", __FILE__, __LINE__ )

#else

#	define TIKI_TRACE_INFO( format, ... )
#	define TIKI_TRACE_WARNING( format, ... )
#	define TIKI_TRACE_ERROR( format, ... )

#	define TIKI_NOT_IMPLEMENTED	TIKI_NOT_IMPLEMENTED_BARRIER

#endif

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )

#	define TIKI_TRACE_DEBUG		::tiki::debug::traceDebug

#else

#	define TIKI_TRACE_DEBUG( format, ... )

#endif

#define TIKI_COMPILETIME_ASSERT( expr ) typedef struct { int x[ ( ( expr ? 1 : -1 ) ) ]; } TIKI_CONCAT( __tiki__assert__, __COUNTER__ )

#endif // TIKI_ASSERT_HPP
