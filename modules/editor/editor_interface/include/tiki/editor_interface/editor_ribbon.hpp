#pragma  once

#include "tiki/base/dynamic_string.hpp"

namespace tiki
{
	class EditorRibbon
	{
	public:

		virtual ~EditorRibbon() { }

		const DynamicString&	getTitle() const { return m_title; }

		virtual void			doUi() = 0;

	protected:

		DynamicString			m_title;

								EditorRibbon( const DynamicString& title );
	};
}
