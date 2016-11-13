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

		virtual									~IFileEditor() { }

		virtual QWidget*						openFile( const QString& fileName ) TIKI_PURE;
		virtual void							saveFile( QWidget* pWidget ) TIKI_PURE;
		virtual void							closeFile( QWidget* pWidget ) TIKI_PURE;

		virtual QString							getFileExtension() const TIKI_PURE;
		virtual const QVector< QtRibbonTab* >&	getRibbonTabs() const TIKI_PURE;
		virtual const QVector< QDockWidget* >&	getDockWidgets() const TIKI_PURE;
	};
}

#endif // TIKI_IFILEEDITOR_HPP_INCLUDED
