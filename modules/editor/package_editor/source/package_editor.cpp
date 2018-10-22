#include "tiki/package_editor/package_editor.hpp"

#include "tiki/editor_interface/editable_file.hpp"
#include "tiki/editor_interface/editor_interface.hpp"
#include "tiki/package_editor/package_file.hpp"
#include "tiki/toolproject/package.hpp"

#include "res_package_editor.hpp"

#include <imgui.h>

namespace tiki
{
	PackageEditor::PackageEditor( EditorInterface* pInterface )
		: FileEditor( pInterface, getPackageEditorResource( PackageEditorResources_BrowserPackage ), "Package", "package" )
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
		PackageFile* pPackage = new PackageFile( fileName, this );
		if( !pPackage->load() )
		{
			delete pPackage;
			return nullptr;
		}

		m_pPackageFile = pPackage;
		return pPackage;
	}

	bool PackageEditor::saveEditable( Editable* pEditable )
	{
		PackageFile* pPackage = static_cast< PackageFile* >( pEditable );
		return pPackage->save();
	}

	void PackageEditor::closeEditable( Editable* pEditable )
	{
		PackageFile* pPackage = static_cast< PackageFile* >( pEditable );
		m_pPackageFile = nullptr;
		delete pPackage;
	}

	void PackageEditor::doUi()
	{
	}

	void PackageEditor::openPackage( Package& package )
	{
		if( &package == m_pPackage )
		{
			return;
		}

		closePackage();

		m_pPackage		= &package;
		m_pPackageFile	= new PackageFile( package.getFilename(), this );

		m_pInterface->openEditable( m_pPackageFile );
	}

	void PackageEditor::closePackage()
	{
		if( m_pPackageFile == nullptr )
		{
			return;
		}

		m_pInterface->closeAll();

		m_pPackage		= nullptr;
		m_pPackageFile	= nullptr;
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