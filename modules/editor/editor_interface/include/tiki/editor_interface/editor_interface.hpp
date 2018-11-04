#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/path.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	class BaseEditor;
	class Editable;
	class EditableFile;
	class EditorRibbon;
	class FileEditor;
	class Project;

	class EditorInterface
	{
	public:

		virtual					~EditorInterface() { }

		virtual EditableFile*	openFile( const Path& fileName ) TIKI_PURE;
		virtual void			openEditable( Editable* pEditable ) TIKI_PURE;
		virtual void			saveEditable( Editable* pEditable ) TIKI_PURE;
		virtual void			closeEditable( Editable* pEditable ) TIKI_PURE;
		virtual void			closeAll() TIKI_PURE;

		virtual void			registerFileEditor( FileEditor* pEditor ) TIKI_PURE;
		virtual void			unregisterFileEditor( FileEditor* pEditor ) TIKI_PURE;
		virtual FileEditor*		findEditorForFile( const Path& fileName ) const TIKI_PURE;

		virtual void			addGlobalRibbon( EditorRibbon* pRibbon ) TIKI_PURE;
		virtual void			removeGlobalRibbon( EditorRibbon* pRibbon ) TIKI_PURE;

		virtual Project&		getProject() TIKI_PURE;

		virtual const Path&		getProjectPath() const TIKI_PURE;
		virtual const Path&		getContentPath() const TIKI_PURE;
		virtual const Path&		getPackagePath() const TIKI_PURE;

		virtual DynamicString	getDialogTitle() const TIKI_PURE;
	};
}
