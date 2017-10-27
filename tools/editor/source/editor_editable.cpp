#include "editor_editable.hpp"

namespace tiki
{
	EditorEditable::EditorEditable( Editor* pEditor, const QString& title, QWidget* pEditWidget, IEditor* pEditEditor )
		: m_pEditor( pEditor )
		, m_title( title )
		, m_pEditEditor( pEditEditor )
		, m_pEditWidget( pEditWidget )
		, m_isDirty( false )
	{
	}

	EditorEditable::EditorEditable( Editor* pEditor, const QString& fileName, IFileEditor* pFileEditor )
		: m_pEditor( pEditor )
		, m_fileName( fileName )
		, m_pEditEditor( pFileEditor )
		, m_pEditWidget( nullptr )
		, m_isDirty( false )
	{
		m_title = QFileInfo( m_fileName ).completeBaseName();
	}

	EditorEditable::~EditorEditable()
	{
	}

	QString EditorEditable::getTabTitle() const
	{
		QString title = m_title;
		if( m_isDirty )
		{
			title += "*";
		}

		return title;
	}

	const QString& EditorEditable::getTitle() const
	{
		return m_title;
	}

	const QString& EditorEditable::getFileName() const
	{
		return m_fileName;
	}

	IEditor* EditorEditable::getEditor() const
	{
		return m_pEditEditor;
	}

	QWidget* EditorEditable::getEditWidget() const
	{
		return m_pEditWidget;
	}

	bool EditorEditable::isDirty() const
	{
		return m_isDirty;
	}

	void EditorEditable::markAsDirty()
	{
		m_isDirty = true;
		m_pEditor->markFileAsDirty( this );
	}

	void EditorEditable::markAsSaved()
	{
		m_isDirty = false;
	}

	void EditorEditable::setEditWidget( QWidget* pWidget )
	{
		m_pEditWidget = pWidget;
	}
}