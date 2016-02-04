#include "tiki/script/scriptinstance.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/script/scriptcontext.hpp"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

namespace tiki
{
	ScriptInstance::ScriptInstance()
	{
		m_pContext		= nullptr;
		m_scriptObject	= LUA_NOREF;
	}

	ScriptInstance::ScriptInstance( ScriptContext& context )
	{
		m_pContext = &context;
		createFromStack();
	}

	ScriptInstance::ScriptInstance( const ScriptInstance& copy )
	{
		*this = copy;
	}

	ScriptInstance::~ScriptInstance()
	{
		if( m_scriptObject != LUA_NOREF )
		{
			TIKI_ASSERT( m_pContext != nullptr );

			luaL_unref( m_pContext->m_pState, m_scriptObject, LUA_REGISTRYINDEX );
			m_scriptObject = LUA_NOREF;
		}

		m_pContext = nullptr;
	}

	bool ScriptInstance::isValid() const
	{
		return m_pContext != nullptr && m_scriptObject != LUA_NOREF;
	}

	void ScriptInstance::createFromStack()
	{
		TIKI_ASSERT( m_pContext != nullptr );

		m_scriptObject	= luaL_ref( m_pContext->m_pState, LUA_REGISTRYINDEX );
		if( m_scriptObject == LUA_REFNIL )
		{
			m_pContext		= nullptr;
			m_scriptObject	= LUA_NOREF;
		}
		else
		{
			TIKI_TRACE_INFO( "[script] New reference: %i\n", m_scriptObject );
		}
	}

	ScriptInstance& ScriptInstance::operator=( const ScriptInstance& copy )
	{
		if( copy.isValid() )
		{
			m_pContext = copy.m_pContext;

			lua_rawgeti( m_pContext->m_pState, LUA_REGISTRYINDEX, copy.m_scriptObject );
			createFromStack();
		}
		else
		{
			m_pContext		= nullptr;
			m_scriptObject	= LUA_NOREF;
		}

		return *this;
	}
}