#include "tiki/script/scriptcall.hpp"

#include "tiki/base/assert.hpp"

extern "C" {
#include "lua.h"
}

namespace tiki
{
	ScriptCall::ScriptCall()
	{
		m_pState			= nullptr;
		m_pInstance			= nullptr;
		m_returnValueCount	= 0;
	}

	ScriptCall::~ScriptCall()
	{
		TIKI_ASSERT( m_pState			== nullptr );
		TIKI_ASSERT( m_pInstance		== nullptr );
		TIKI_ASSERT( m_returnValueCount	== 0 );
	}

	bool ScriptCall::create( lua_State* pState, bool hasInstance )
	{
		m_pState = pState;

		if( hasInstance )
		{
			void* pUserData = lua_touserdata( pState, -1 );
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
		m_pState			= nullptr;
		m_pInstance			= nullptr;
		m_returnValueCount	= 0;
	}

	void* ScriptCall::getInstance() const
	{
		return m_pInstance;
	}

	int ScriptCall::getReturnValueCount() const
	{
		return m_returnValueCount;
	}
}
