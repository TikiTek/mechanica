#pragma once
#ifndef TIKI_EDITOR_WINDOW_HPP_INCLUDED
#define TIKI_EDITOR_WINDOW_HPP_INCLUDED

#include "tiki/qtapplication/qtmainwindow.hpp"

#include "tiki/generic_data_editor/generic_data_editor.hpp"
#include "tiki/package_editor/package_editor.hpp"

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

#endif // TIKI_EDITOR_WINDOW_HPP_INCLUDED
