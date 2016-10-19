#include "tiki/script/scriptcontext.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/string.hpp"
#include "tiki/script/scriptvalue.hpp"

extern "C"
{
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

namespace tiki
{
	ScriptContext::ScriptContext()
	{
		m_pState = nullptr;
	}

	ScriptContext::~ScriptContext()
	{
		TIKI_ASSERT( m_pState == nullptr );
	}

	bool ScriptContext::create()
	{
		m_pState = lua_newstate( &ScriptContext::allocateMemory, nullptr );

		return m_pState != nullptr;
	}

	void ScriptContext::dispose()
	{
		if( m_pState != nullptr )
		{
			lua_close( m_pState );
			m_pState = nullptr;
		}
	}

	void ScriptContext::addExtension( ScriptExtensions extension )
	{
		switch( extension )
		{
		case ScriptExtensions_Base:
			luaopen_base( m_pState );
			break;

		case ScriptExtensions_Objects:
			luaopen_table( m_pState );
			break;

		case ScriptExtensions_IO:
			luaopen_io( m_pState );
			break;

		case ScriptExtensions_String:
			luaopen_string( m_pState );
			break;

		case ScriptExtensions_Math:
			luaopen_math( m_pState );
			break;

		default:
			break;
		}
	}

	void ScriptContext::registerFunction( ScriptFunc* pFunc )
	{

	}

	void ScriptContext::unregisterFunction( ScriptFunc* pFunc )
	{

	}

	void ScriptContext::setGlobalValue( const char* pVariableName, const ScriptValue& value )
	{
		value.pushValue();
		lua_setglobal( m_pState, pVariableName );
	}

	bool ScriptContext::executeScript( const char* pCode )
	{	
		const int error = luaL_dostring( m_pState, pCode );
		if( error != LUA_OK )
		{
			const char* pErrorMessage = lua_tostring( m_pState, -1 );
			TIKI_TRACE_ERROR( "[script] Unexpected error in Script: %s\n", pErrorMessage );

			return false;
		}

		return true;
	}

	void* ScriptContext::allocateMemory( void* pUserData, void* pPointer, size_t osize, size_t nsize )
	{
		if( nsize == 0u )
		{
			TIKI_MEMORY_FREE( pPointer );
			return nullptr;
		}
		else if( pPointer == nullptr )
		{
			return TIKI_ALLOC( nsize );
		}
		else
		{
			void* pNewData = TIKI_ALLOC( nsize );
			memory::copy( pNewData, pPointer, TIKI_MIN( nsize, osize ) );

			TIKI_MEMORY_FREE( pPointer );

			return pNewData;
		}
	}

	void ScriptContext::stackDump()
	{
		debug::trace( "stackDump: " );

		int i;
		int top = lua_gettop( m_pState );
		for( i = 1; i <= top; i++ ) {  /* repeat for each level */
			int t = lua_type( m_pState, i );
			switch( t ) {

			case LUA_TSTRING:  /* strings */
				debug::trace( "`%s'", lua_tostring( m_pState, i ) );
				break;

			case LUA_TBOOLEAN:  /* booleans */
				debug::trace( lua_toboolean( m_pState, i ) ? "true" : "false" );
				break;

			case LUA_TNUMBER:  /* numbers */
				debug::trace( "%g", lua_tonumber( m_pState, i ) );
				break;

			default:  /* other values */
				debug::trace( "%s", lua_typename( m_pState, t ) );
				break;

			}
			debug::trace( "  " );  /* put a separator */
		}
		debug::trace( "\n" );  /* end the listing */
	}
}