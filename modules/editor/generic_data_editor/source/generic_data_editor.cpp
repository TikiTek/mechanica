#include "tiki/generic_data_editor/generic_data_editor.hpp"

#include "tiki/editor_interface/ieditorinterface.hpp"
#include "tiki/editor_interface/ifile.hpp"

#include "generic_data_editor_widget.hpp"

namespace tiki
{
	GenericDataEditor::GenericDataEditor( IEditorInterface* pInterface )
		: m_pInterface( pInterface )
		, m_icon( ":/genericdata-editor/browser-file-genericdata.png" )
	{
		m_typeCollection.create();
	}

	GenericDataEditor::~GenericDataEditor()
	{
		m_typeCollection.dispose();
	}

	QWidget* GenericDataEditor::openFile( IFile* pFile )
	{
		GenericDataEditorWidget* pWidget = new GenericDataEditorWidget( m_typeCollection );
		if( !pWidget->openFile( pFile ) )
		{
			delete pWidget;
			return nullptr;
		}

		return pWidget;
	}

	bool GenericDataEditor::saveEditable( IEditable* pEditable )
	{
		GenericDataEditorWidget* pWidget = (GenericDataEditorWidget*)pEditable->getEditWidget();
		return pWidget->saveFile();
	}

	void GenericDataEditor::closeEditable( IEditable* pEditable )
	{
		GenericDataEditorWidget* pWidget = (GenericDataEditorWidget*)pEditable->getEditWidget();
		pWidget->closeFile();
		delete pWidget;
	}

	QString GenericDataEditor::getFileTypeName() const
	{
		return "Generic Data";
	}

	QString GenericDataEditor::getFileExtension() const
	{
		return "genericobjects";
	}

	QIcon GenericDataEditor::getEditableIcon() const
	{
		return m_icon;
	}
}
