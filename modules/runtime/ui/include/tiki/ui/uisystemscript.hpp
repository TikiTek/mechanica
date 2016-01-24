#pragma once
#ifndef TIKI_UISYSTEMSCRIPT_HPP_INCLUDED
#define TIKI_UISYSTEMSCRIPT_HPP_INCLUDED

#include "tiki/script/scriptclass.hpp"

namespace tiki
{
	class UiSystemScript
	{
		TIKI_NONCOPYABLE_CLASS( UiSystemScript );

	public:

		bool		create( ScriptContext& scriptContext );
		void		dispose();

	private:


	};
}

#endif // TIKI_UISYSTEMSCRIPT_HPP_INCLUDED
