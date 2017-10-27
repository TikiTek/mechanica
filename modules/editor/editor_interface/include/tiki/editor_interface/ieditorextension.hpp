#pragma once
#ifndef TIKI_IEDITOREXTENSION_HPP_INCLUDED
#define TIKI_IEDITOREXTENSION_HPP_INCLUDED

#include "tiki/base/types.hpp"

#include <QSet>

class QDockWidget;
class QWidget;

namespace tiki
{
	class IFile;
	class QtRibbonTab;

	class IEditorExtension
	{
	public:

		const QSet< QtRibbonTab* >&	getGlobalRibbonTabs() const { return m_globalTabs; }
		const QSet< QDockWidget* >&	getGlobalDockWidgets() const { return m_globalDocks; }

	protected:

		QSet< QtRibbonTab* >		m_globalTabs;
		QSet< QDockWidget* >		m_globalDocks;

		virtual						~IEditorExtension() { }
	};
}

#endif // TIKI_IEDITOREXTENSION_HPP_INCLUDED
