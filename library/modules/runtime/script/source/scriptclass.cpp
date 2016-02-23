#include "tiki/script/scriptclass.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/container/fixedsizedarray.hpp"
#include "tiki/script/scriptcontext.hpp"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace tiki
{
	ScriptClass::ScriptClass()
	{
		m_pContext	= nullptr;
		m_pName		= nullptr;
	}

	ScriptClass::~ScriptClass()
	{
		TIKI_ASSERT( m_pContext == nullptr );
	}

	bool ScriptClass::create( ScriptContext& context, const char* pName, const ScriptMethod* pMethods, uint methodCount, ScriptWrapperFunc pOnCollectMethod /* = nullptr */ )
	{
		m_pContext	= &context;
		m_pName		= pName;

		lua_State* pState = context.m_pState;

		lua_newtable( pState );
		int methodtable = lua_gettop( pState );

		luaL_newmetatable( pState, pName );
		int metatable = lua_gettop( pState );

		lua_pushliteral( pState, "__metatable" );
		lua_pushvalue( pState, methodtable );
		lua_settable( pState, metatable );  // hide metatable from Lua getmetatable()

		lua_pushliteral( pState, "__index" );
		lua_pushvalue( pState, methodtable );
		lua_settable( pState, metatable );

		if( pOnCollectMethod )
		{
			lua_pushliteral( pState, "__gc" );
			lua_pushcfunction( pState, pOnCollectMethod );
			lua_settable( pState, metatable );
		}

		lua_pop( pState, 1 );  // drop metatable
		
		FixedSizedArray< luaL_Reg, 32u > functions;
		for( uint i = 0u; i < methodCount; ++i )
		{
			luaL_Reg& function = functions.push();
			function.name	= pMethods[ i ].pMethodName;
			function.func	= pMethods[ i ].pMethodFunc;
		}

		luaL_Reg& lastFunction = functions.push();
		lastFunction.name	= nullptr;
		lastFunction.func	= nullptr;
		
		luaL_setfuncs( pState, functions.getBegin(), 0 );
		lua_pop( pState, 1 );  // drop methodtable

		//lua_register( pState, pName, create_account );

		return true;
	}

	void ScriptClass::dispose()
	{
		m_pContext = nullptr;
	}

	ScriptValue ScriptClass::registerInstance( const void* pInstanceData, uint instanceDataSize )
	{
		TIKI_ASSERT( m_pContext != nullptr );
		lua_State* pState = m_pContext->m_pState;

		void* pScriptData = lua_newuserdata( pState, instanceDataSize );
		memory::copy( pScriptData, pInstanceData, instanceDataSize );

		luaL_getmetatable( pState, m_pName );
		lua_setmetatable( pState, -2 );

		ScriptValue value( *m_pContext );
		value.createObjectFromStack();

		return value;
	}

	void ScriptClass::unregisterInstance( const ScriptValue& instance )
	{

	}
}
