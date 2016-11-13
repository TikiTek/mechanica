#pragma once
#ifndef TIKI_EDITORWINDOW_HPP_INCLUDED
#define TIKI_EDITORWINDOW_HPP_INCLUDED

#include "tiki/genericdataeditor/genericdataeditor.hpp"
#include "tiki/packageeditor/packageeditor.hpp"
#include "tiki/qtapplication/qtmainwindow.hpp"

#include <QList>
#include <QVector>

namespace tiki
{
	class QtRibbonButton;

	class EditorWindow : public QtMainWindow
	{
		Q_OBJECT

	public:

						EditorWindow();
		virtual			~EditorWindow();

	private:

		QtRibbonTab*	m_pFileRibbon;

		QtRibbonButton*	m_pFileNewButton;

	};
}

#endif // TIKI_EDITORWINDOW_HPP_INCLUDED
