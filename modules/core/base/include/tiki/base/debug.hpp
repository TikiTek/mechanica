#ifndef TIKI_DEBUG_HPP
#define TIKI_DEBUG_HPP

#include "tiki/base/types.hpp"

namespace tiki
{
	namespace debug
	{
		enum TraceLevel
		{
			TraceLevel_None		= 0u,
			TraceLevel_Debug,
			TraceLevel_Info,
			TraceLevel_Warning,
			TraceLevel_Error
		};

		typedef void(*TraceCallback)(cstring, TraceLevel);

		bool	isDebuggerActive();

		void	trace( cstring format, ... );
		void	traceInfo( cstring format, ... );
		void	traceWarning( cstring format, ... );
		void	traceError( cstring format, ... );
		void	traceDebug( cstring format, ... );
		void	setTraceCallback( TraceCallback pCallback );

		void	breakPoint();

		void	breakOnAlloc( int number );
		void	dumpMemoryStats();
	}
}

#endif // TIKI_DEBUG_HPP
