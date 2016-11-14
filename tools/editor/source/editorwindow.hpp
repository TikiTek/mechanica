#pragma once
#ifndef TIKI_EDITORWINDOW_HPP_INCLUDED
#define TIKI_EDITORWINDOW_HPP_INCLUDED

#include "tiki/genericdataeditor/genericdataeditor.hpp"
#include "tiki/packageeditor/packageeditor.hpp"
#include "tiki/qtapplication/qtmainwindow.hpp"



namespace tiki
{
	class QtRibbonButton;

	class EditorWindow : public QtMainWindow
	{
		Q_OBJECT

	public:

						EditorWindow();
		virtual			~EditorWindow();

		void			openFileTab( QWidget* pWidget, const QString& title );
		void			changeFileTab( QWidget* pWidget, const QString& title );
		void			closeFileTab( QWidget* pWidget );

	signals:

		void			fileCloseRequest( QWidget* pWidget );

	private slots:

		void			fileCloseRequested( int index );

	private:

		QTabWidget*		m_pFileTabs;
	};
}

#endif // TIKI_EDITORWINDOW_HPP_INCLUDED
