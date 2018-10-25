#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/flagmask.hpp"
#include "tiki/toolapplication/tool_image.hpp"

namespace tiki
{
	enum ToolMessageBoxIcon : uint8
	{
		ToolMessageBoxIcon_None,
		ToolMessageBoxIcon_Information,
		ToolMessageBoxIcon_Question,
		ToolMessageBoxIcon_Warning,
		ToolMessageBoxIcon_Error,

		ToolMessageBoxIcon_Count
	};

	enum ToolMessageBoxButton : uint8
	{
		ToolMessageBoxButton_Ok,
		ToolMessageBoxButton_Yes,
		ToolMessageBoxButton_No,
		ToolMessageBoxButton_Abort,
		ToolMessageBoxButton_Retry,
		ToolMessageBoxButton_Ignore,
		ToolMessageBoxButton_Cancel,

		ToolMessageBoxButton_Count
	};
	typedef FlagMask8< ToolMessageBoxButton > ToolMessageBoxButtonFlagMask;

	static const ToolMessageBoxButtonFlagMask ToolMessageBoxButtons_Ok					= ToolMessageBoxButtonFlagMask( ToolMessageBoxButton_Ok );
	static const ToolMessageBoxButtonFlagMask ToolMessageBoxButtons_OkCancel			= ToolMessageBoxButtonFlagMask( ToolMessageBoxButton_Ok ) | ToolMessageBoxButtonFlagMask( ToolMessageBoxButton_Cancel );
	static const ToolMessageBoxButtonFlagMask ToolMessageBoxButtons_YesNo				= ToolMessageBoxButtonFlagMask( ToolMessageBoxButton_Yes ) | ToolMessageBoxButtonFlagMask( ToolMessageBoxButton_No );
	static const ToolMessageBoxButtonFlagMask ToolMessageBoxButtons_YesNoCancel			= ToolMessageBoxButtonFlagMask( ToolMessageBoxButton_Yes ) | ToolMessageBoxButtonFlagMask( ToolMessageBoxButton_No ) | ToolMessageBoxButtonFlagMask( ToolMessageBoxButton_Cancel );
	static const ToolMessageBoxButtonFlagMask ToolMessageBoxButtons_AbortRetryIgnore	= ToolMessageBoxButtonFlagMask( ToolMessageBoxButton_Abort ) | ToolMessageBoxButtonFlagMask( ToolMessageBoxButton_Retry ) | ToolMessageBoxButtonFlagMask( ToolMessageBoxButton_Ignore );
	static const ToolMessageBoxButtonFlagMask ToolMessageBoxButtons_RetryCancel			= ToolMessageBoxButtonFlagMask( ToolMessageBoxButton_Retry ) | ToolMessageBoxButtonFlagMask( ToolMessageBoxButton_Cancel );

	class ToolMessageBox
	{
		TIKI_NONCOPYABLE_CLASS( ToolMessageBox );

	public:

					ToolMessageBox();

		void		open( const DynamicString& title, const DynamicString& message, ToolMessageBoxButtonFlagMask buttons = ToolMessageBoxButtons_Ok, ToolMessageBoxIcon icon = ToolMessageBoxIcon_None );

		void		doUi();

	private:

		ToolImage						m_informationIcon;
		ToolImage						m_questionIcon;
		ToolImage						m_warningIcon;
		ToolImage						m_errorIcon;

		DynamicString					m_title;
		DynamicString					m_message;
		ToolMessageBoxIcon				m_icon;
		ToolMessageBoxButtonFlagMask	m_buttons;
	};
}