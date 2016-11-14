#pragma once
#ifndef TIKI_EDITORFILE_HPP_INCLUDED
#define TIKI_EDITORFILE_HPP_INCLUDED

#include "tiki/editorinterface/ieditorfile.hpp"

#include <QString>

namespace tiki
{
	class EditorFile : public IEditorFile
	{
		TIKI_NONCOPYABLE_CLASS( EditorFile );

	public:

		explicit				EditorFile( Editor* pEditor, const QString& fileName, IFileEditor* pFileEditor );
		virtual					~EditorFile();

		virtual const QString&	getFileName() const TIKI_OVERRIDE_FINAL;
		virtual IFileEditor*	getFileEditor() const TIKI_OVERRIDE_FINAL;
		virtual QWidget*		getEditWidget() const TIKI_OVERRIDE_FINAL;

		void					setEditWidget( QWidget* pWidget );

		QString					getTabTitle() const;

		virtual bool			isDirty() const TIKI_OVERRIDE_FINAL;
		virtual void			markAsDirty() TIKI_OVERRIDE_FINAL;
		void					markAsSaved();

	private:

		Editor*				m_pEditor;

		QString				m_fileName;
		IFileEditor*		m_pFileEditor;
		QWidget*			m_pEditWidget;

		bool				m_isDirty;
	};
}

#endif // TIKI_EDITORFILE_HPP_INCLUDED
