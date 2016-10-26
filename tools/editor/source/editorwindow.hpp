#pragma once
#ifndef TIKI_EDITORWINDOW_HPP_INCLUDED
#define TIKI_EDITORWINDOW_HPP_INCLUDED

#include "tiki/qtapplication/qtmainwindow.hpp"

namespace tiki
{
	class QtRibbonButton;

	class EditorWindow : public QtMainWindow
	{
		Q_OBJECT

	public:

		EditorWindow();
		~EditorWindow();

	private:

		QtRibbonTab*	m_pFileRibbon;

		QtRibbonButton*	m_pFileNewButton;

	};
}

#endif // TIKI_EDITORWINDOW_HPP_INCLUDED
