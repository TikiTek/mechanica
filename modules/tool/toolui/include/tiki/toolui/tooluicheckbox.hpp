#pragma once
#ifndef TIKI_TOOLUICHECKBOX_HPP_INCLUDED
#define TIKI_TOOLUICHECKBOX_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/ui/uitypes.hpp"

namespace tiki
{
	class UiElement;
	class UiSystem;

	class ToolUiCheckBox
	{
		TIKI_NONCOPYABLE_CLASS( ToolUiCheckBox );

	public:

		static UiElement*	createCheckBox( UiSystem& uiSystem );
		static void			disposeCheckBox( UiSystem& uiSystem, UiElement* pElement );

		static bool			isChecked( UiElement* pElement );
		static void			setChecked( UiElement* pElement, bool checked );

		static void			setText( UiElement* pElement, const char* pText );

	private:

		static void			onMouseIn( UiElement* pElement, const UiEvent& eventData );
		static void			onMouseOut( UiElement* pElement, const UiEvent& eventData );
		static void			onMouseDown( UiElement* pElement, const UiEvent& eventData );
		static void			onMouseUp( UiElement* pElement, const UiEvent& eventData );

	};
}

#endif // TIKI_TOOLUICHECKBOX_HPP_INCLUDED
