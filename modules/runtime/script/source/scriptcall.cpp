#include "tiki/script/scriptcall.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/script/scriptcontext.hpp"

extern "C" {
#include "lua.h"
}

namespace tiki
{
	ScriptCall::ScriptCall()
	{
		m_pContext			= nullptr;
		m_pInstance			= nullptr;
	}

	ScriptCall::~ScriptCall()
	{
		TIKI_ASSERT( m_pContext == nullptr );
		TIKI_ASSERT( m_pInstance	== nullptr );
	}

	bool ScriptCall::create( ScriptContext& context, bool hasInstance )
	{
		m_pContext = &context;

		if( hasInstance )
		{
			void* pUserData = lua_touserdata( m_pContext->m_pState, 1 );
			if( pUserData == nullptr )
			{
				dispose();
				return false;
			}

			m_pInstance = *(void**)pUserData;
			return m_pInstance != nullptr;
		}

		return true;
	}

	void ScriptCall::dispose()
	{
		m_pContext	= nullptr;
		m_pInstance	= nullptr;
		m_returnValue.dispose();
	}

	void* ScriptCall::getInstance() const
	{
		return m_pInstance;
	}

	ScriptValue ScriptCall::getArgument( uint index ) const
	{
		const int stackIndex = -(int)(index + 1u);

		ScriptValue value( *m_pContext );
		value.createFromStack( stackIndex );

		return value;
	}

	void ScriptCall::setReturnValue( const ScriptValue& value ) const
	{
		m_returnValue = value;
	}

	int ScriptCall::pushReturnValue()
	{
		if( m_returnValue.isValid() )
		{
			m_returnValue.pushValue();
			return 1;
		}

		return 0;
	}
}
