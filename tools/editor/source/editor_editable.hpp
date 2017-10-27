#pragma once
#ifndef TIKI_EDITOR_FILE_HPP_INCLUDED
#define TIKI_EDITOR_FILE_HPP_INCLUDED

#include "tiki/editor_interface/ifile.hpp"

namespace tiki
{
	class EditorEditable : public IFile
	{
		TIKI_NONCOPYABLE_CLASS( EditorEditable );

	public:

		explicit				EditorEditable( Editor* pEditor, const QString& title, QWidget* pEditWidget, IEditor* pEditEditor );
		explicit				EditorEditable( Editor* pEditor, const QString& fileName, IFileEditor* pFileEditor );
		virtual					~EditorEditable();

		QString					getTabTitle() const;

		virtual const QString&	getTitle() const TIKI_OVERRIDE_FINAL;
		virtual IEditor*		getEditor() const TIKI_OVERRIDE_FINAL;
		virtual QWidget*		getEditWidget() const TIKI_OVERRIDE_FINAL;

		virtual const QString&	getFileName() const TIKI_OVERRIDE_FINAL;

		virtual bool			isDirty() const TIKI_OVERRIDE_FINAL;
		virtual void			markAsDirty() TIKI_OVERRIDE_FINAL;
		void					markAsSaved();

		void					setEditWidget( QWidget* pWidget );

	private:

		Editor*				m_pEditor;

		QString				m_title;
		QString				m_fileName;
		IEditor*			m_pEditEditor;
		QWidget*			m_pEditWidget;

		bool				m_isDirty;
	};
}

#endif // TIKI_EDITOR_FILE_HPP_INCLUDED
