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
	class IEditorFile;
	class QtRibbonTab;

	class IFileEditor
	{
	public:

		virtual									~IFileEditor() { }

		virtual QWidget*						openFile( IEditorFile* pFile ) TIKI_PURE;
		virtual bool							saveFile( IEditorFile* pFile ) TIKI_PURE;
		virtual void							closeFile( IEditorFile* pFile ) TIKI_PURE;

		virtual QString							getFileExtension() const TIKI_PURE;
		virtual const QVector< QtRibbonTab* >&	getRibbonTabs() const TIKI_PURE;
		virtual const QVector< QDockWidget* >&	getDockWidgets() const TIKI_PURE;
	};
}

#endif // TIKI_IFILEEDITOR_HPP_INCLUDED
