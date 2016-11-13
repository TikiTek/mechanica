#pragma once
#ifndef TIKI_IEDITORINTERFACE_HPP_INCLUDED
#define TIKI_IEDITORINTERFACE_HPP_INCLUDED

#include <QString>

namespace tiki
{
	class IFileEditor;

	class IEditorInterface
	{
	public:

		virtual			~IEditorInterface() { }

		void			openFile( const QString& fileName ) TIKI_PURE;
		void			closeFile( const QString& fileName ) TIKI_PURE;
		void			closeAllFiles() TIKI_PURE;

		void			registerFileEditor( IFileEditor* pEditor ) TIKI_PURE;
		void			unregisterFileEditor( IFileEditor* pEditor ) TIKI_PURE;

		void			addGlobalDockWidget( QDockWidget* pWidget ) TIKI_PURE;
		void			removeGlobalDockWidget( QDockWidget* pWidget ) TIKI_PURE;
	};
}

#endif // TIKI_IEDITORINTERFACE_HPP_INCLUDED
