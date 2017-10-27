#pragma once
#ifndef TIKI_IEDITOR_HPP_INCLUDED
#define TIKI_IEDITOR_HPP_INCLUDED

#include "tiki/base/types.hpp"

#include <QIcon>
#include <QSet>

class QDockWidget;
class QWidget;

namespace tiki
{
	class IEditable;
	class IFileEditor;
	class QtRibbonTab;

	class IEditor
	{
	public:

		virtual bool				saveEditable( IEditable* pEditable ) TIKI_PURE;
		virtual void				closeEditable( IEditable* pEditable ) TIKI_PURE;

		virtual QIcon				getEditableIcon() const TIKI_PURE;

		const QSet< QtRibbonTab* >&	getEditableRibbonTabs() const { return m_editableTabs; }
		const QSet< QDockWidget* >&	getEditableDockWidgets() const { return m_editableDocks; }

		virtual IFileEditor*		asFileEditor() { return nullptr; }

	protected:

		QSet< QtRibbonTab* >		m_editableTabs;
		QSet< QDockWidget* >		m_editableDocks;

		virtual						~IEditor() { }
	};
}

#endif // TIKI_IEDITOR_HPP_INCLUDED
