#pragma once
#ifndef TIKI_SCRIPTCONTEXT_INL_INCLUDED
#define TIKI_SCRIPTCONTEXT_INL_INCLUDED

namespace tiki
{
	template< ScriptFunc TFunc >
	int ScriptContext::scriptWrapperFunction( lua_State* pState )
	{
		ScriptContext context;
		context.m_pState = pState;

		context.stackDump();

		ScriptCall call;
		if( call.create( context, true ) )
		{
			TFunc( call );
		}
		else
		{
			context.stackDump();
			TIKI_TRACE_ERROR( "[script] Unable to call C function. Context can't be created.\n" );
		}

		const int returnValueCount = call.pushReturnValue();
		call.dispose();

		context.m_pState = nullptr;

		return returnValueCount;
	}
}

#endif // TIKI_SCRIPTCONTEXT_INL_INCLUDED
