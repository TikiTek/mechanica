#pragma once
#ifndef TIKI_IEDITORINTERFACE_HPP_INCLUDED
#define TIKI_IEDITORINTERFACE_HPP_INCLUDED

#include "tiki/base/types.hpp"

#include <QDir>
#include <QString>

class QWidget;

namespace tiki
{
	class IEditorExtension;
	class IFile;
	class IFileEditor;

	class IEditorInterface
	{
	public:

		virtual					~IEditorInterface() { }

		virtual IFile*			openFile( const QString& fileName ) TIKI_PURE;
		virtual void			saveFile( IFile* pFile ) TIKI_PURE;
		virtual void			closeFile( IFile* pFile ) TIKI_PURE;
		virtual void			closeAllFiles() TIKI_PURE;

		virtual void			registerFileEditor( IFileEditor* pEditor ) TIKI_PURE;
		virtual void			unregisterFileEditor( IFileEditor* pEditor ) TIKI_PURE;
		virtual IFileEditor*	findEditorForFile( const QString& fileName ) const TIKI_PURE;

		virtual void			registerEditorExtension( IEditorExtension* pExtension ) TIKI_PURE;
		virtual void			unregisterEditorExtension( IEditorExtension* pExtension ) TIKI_PURE;

		virtual QDir			getProjectPath() const TIKI_PURE;
		virtual QDir			getContentPath() const TIKI_PURE;
		virtual QDir			getPackagePath() const TIKI_PURE;

		virtual QWidget*		getDialogParent() const TIKI_PURE;
		virtual QString			getDialogTitle() const TIKI_PURE;
	};
}

#endif // TIKI_IEDITORINTERFACE_HPP_INCLUDED
