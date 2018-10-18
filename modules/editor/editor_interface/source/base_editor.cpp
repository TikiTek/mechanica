#include "tiki/editor_interface/base_editor.hpp"

namespace tiki
{
	BaseEditor::BaseEditor( EditorInterface* pInterface, ConstMemoryBlock iconData )
		: m_pInterface( pInterface )
	{
		TIKI_VERIFY( m_icon.createFromMemory( iconData ) );
	}
}