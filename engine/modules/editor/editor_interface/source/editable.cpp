#include "tiki/editor_interface/editable.hpp"

namespace tiki
{
	Editable::Editable( const DynamicString& title, BaseEditor* pEditor )
		: m_isDirty( false )
		, m_title( title )
		, m_pEditor( pEditor )
	{
	}
}
