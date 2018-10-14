#include "tiki/editor_interface/base_editor.hpp"

namespace tiki
{
	BaseEditor::BaseEditor( EditorInterface* pInterface, const char* pIconPath )
		: m_pInterface( pInterface )
	{
		m_icon.createFromFile( pIconPath );
	}
}