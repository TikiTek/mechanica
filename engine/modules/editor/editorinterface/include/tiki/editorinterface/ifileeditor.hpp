#pragma once
#ifndef TIKI_IFILEEDITOR_HPP_INCLUDED
#define TIKI_IFILEEDITOR_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/container/arrayview.hpp"

#include <QIcon>
#include <QString>
#include <QVector>

class QDockWidget;
class QWidget;

namespace tiki
{
	class IFile;
	class QtRibbonTab;

	class IFileEditor
	{
	public:

		virtual									~IFileEditor() { }

		virtual QWidget*						openFile( IFile* pFile ) TIKI_PURE;
		virtual bool							saveFile( IFile* pFile ) TIKI_PURE;
		virtual void							closeFile( IFile* pFile ) TIKI_PURE;

		virtual QString							getFileTypeName() const TIKI_PURE;
		virtual QString							getFileExtension() const TIKI_PURE;
		virtual QIcon							getFileIcon() const TIKI_PURE;

		virtual const QVector< QtRibbonTab* >&	getRibbonTabs() const TIKI_PURE;
		virtual const QVector< QDockWidget* >&	getDockWidgets() const TIKI_PURE;
	};
}

#endif // TIKI_IFILEEDITOR_HPP_INCLUDED
