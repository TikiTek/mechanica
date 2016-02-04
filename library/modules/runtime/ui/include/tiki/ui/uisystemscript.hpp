#pragma once
#ifndef TIKI_UISYSTEMSCRIPT_HPP_INCLUDED
#define TIKI_UISYSTEMSCRIPT_HPP_INCLUDED

#include "tiki/script/scriptclass.hpp"

namespace tiki
{
	class ScriptCall;
	class UiSystem;

	class UiSystemScript : public ScriptClass
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( UiSystemScript );

	public:

		bool			create( ScriptContext& scriptContext, UiSystem& uiSystem );
		void			dispose();

	private:

		static void		addElement( ScriptCall& context );

	};
}

#endif // TIKI_UISYSTEMSCRIPT_HPP_INCLUDED
