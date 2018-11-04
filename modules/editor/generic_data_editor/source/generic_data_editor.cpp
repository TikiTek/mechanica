#include "tiki/generic_data_editor/generic_data_editor.hpp"

#include "tiki/editor_interface/editable_file.hpp"
#include "tiki/editor_interface/editor_interface.hpp"
#include "tiki/toolproject/package.hpp"
#include "tiki/toolproject/project.hpp"

#include "generic_data_file.hpp"

#include "res_generic_data_editor.hpp"

namespace tiki
{
	GenericDataEditor::GenericDataEditor( EditorInterface* pInterface )
		: FileEditor( pInterface, getGenericDataEditorResource( GenericDataEditorResources_BrowserFileGenericData ), "Generic Data", ".generic_data" )
	{
		for( const Package& package : pInterface->getProject().getPackages() )
		{
			m_typeCollection.addPackage( package );
		}
	}

	GenericDataEditor::~GenericDataEditor()
	{
	}

	EditableFile* GenericDataEditor::openFile( const Path& fileName )
	{
		GenericDataFile* pFile = new GenericDataFile( fileName, *this );
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
}
