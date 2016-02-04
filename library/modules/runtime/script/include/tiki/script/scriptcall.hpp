#pragma once
#ifndef TIKI_SCRIPTCALL_HPP_INCLUDED
#define TIKI_SCRIPTCALL_HPP_INCLUDED

#include "tiki/base/types.hpp"

struct lua_State;

namespace tiki
{
	class ScriptCall
	{
		TIKI_NONCOPYABLE_CLASS( ScriptCall );

	public:

						ScriptCall();
						~ScriptCall();

		bool			create( lua_State* pState, bool hasInstance );
		void			dispose();

		void*			getInstance() const;

		int				getReturnValueCount() const;

	private:

		lua_State*		m_pState;
		void*			m_pInstance;
		int				m_returnValueCount;

	};
}

#endif // TIKI_SCRIPTCALL_HPP_INCLUDED
