#include "editorfile.hpp"

namespace tiki
{
	EditorFile::EditorFile( Editor* pEditor, const QString& fileName, IFileEditor* pFileEditor )
		: m_pEditor( pEditor )
		, m_fileName( fileName )
		, m_pFileEditor( pFileEditor )
		, m_pEditWidget( nullptr )
		, m_isDirty( false )
	{
	}

	EditorFile::~EditorFile()
	{
	}

	const QString& EditorFile::getFileName() const
	{
		return m_fileName;
	}

	IFileEditor* EditorFile::getFileEditor() const
	{
		return m_pFileEditor;
	}

	QWidget* EditorFile::getEditWidget() const
	{
		return m_pEditWidget;
	}

	void EditorFile::setEditWidget( QWidget* pWidget )
	{
		m_pEditWidget = pWidget;
	}

	QString EditorFile::getTabTitle() const
	{
		QFileInfo fileInfo( m_fileName );
		QString title = fileInfo.baseName();

		if( m_isDirty )
		{
			title += "*";
		}

		return title;
	}

	bool EditorFile::isDirty() const
	{
		return m_isDirty;
	}

	void EditorFile::markAsDirty()
	{
		m_isDirty = true;
		m_pEditor->markFileAsDirty( this );
	}

	void EditorFile::markAsSaved()
	{
		m_isDirty = false;
	}
}