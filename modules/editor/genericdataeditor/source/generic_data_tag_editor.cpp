#include "generic_data_tag_editor.hpp"

namespace tiki
{
	GenericDataTagEditor::GenericDataTagEditor( QWindow* pParent, GenericDataValue* pValue )
		: QWindow( pParent )
		, m_pValue( pValue )
	{
	}

	GenericDataTagEditor::~GenericDataTagEditor()
	{

	}
}
