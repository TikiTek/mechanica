#include "tiki/generic_data_editor/generic_data_editor.hpp"

#include "tiki/editor_interface/editor_interface.hpp"
#include "tiki/editor_interface/editable_file.hpp"

//#include "generic_data_editor_widget.hpp"

namespace tiki
{
	GenericDataEditor::GenericDataEditor( EditorInterface* pInterface )
		: FileEditor( pInterface, "generic-data-editor/browser-file-genericdata.png", "Generic Data", "generic_data" )
	{
		m_typeCollection.create();
	}

	GenericDataEditor::~GenericDataEditor()
	{
		m_typeCollection.dispose();
	}

	EditableFile* GenericDataEditor::openFile( const Path& fileName )
	{
		//GenericDataEditorWidget* pWidget = new GenericDataEditorWidget( m_typeCollection );
		//if( !pWidget->openFile( pFile ) )
		//{
		//	delete pWidget;
		//	return nullptr;
		//}
		//
		//return pWidget;
		return nullptr;
	}

	bool GenericDataEditor::saveEditable( Editable* pEditable )
	{
		//GenericDataEditorWidget* pWidget = (GenericDataEditorWidget*)pEditable->getEditWidget();
		//return pWidget->saveFile();
		return false;
	}

	void GenericDataEditor::closeEditable( Editable* pEditable )
	{
		//GenericDataEditorWidget* pWidget = (GenericDataEditorWidget*)pEditable->getEditWidget();
		//pWidget->closeFile();
		//delete pWidget;
	}
}
