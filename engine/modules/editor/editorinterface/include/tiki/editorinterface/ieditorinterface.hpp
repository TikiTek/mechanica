#pragma once
#ifndef TIKI_IEDITORINTERFACE_HPP_INCLUDED
#define TIKI_IEDITORINTERFACE_HPP_INCLUDED

#include "tiki/base/types.hpp"

#include <QString>

class QDockWidget;

namespace tiki
{
	class IFileEditor;
	class QtRibbonTab;

	class IEditorInterface
	{
	public:

		virtual			~IEditorInterface() { }

		virtual void	openFile( const QString& fileName ) TIKI_PURE;
		virtual void	closeFile( const QString& fileName ) TIKI_PURE;
		virtual void	closeAllFiles() TIKI_PURE;

		virtual void	registerFileEditor( IFileEditor* pEditor ) TIKI_PURE;
		virtual void	unregisterFileEditor( IFileEditor* pEditor ) TIKI_PURE;

		virtual void	addGlobalRibbonTab( QtRibbonTab* pTab ) TIKI_PURE;
		virtual void	removeGlobalRibbonTab( QtRibbonTab* pTab ) TIKI_PURE;

		virtual void	addGlobalDockWidget( QDockWidget* pWidget ) TIKI_PURE;
		virtual void	removeGlobalDockWidget( QDockWidget* pWidget ) TIKI_PURE;
	};
}

#endif // TIKI_IEDITORINTERFACE_HPP_INCLUDED
