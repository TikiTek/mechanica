#pragma once
#ifndef TIKI_IEDITORINTERFACE_HPP_INCLUDED
#define TIKI_IEDITORINTERFACE_HPP_INCLUDED

#include "tiki/base/types.hpp"

#include <QString>

class QWidget;
class QDockWidget;

namespace tiki
{
	class IEditorFile;
	class IFileEditor;
	class QtRibbonTab;

	class IEditorInterface
	{
	public:

		virtual					~IEditorInterface() { }

		virtual IEditorFile*	openFile( const QString& fileName ) TIKI_PURE;
		virtual void			saveFile( IEditorFile* pFile ) TIKI_PURE;
		virtual void			closeFile( IEditorFile* pFile ) TIKI_PURE;
		virtual void			closeAllFiles() TIKI_PURE;

		virtual void			registerFileEditor( IFileEditor* pEditor ) TIKI_PURE;
		virtual void			unregisterFileEditor( IFileEditor* pEditor ) TIKI_PURE;

		virtual void			addGlobalRibbonTab( QtRibbonTab* pTab ) TIKI_PURE;
		virtual void			removeGlobalRibbonTab( QtRibbonTab* pTab ) TIKI_PURE;

		virtual void			addGlobalDockWidget( QDockWidget* pWidget ) TIKI_PURE;
		virtual void			removeGlobalDockWidget( QDockWidget* pWidget ) TIKI_PURE;

		virtual QString			getProjectPath() const TIKI_PURE;
		virtual QString			getContentPath() const TIKI_PURE;
		virtual QString			getPackagePath() const TIKI_PURE;

		virtual QWidget*		getDialogParent() const TIKI_PURE;
		virtual QString			getDialogTitle() const TIKI_PURE;
	};
}

#endif // TIKI_IEDITORINTERFACE_HPP_INCLUDED
