#include "tiki/editorbase/editorsystem.hpp"

#include "tiki/editorbase/transformgizmo.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/input/inputevent.hpp"
#include "tiki/io/gamebuildfilesystem.hpp"
#include "tiki/math/box.hpp"
#include "tiki/math/intersection.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/resource/resourcemanager.hpp"
#include "tiki/runtimeshared/frameworkfactories.hpp"

namespace tiki
{
	struct EditorData
	{
		GraphicsSystem		graphicsSystem;
		ImmediateRenderer	immediateRenderer;
		ResourceManager		resourceManager;
		GamebuildFileSystem fileSystem;
		FrameworkFactories	frameworkFactories;

		Ray3				testRay;
	};


	EditorSystem::EditorSystem()
	{

	}

	bool EditorSystem::create( EditorParameters^ parameters )
	{
		m_pData = new EditorData();

		m_TransformGizmo = gcnew TransformGizmo();

		GraphicsSystemParameters graphicsParams;
		graphicsParams.pWindowHandle = (void*)parameters->windowHandle;
		if ( !m_pData->graphicsSystem.create( graphicsParams ) )
		{
			dispose();
			return false;
		}

		m_pData->fileSystem.create( "../../../../../../gamebuild/");

		ResourceManagerParameters resourceParams;
		resourceParams.pFileSystem = &m_pData->fileSystem;

		if ( !m_pData->resourceManager.create(resourceParams) )
		{
			dispose();
			return false;
		}

		m_pData->frameworkFactories.create( m_pData->resourceManager, m_pData->graphicsSystem );
		
		if ( !m_pData->immediateRenderer.create( m_pData->graphicsSystem, m_pData->resourceManager ) )
		{
			dispose();
			return false;
		}

		if ( !m_input.create() )
		{
			return false;
		}
		
		if ( !m_camera.create( m_pData->graphicsSystem ) )
		{
			return false;
		}

		if ( !m_TransformGizmo->create( &m_pData->immediateRenderer, %m_camera) )
		{
			return false;
		}

		return true;
	}

	void EditorSystem::dispose()
	{
		if ( m_pData == nullptr )
		{
			return;
		}

		m_camera.dispose();
		m_input.dispose();

		m_pData->immediateRenderer.dispose( m_pData->graphicsSystem, m_pData->resourceManager );
		m_pData->frameworkFactories.dispose(m_pData->resourceManager);
		m_pData->resourceManager.dispose();
		m_pData->graphicsSystem.dispose();
		m_pData->fileSystem.dispose();

		delete m_pData;
		m_pData = nullptr;
	}

	void EditorSystem::update()
	{
		InputEvent inputEvent;
		while ( m_input.popInputEvent( inputEvent ) )
		{
			processInputEvent( inputEvent );
		}

		m_camera.update( 0.01666f ); // todo time

		m_TransformGizmo->update( 0.01666f );
	}

	void EditorSystem::render()
	{
		GraphicsContext& context = m_pData->graphicsSystem.beginFrame();
		ImmediateRenderer& renderer = m_pData->immediateRenderer;		

		context.clear( context.getBackBuffer(), TIKI_COLOR_BLACK );

		renderer.beginRendering( context );
		renderer.beginRenderPass( nullptr, &m_camera.getCamera() );

		//Rectangle rect;
		//rect.x		= -1.0f;
		//rect.y		= -1.0f;
		//rect.width	= 2.0f;
		//rect.height	= 2.0f;
		//renderer.drawRectangle( rect, TIKI_COLOR_GREEN );


		renderer.drawRay( m_pData->testRay );

		m_TransformGizmo->render();

		renderer.endRenderPass();
		renderer.endRendering();
		
		m_pData->graphicsSystem.endFrame();
	}

	bool EditorSystem::resize( int x, int y )
	{
		m_camera.resize( x, y );

		return m_pData->graphicsSystem.resize( x, y );
	}

	EditorInput^ EditorSystem::Input::get()
	{
		return %m_input;
	}

	void EditorSystem::processInputEvent( InputEvent& inputEvent )
	{
		if ( inputEvent.eventType == InputEventType_Mouse_ButtonDown && inputEvent.data.mouseButton.button == MouseButton_Left )
		{
			m_pData->testRay = m_camera.getMouseRay();
		}

		m_camera.processInputEvent( inputEvent );
		m_TransformGizmo->processInputEvent( inputEvent );
	}
}