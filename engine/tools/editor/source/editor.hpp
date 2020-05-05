#pragma once

#include "tiki/editor_interface/editor_interface.hpp"

#include "tiki/container/map.hpp"
#include "tiki/resource/resource.hpp"
#include "tiki/tool_application/tool_message_box.hpp"
#include "tiki/tool_project/project.hpp"

#include "editor_editable_ribbon.hpp"
#include "editor_file_browser_ui.hpp"

struct ImHorizontalSplitter;

namespace tiki
{
	class AssetConverterInterface;
	class ConverterEditor;
	class EditorRibbon;
	class EntityTemplateEditor;
	class GenericDataEditor;
	class PackageEditor;
	class PacketManager;
	class ResourceRequest;
	struct InputEvent;

	class Editor : public EditorInterface
	{
		TIKI_NONCOPYABLE_CLASS( Editor );

	public:

										Editor( Project& project );
		virtual							~Editor();

		bool							create();
		void							dispose();

		void							update( float deltaTime );
		void							render( GraphicsContext& graphicsContext );

		bool							processToolInputEvent( const InputEvent& inputEvent );

		void							doUi();

		virtual EditableFile*			openFile( const Path& fileName ) TIKI_OVERRIDE_FINAL;
		virtual void					openEditable( Editable* pEditable ) TIKI_OVERRIDE_FINAL;
		virtual void					saveEditable( Editable* pEditable ) TIKI_OVERRIDE_FINAL;
		virtual void					closeEditable( Editable* pEditable ) TIKI_OVERRIDE_FINAL;
		virtual void					closeAll() TIKI_OVERRIDE_FINAL;
		Editable*						getCurrentEditable() { return m_pCurrentEditable; }

		virtual void					registerFileEditor( FileEditor* pEditor ) TIKI_OVERRIDE_FINAL;
		virtual void					unregisterFileEditor( FileEditor* pEditor ) TIKI_OVERRIDE_FINAL;
		virtual FileEditor*				findEditorForFile( const Path& fileName ) const;

		virtual void					addGlobalRibbon( EditorRibbon* pTab ) TIKI_OVERRIDE_FINAL;
		virtual void					removeGlobalRibbon( EditorRibbon* pTab ) TIKI_OVERRIDE_FINAL;

		//virtual void					addGlobalDockWidget( QDockWidget* pWidget, Qt::DockWidgetArea area ) TIKI_OVERRIDE_FINAL;
		//virtual void					removeGlobalDockWidget( QDockWidget* pWidget ) TIKI_OVERRIDE_FINAL;

		virtual Project&				getProject() TIKI_OVERRIDE_FINAL;

		virtual const Path&				getProjectPath() const TIKI_OVERRIDE_FINAL;
		virtual const Path&				getContentPath() const TIKI_OVERRIDE_FINAL;
		virtual const Path&				getPackagePath() const TIKI_OVERRIDE_FINAL;

		virtual DynamicString			getDialogTitle() const TIKI_OVERRIDE_FINAL;

	private:

		struct EditorResource
		{
			EditorResourceResult	result;
			const ResourceRequest*	pRequest;
			const Resource*			pResource;
		};
		typedef Map< ResourceId, EditorResource > ResourceMap;

		Project&						m_project;
		AssetConverterInterface*		m_pAssetConverter;

		Path							m_projectPath;
		Path							m_contentPath;
		Path							m_packagePath;

		ResourceMap						m_resources;

		List< BaseEditor* >				m_editors;
		List< EditorRibbon* >			m_ribbons;
		EditorRibbon*					m_pCurrentRibbon;

		ConverterEditor*				m_pConverterEditor;
		EntityTemplateEditor*			m_pEntityTemplateEditor;
		GenericDataEditor*				m_pGenericDataEditor;
		PackageEditor*					m_pPackageEditor;

		List< Editable* >				m_editables;
		Editable*						m_pCurrentEditable;

		float							m_fileBrowserWidth = 350.0f;
		EditorFileBrowserUi				m_fileBrowserUi;
		EditorEditableRibbon			m_editableRibbon;
		ToolMessageBox					m_messageBox;

		//QShortcut						m_openShortcut;
		//QShortcut						m_saveShortcut;
		//QShortcut						m_closeShortcut;

		virtual EditorResourceResult	getResource( const Resource** ppResource, const char* pFilename, fourcc resourceType ) TIKI_OVERRIDE_FINAL;

		void							saveOnCloseCallback( ToolMessageBoxButton button, UserData userData );
		void							closeEditableInternal( Editable* pEditable );

		void							setProjectPathes();
		void							setPackagePath();

		void							doRibbonUi();
		void							doFileBrowserUi( ImHorizontalSplitter& splitter );
		void							doEditableUi( ImHorizontalSplitter& splitter );
	};
}
