#pragma once
#ifndef TIKI_EDITOR_HPP_INCLUDED
#define TIKI_EDITOR_HPP_INCLUDED

#include "tiki/editorinterface/ieditorinterface.hpp"

#include <QVector>

namespace tiki
{
	class Editor : public IEditorInterface
	{
		TIKI_NONCOPYABLE_CLASS( Editor );

	public:

					Editor( EditorWindow* pWindow );
		virtual		~Editor();

	private:

		EditorWindow*			m_pWindow;

		QVector< IFileEditor* >	m_pEditors;
	};
}

#endif // TIKI_EDITOR_HPP_INCLUDED
