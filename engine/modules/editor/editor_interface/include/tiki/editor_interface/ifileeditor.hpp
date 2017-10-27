#pragma once
#ifndef TIKI_IFILEEDITOR_HPP_INCLUDED
#define TIKI_IFILEEDITOR_HPP_INCLUDED

#include "tiki/base/types.hpp"

#include <QIcon>
#include <QString>
#include <QSet>

class QDockWidget;
class QWidget;

namespace tiki
{
	class IFile;
	class QtRibbonTab;

	class IFileEditor
	{
	public:

		virtual QWidget*			openFile( IFile* pFile ) TIKI_PURE;
		virtual bool				saveFile( IFile* pFile ) TIKI_PURE;
		virtual void				closeFile( IFile* pFile ) TIKI_PURE;

		virtual QString				getFileTypeName() const TIKI_PURE;
		virtual QString				getFileExtension() const TIKI_PURE;
		virtual QIcon				getFileIcon() const TIKI_PURE;

		const QSet< QtRibbonTab* >&	getFileRibbonTabs() const { return m_fileTabs; }
		const QSet< QDockWidget* >&	getFileDockWidgets() const { return m_fileDocks; }

	protected:

		QSet< QtRibbonTab* >		m_fileTabs;
		QSet< QDockWidget* >		m_fileDocks;

		virtual						~IFileEditor() { }
	};
}

#endif // TIKI_IFILEEDITOR_HPP_INCLUDED
