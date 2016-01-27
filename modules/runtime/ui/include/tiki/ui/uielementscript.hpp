#pragma once
#ifndef TIKI_UIELEMENTSCRIPT_HPP_INCLUDED
#define TIKI_UIELEMENTSCRIPT_HPP_INCLUDED

#include "tiki/script/scriptclass.hpp"

namespace tiki
{
	class ScriptCall;
	class UiSystemScript;

	class UiElementScript : public ScriptClass
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( UiElementScript );
		friend class UiSystemScript;

	public:

		bool			create( ScriptContext& scriptContext );
		void			dispose();

	private:

		static void		setPosition( const ScriptCall& call );
		static void		setWidth( const ScriptCall& call );
		static void		setHeight( const ScriptCall& call );

	};
}

#endif // TIKI_UIELEMENTSCRIPT_HPP_INCLUDED
