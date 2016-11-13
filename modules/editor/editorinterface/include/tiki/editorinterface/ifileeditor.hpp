#pragma once
#ifndef TIKI_IFILEEDITOR_HPP_INCLUDED
#define TIKI_IFILEEDITOR_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/container/arrayview.hpp"

#include <QString>
#include <QVector>

class QDockWidget;
class QWidget;

namespace tiki
{
	class QtRibbonTab;

	class IFileEditor
	{
	public:

		virtual							~IFileEditor() { }

		QWidget*						openFile( const QString& fileName ) TIKI_PURE;
		void							closeFile( QWidget* pWidget ) TIKI_PURE;

		QString							getFileExtension() const TIKI_PURE;
		const QVector< QtRibbonTab* >&	getRibbonTabs() const TIKI_PURE;
		const QVector< QDockWidget* >&	getDockWidgets() const TIKI_PURE;
	};
}

#endif // TIKI_IFILEEDITOR_HPP_INCLUDED
