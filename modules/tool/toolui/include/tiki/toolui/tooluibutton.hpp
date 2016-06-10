#pragma once
#ifndef TIKI_TOOLUIBUTTON_HPP_INCLUDED
#define TIKI_TOOLUIBUTTON_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/ui/uitypes.hpp"

namespace tiki
{
	class UiElement;
	class UiSystem;

	class ToolUiButton
	{
		TIKI_NONCOPYABLE_CLASS( ToolUiButton );

	public:

		static UiElement*	createButton( UiSystem& uiSystem );
		static void			disposeButton( UiSystem& uiSystem, UiElement* pElement );

		static void			setText( UiElement* pElement, const char* pText );

	private:

		static void			onMouseIn( UiElement* pElement, const UiEvent& eventData );
		static void			onMouseOut( UiElement* pElement, const UiEvent& eventData );
		static void			onMouseDown( UiElement* pElement, const UiEvent& eventData );
		static void			onMouseUp( UiElement* pElement, const UiEvent& eventData );

	};
}

#endif // TIKI_TOOLUIBUTTON_HPP_INCLUDED
