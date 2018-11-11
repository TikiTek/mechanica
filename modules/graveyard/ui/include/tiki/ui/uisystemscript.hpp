#pragma once
#ifndef TIKI_UISYSTEMSCRIPT_HPP_INCLUDED
#define TIKI_UISYSTEMSCRIPT_HPP_INCLUDED

#include "tiki/script/scriptclass.hpp"

namespace tiki
{
	class ScriptCall;
	class UiElementScript;
	class UiSystem;

	class UiSystemScript : public ScriptClass
	{
		TIKI_NONCOPYABLE_CLASS( UiSystemScript );

	public:

							UiSystemScript();
							~UiSystemScript();

		bool				create( ScriptContext& scriptContext, UiSystem& uiSystem, UiElementScript& elementScript );
		void				dispose();

	private:

		UiSystem*			m_pUiSystem;
		UiElementScript*	m_pUiElementScript;

		static void			addElement( const ScriptCall& call );
		static void			removeElement( const ScriptCall& call );

	};
}

#endif // TIKI_UISYSTEMSCRIPT_HPP_INCLUDED
