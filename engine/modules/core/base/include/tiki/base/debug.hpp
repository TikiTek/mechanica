#ifndef TIKI_DEBUG_HPP
#define TIKI_DEBUG_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/user_data.hpp"

namespace tiki
{
	enum TraceLevel
	{
		TraceLevel_None		= 0u,
		TraceLevel_Debug,
		TraceLevel_Info,
		TraceLevel_Warning,
		TraceLevel_Error
	};

	typedef void( *TraceCallback )( const char*, TraceLevel, UserData );

	namespace debug
	{
		void	trace( const char* pFormat, ... );
		void	traceInfo( const char* pFormat, ... );
		void	traceWarning( const char* pFormat, ... );
		void	traceError( const char* pFormat, ... );
		void	traceDebug( const char* pFormat, ... );
		void	setTraceCallback( TraceCallback pCallback, UserData userData = UserData() );

		bool	isDebuggerActive();
		void	breakPoint();

		void	breakOnAlloc( int number );
		void	dumpMemoryStats();

		void	nativeTrace( const char* pText );
	}
}

#endif // TIKI_DEBUG_HPP
