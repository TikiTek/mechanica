#include "tiki/package_editor/package_editor.hpp"

#include "tiki/editor_interface/editable_file.hpp"
#include "tiki/editor_interface/editor_interface.hpp"
#include "tiki/package_editor/package_file.hpp"
#include "tiki/tool_project/package.hpp"
#include "tiki/tool_project/project.hpp"

#include "res_package_editor.hpp"

#include <imgui.h>

namespace tiki
{
	PackageEditor::PackageEditor( EditorInterface& editor )
		: FileEditor( editor, getPackageEditorResource( PackageEditorResources_BrowserPackage ), "Package", ".package" )
		, m_ribbon( editor, *this )
	{
		m_pPackage		= nullptr;
		m_pPackageFile	= nullptr;

		m_editor.addGlobalRibbon( &m_ribbon );
	}

	PackageEditor::~PackageEditor()
	{
		m_editor.removeGlobalRibbon( &m_ribbon );
	}

	EditableFile* PackageEditor::openFile( const Path& fileName )
	{
		Package* pPackage = m_editor.getProject().findPackage( fileName.getBasename() );
		if( pPackage == nullptr )
		{
			return nullptr;
		}

		openPackage( *pPackage );

		return m_pPackageFile;
	}

	bool PackageEditor::saveEditable( Editable& editable )
	{
		PackageFile& package = static_cast< PackageFile& >( editable );
		return package.save();
	}

	void PackageEditor::closeEditable( Editable& editable )
	{
		TIKI_ASSERT( &editable == m_pPackageFile );

		PackageFile& package = static_cast< PackageFile& >( editable );
		m_pPackageFile = nullptr;
		delete &package;
	}

	void PackageEditor::doUi()
	{
	}

	bool PackageEditor::openPackage( Package& package )
	{
		if( &package == m_pPackage )
		{
			return true;
		}

		closePackage();

		m_pPackage		= &package;
		m_pPackageFile	= new PackageFile( package, *this );

		if( !m_pPackageFile->load() )
		{
			delete m_pPackageFile;

			m_pPackage		= nullptr;
			m_pPackageFile	= nullptr;

			return false;
		}

		m_editor.openEditable( *m_pPackageFile );
		return true;
	}

	void PackageEditor::closePackage()
	{
		if( m_pPackageFile == nullptr )
		{
			return;
		}

		m_pPackage		= nullptr;
		m_pPackageFile	= nullptr;

		m_editor.closeAll();
	}

	void PackageEditor::editPackage()
	{
		if( m_pPackageFile == nullptr )
		{
			return;
		}

		m_editor.openEditable( *m_pPackageFile );
	}
}