#pragma once
#ifndef TIKI_EDITORFILE_HPP_INCLUDED
#define TIKI_EDITORFILE_HPP_INCLUDED

namespace tiki
{
	class EditorFile
	{
		TIKI_NONCOPYABLE_CLASS( EditorFile );

	public:

							EditorFile( IFileEditor* pEditor, const QString& fileName );
		virtual				~EditorFile();

		const QString&		getFileName() const { return m_fileName; }
		const IFileEditor*	getEditor() const { return m_pEditor; }

		//bool				isDirty() const;

	private:

		QString			m_fileName;
		IFileEditor*	m_pEditor;
	};
}

#endif // TIKI_EDITORFILE_HPP_INCLUDED
