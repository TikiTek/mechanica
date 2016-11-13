#include "editor.hpp"

#include "editorwindow.hpp"

namespace tiki
{
	Editor::Editor( EditorWindow* pWindow )
		: m_pWindow( pWindow )
	{
	}

	Editor::~Editor()
	{
	}

	void Editor::openFile( const QString& fileName )
	{

	}

	void Editor::closeFile( const QString& fileName )
	{

	}

	void Editor::closeAllFiles()
	{

	}

	void Editor::registerFileEditor( IFileEditor* pEditor )
	{

	}

	void Editor::unregisterFileEditor( IFileEditor* pEditor )
	{

	}

	void Editor::addGlobalRibbonTab( QtRibbonTab* pTab )
	{
		m_pWindow->addRibbonTab( pTab );
	}

	void Editor::removeGlobalRibbonTab( QtRibbonTab* pTab )
	{
		m_pWindow->removeRibbonTab( pTab );
	}

	void Editor::addGlobalDockWidget( QDockWidget* pWidget )
	{
		m_pWindow->addDockWidget( Qt::LeftDockWidgetArea, pWidget );
	}

	void Editor::removeGlobalDockWidget( QDockWidget* pWidget )
	{
		m_pWindow->removeDockWidget( pWidget );
	}

	void Editor::beginEditing( EditorFile& pFile )
	{

	}

	void Editor::endEditing( EditorFile& file )
	{

	}

}