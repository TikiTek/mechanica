#include "tiki/entity_template_editor/entity_template_editor.hpp"

#include "tiki/generic_data_editor/generic_data_editor.hpp"

namespace tiki
{
	EntityTemplateEditor::EntityTemplateEditor( EditorInterface& editor, GenericDataEditor& genericDataEditor )
		: BaseEditor( editor )
		, m_genericDataEditor( genericDataEditor )
		, m_entityTemplateVuew( genericDataEditor.getTypeCollection() )
		, m_transformView( genericDataEditor.getTypeCollection() )
	{
		m_genericDataEditor.registerView( m_entityTemplateVuew );
		m_genericDataEditor.registerView( m_transformView );
	}

	EntityTemplateEditor::~EntityTemplateEditor()
	{
	}
}