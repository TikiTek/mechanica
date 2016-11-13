#include "tiki/packageeditor/packageeditor.hpp"

#include "tiki/editorinterface/ieditorinterface.hpp"
#include "tiki/qtapplication/qtribbontab.hpp"
#include "tiki/packageeditor/packageeditorwidget.hpp"

namespace tiki
{
	PackageEditor::PackageEditor( IEditorInterface* pInterface )
		: m_pInterface( pInterface )
	{
		m_pRibbon = new QtRibbonTab( "Package" );

		pInterface->addGlobalRibbonTab( m_pRibbon );
	}

	PackageEditor::~PackageEditor()
	{

	}

	QWidget* PackageEditor::openFile( const QString& fileName )
	{
		PackageEditorWidget* pEditorWidget = new PackageEditorWidget();
		if( !pEditorWidget->openPackage( fileName ) )
		{
			delete pEditorWidget;
			return nullptr;
		}

		return pEditorWidget;
	}

	void PackageEditor::saveFile( QWidget* pWidget )
	{

	}

	void PackageEditor::closeFile( QWidget* pWidget )
	{
		PackageEditorWidget* pEditorWidget = (PackageEditorWidget*)pWidget;

		pEditorWidget->savePackage();
		delete pEditorWidget;
	}

	QString PackageEditor::getFileExtension() const
	{
		return ".tikipackage";
	}

	const QVector< QtRibbonTab* >& PackageEditor::getRibbonTabs() const
	{
		return m_tabs;
	}

	const QVector< QDockWidget* >& PackageEditor::getDockWidgets() const
	{
		return m_docks;
	}
}