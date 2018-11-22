#include "tiki/generic_data_editor/generic_data_editor.hpp"

#include "tiki/editor_interface/editable_file.hpp"
#include "tiki/editor_interface/editor_interface.hpp"
#include "tiki/tool_project/package.hpp"
#include "tiki/tool_project/project.hpp"

#include "generic_data_file.hpp"

#include "res_generic_data_editor.hpp"

namespace tiki
{
	GenericDataEditor::GenericDataEditor( EditorInterface& editor, ResourceManager& resourceManager, GraphicsSystem& graphicsSystem )
		: FileEditor( editor, getGenericDataEditorResource( GenericDataEditorResources_BrowserFileGenericData ), "Generic Data", ".generic_data" )
	{
		for( const Package& package : m_editor.getProject().getPackages() )
		{
			m_typeCollection.addPackage( package );
		}

		TIKI_VERIFY( m_renderer.create( resourceManager, graphicsSystem ) );
	}

	GenericDataEditor::~GenericDataEditor()
	{
		m_renderer.dispose();
	}

	EditableFile* GenericDataEditor::openFile( const Path& fileName )
	{
		GenericDataFile* pFile = new GenericDataFile( fileName, *this, m_renderer );
		if( !pFile->load() )
		{
			delete pFile;
			return nullptr;
		}

		return pFile;
	}

	bool GenericDataEditor::saveEditable( Editable* pEditable )
	{
		GenericDataFile* pFile = static_cast< GenericDataFile* >( pEditable );
		return pFile->save();
	}

	void GenericDataEditor::closeEditable( Editable* pEditable )
	{
		delete pEditable;
	}

	void GenericDataEditor::registerView( GenericDataView& view )
	{
		m_renderer.registerView( view );
	}

	void GenericDataEditor::unregisterView( GenericDataView& view )
	{
		m_renderer.unregisterView( view );
	}
}
