#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/path.hpp"
#include "tiki/base/types.hpp"
#include "tiki/tool_application/tool_message_box.hpp"

namespace tiki
{
	class BaseEditor;
	class Editable;
	class EditableFile;
	class EditorRibbon;
	class FileEditor;
	class Project;
	class Resource;

	enum EditorResourceResult
	{
		EditorResourceResult_Busy,
		EditorResourceResult_Ok,
		EditorResourceResult_Error
	};

	class EditorInterface
	{
	public:

		virtual							~EditorInterface() { }

		virtual EditableFile*			openFile( const Path& fileName ) = 0;
		virtual void					openEditable( Editable& editable ) = 0;
		virtual void					saveEditable( Editable& editable ) = 0;
		virtual void					closeEditable( Editable& editable ) = 0;
		virtual void					closeAll() = 0;

		virtual void					registerFileEditor( FileEditor* pEditor ) = 0;
		virtual void					unregisterFileEditor( FileEditor* pEditor ) = 0;
		virtual FileEditor*				findEditorForFile( const Path& fileName ) const = 0;

		virtual void					addGlobalRibbon( EditorRibbon* pRibbon ) = 0;
		virtual void					removeGlobalRibbon( EditorRibbon* pRibbon ) = 0;

		virtual Project&				getProject() = 0;

		virtual const Path&				getProjectPath() const = 0;
		virtual const Path&				getContentPath() const = 0;
		virtual const Path&				getPackagePath() const = 0;

		virtual void					showMessageBox( const DynamicString& message, ToolMessageBoxButtonFlagMask buttons = ToolMessageBoxButtons_Ok, ToolMessageBoxIcon icon = ToolMessageBoxIcon::None, ToolMessageBoxCallbackDelegate callback = ToolMessageBoxCallbackDelegate(), UserData userData = UserData() ) = 0;

		template< class T >
		inline EditorResourceResult		getResource( const T** ppResource, const DynamicString& fileName );

	private:

		virtual EditorResourceResult	getResource( const Resource** ppResource, const char* pFilename, fourcc resourceType ) = 0;
	};
}

#include "../../../source/editor_interface.inl"
