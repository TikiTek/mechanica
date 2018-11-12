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
	PackageEditor::PackageEditor( EditorInterface* pInterface )
		: FileEditor( pInterface, getPackageEditorResource( PackageEditorResources_BrowserPackage ), "Package", ".package" )
		, m_ribbon( pInterface, *this )
	{
		m_pPackage		= nullptr;
		m_pPackageFile	= nullptr;

		m_pInterface->addGlobalRibbon( &m_ribbon );
	}

	PackageEditor::~PackageEditor()
	{
		m_pInterface->removeGlobalRibbon( &m_ribbon );
	}

	EditableFile* PackageEditor::openFile( const Path& fileName )
	{
		Package* pPackage = m_pInterface->getProject().findPackage( fileName.getFilename() );
		if( pPackage == nullptr )
		{
			return nullptr;
		}

		openPackage( *pPackage );

		return m_pPackageFile;
	}

	bool PackageEditor::saveEditable( Editable* pEditable )
	{
		PackageFile* pPackage = static_cast< PackageFile* >( pEditable );
		return pPackage->save();
	}

	void PackageEditor::closeEditable( Editable* pEditable )
	{
		TIKI_ASSERT( pEditable == m_pPackageFile );

		PackageFile* pPackage = static_cast< PackageFile* >( pEditable );
		m_pPackageFile = nullptr;
		delete pPackage;
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
		m_pPackageFile	= new PackageFile( package.getFilename(), this );

		if( !m_pPackageFile->load() )
		{
			delete m_pPackageFile;

			m_pPackage		= nullptr;
			m_pPackageFile	= nullptr;

			return false;
		}

		m_pInterface->openEditable( m_pPackageFile );
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

		m_pInterface->closeAll();
	}

	void PackageEditor::editPackage()
	{
		if( m_pPackageFile == nullptr )
		{
			return;
		}

		m_pInterface->openEditable( m_pPackageFile );
	}
}