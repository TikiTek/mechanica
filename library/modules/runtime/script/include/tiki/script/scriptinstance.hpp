#pragma once
#ifndef TIKI_SCRIPT_HPP_INCLUDED
#define TIKI_SCRIPT_HPP_INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	class ScriptContext;

	class ScriptInstance
	{
		friend class ScriptContext;

	public:

						ScriptInstance();
						ScriptInstance( ScriptContext& context );
						ScriptInstance( const ScriptInstance& copy );
						~ScriptInstance();

		bool			isValid() const;
		
		ScriptInstance&	operator=( const ScriptInstance& copy );

	private:

		ScriptContext*	m_pContext;
		int				m_scriptObject;

		void			createFromStack();

	};
}

#endif // TIKI_SCRIPT_HPP_INCLUDED
