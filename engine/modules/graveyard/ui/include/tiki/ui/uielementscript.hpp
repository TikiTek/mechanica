#pragma once
#ifndef TIKI_UIELEMENTSCRIPT_HPP_INCLUDED
#define TIKI_UIELEMENTSCRIPT_HPP_INCLUDED

#include "tiki/script/scriptclass.hpp"

namespace tiki
{
	class ScriptCall;
	class ScriptContext;
	class UiElement;
	class UiSystem;
	class UiSystemScript;

	class UiElementScript : public ScriptClass
	{
		TIKI_NONCOPYABLE_CLASS( UiElementScript );
		friend class UiSystemScript;

	public:

						UiElementScript();
						~UiElementScript();

		bool			create( ScriptContext& scriptContext, UiSystem& uiSystem );
		void			dispose();

	private: // friend

		ScriptValue		createScriptElement( const ScriptValue& parentValue );
		void			disposeScriptElement( const ScriptValue& elementValue );

	private:

		UiSystem*		m_pUiSystem;

		static void		setPosition( const ScriptCall& call );
		static void		setWidth( const ScriptCall& call );
		static void		setHeight( const ScriptCall& call );

	};
}

#endif // TIKI_UIELEMENTSCRIPT_HPP_INCLUDED
