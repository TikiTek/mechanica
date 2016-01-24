#include "tiki/script/scriptcontext.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/string.hpp"

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
		lua_close( m_pState );
		m_pState = nullptr;
	}

	void ScriptContext::addExtension( ScriptExtensions extension )
	{
		switch( extension )
		{
		case tiki::ScriptExtensions_Base:
			luaopen_base( m_pState );
			break;

		case tiki::ScriptExtensions_Objects:
			luaopen_table( m_pState );
			break;

		case tiki::ScriptExtensions_IO:
			luaopen_io( m_pState );
			break;

		case tiki::ScriptExtensions_String:
			luaopen_string( m_pState );
			break;

		case tiki::ScriptExtensions_Math:
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

	bool ScriptContext::executeScript( const char* pCode )
	{	
		const int error = luaL_loadstring( m_pState, pCode );
		if( error != LUA_OK )
		{
			//LUA_ERRRUN: a runtime error.
			//LUA_ERRMEM : memory allocation error.For such errors, Lua does not call the message handler.
			//LUA_ERRERR : error while running the message handler.
			//LUA_ERRGCMM : e
			TIKI_TRACE_ERROR( "[script] Unexpected error in Script.\n" );

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
		else if( pPointer )
		{
			return TIKI_MEMORY_ALLOC( nsize );
		}
		else
		{
			void* pNewData = TIKI_MEMORY_ALLOC( nsize );
			memory::copy( pNewData, pPointer, osize );

			TIKI_MEMORY_FREE( pPointer );

			return pNewData;
		}
	}
}