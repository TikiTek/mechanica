#include "tiki/editor_interface/editable.hpp"

namespace tiki
{
	Editable::Editable( const DynamicString& title, BaseEditor& editor )
		: m_isDirty( false )
		, m_title( title )
		, m_editor( editor )
	{
	}
}
