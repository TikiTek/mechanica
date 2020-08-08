#pragma once

#include "tiki/base/delegate.hpp"
#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/flag_mask.hpp"
#include "tiki/base/user_data.hpp"
#include "tiki/container/dynamic_queue.hpp"
#include "tiki/tool_application/tool_image.hpp"

namespace tiki
{
	enum class ToolMessageBoxIcon : uint8
	{
		None,
		Information,
		Question,
		Warning,
		Error
	};

	enum class ToolMessageBoxButton : uint8
	{
		Ok,
		Yes,
		No,
		Abort,
		Retry,
		Ignore,
		Cancel,

		Count
	};
	typedef FlagMask8< ToolMessageBoxButton > ToolMessageBoxButtonFlagMask;

	static const ToolMessageBoxButtonFlagMask ToolMessageBoxButtons_Ok					= ToolMessageBoxButtonFlagMask( ToolMessageBoxButton::Ok );
	static const ToolMessageBoxButtonFlagMask ToolMessageBoxButtons_OkCancel			= ToolMessageBoxButtonFlagMask( ToolMessageBoxButton::Ok ) | ToolMessageBoxButtonFlagMask( ToolMessageBoxButton::Cancel );
	static const ToolMessageBoxButtonFlagMask ToolMessageBoxButtons_YesNo				= ToolMessageBoxButtonFlagMask( ToolMessageBoxButton::Yes ) | ToolMessageBoxButtonFlagMask( ToolMessageBoxButton::No );
	static const ToolMessageBoxButtonFlagMask ToolMessageBoxButtons_YesNoCancel			= ToolMessageBoxButtonFlagMask( ToolMessageBoxButton::Yes ) | ToolMessageBoxButtonFlagMask( ToolMessageBoxButton::No ) | ToolMessageBoxButtonFlagMask( ToolMessageBoxButton::Cancel );
	static const ToolMessageBoxButtonFlagMask ToolMessageBoxButtons_AbortRetryIgnore	= ToolMessageBoxButtonFlagMask( ToolMessageBoxButton::Abort ) | ToolMessageBoxButtonFlagMask( ToolMessageBoxButton::Retry ) | ToolMessageBoxButtonFlagMask( ToolMessageBoxButton::Ignore );
	static const ToolMessageBoxButtonFlagMask ToolMessageBoxButtons_RetryCancel			= ToolMessageBoxButtonFlagMask( ToolMessageBoxButton::Retry ) | ToolMessageBoxButtonFlagMask( ToolMessageBoxButton::Cancel );

	typedef Delegate< void, ToolMessageBoxButton, UserData > ToolMessageBoxCallbackDelegate;

	class ToolMessageBox : NonCopyable
	{
	public:

					ToolMessageBox();

		void		open( const DynamicString& title, const DynamicString& text, ToolMessageBoxButtonFlagMask buttons = ToolMessageBoxButtons_Ok, ToolMessageBoxIcon icon = ToolMessageBoxIcon::None, ToolMessageBoxCallbackDelegate callback = ToolMessageBoxCallbackDelegate(), UserData userData = UserData() );

		void		doUi();

	private:

		struct Message
		{
			DynamicString					title;
			DynamicString					text;
			ToolMessageBoxIcon				icon;
			ToolMessageBoxButtonFlagMask	buttons;
			ToolMessageBoxCallbackDelegate	callback;
			UserData						userData;
		};

		ToolImage							m_informationIcon;
		ToolImage							m_questionIcon;
		ToolImage							m_warningIcon;
		ToolImage							m_errorIcon;

		DynamicQueue< Message >				m_messages;
	};
}