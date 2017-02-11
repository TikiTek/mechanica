#include "generic_data_tag_editor.hpp"

namespace tiki
{
	GenericDataTagEditor::GenericDataTagEditor( QWindow* pParent, GenericDataTag* pTag )
		: QWindow( pParent )
		, m_pTag( pTag )
	{
	}

	GenericDataTagEditor::~GenericDataTagEditor()
	{

	}
}
