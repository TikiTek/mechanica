#pragma once
#ifndef TIKI_SCRIPTCONTEXT_INL_INCLUDED
#define TIKI_SCRIPTCONTEXT_INL_INCLUDED

namespace tiki
{
	template< ScriptFunc TFunc >
	int ScriptContext::scriptWrapperFunction( lua_State* pState )
	{
		ScriptCall context;
		if( context.create( pState, true ) )
		{
			TFunc( context );
		}
		else
		{
			TIKI_TRACE_ERROR( "[script] Unable to call C function. Context can't be created.\n" );
		}

		const int returnValueCount = context.getReturnValueCount();
		context.dispose();

		return returnValueCount;
	}
}

#endif // TIKI_SCRIPTCONTEXT_INL_INCLUDED
