#pragma once
#ifndef TIKI_SCRIPTOBJECT_HPP_INCLUDED
#define TIKI_SCRIPTOBJECT_HPP_INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	class ScriptCallContext;
	class ScriptContext;

	typedef void( *ScriptMemberFunc )( void* pInstance, ScriptCallContext& context );

	struct ScriptMethod
	{
		const char*			pMethodName;
		ScriptMemberFunc*	pMethodFunc;
	};

	class ScriptClass
	{
		TIKI_NONCOPYABLE_CLASS( ScriptClass );

	protected:

		bool	create( ScriptContext& context, const char* pName, const ScriptMethod* pMethods, uint methodCount );
		void	dispose();

		void	registerInstance( void* pInstance );
		void	unregisterInstance( void* pInstance );

	private:

		int		m_methodTable;
		int		m_metaTable;

	};
}

#endif // TIKI_SCRIPTOBJECT_HPP_INCLUDED
