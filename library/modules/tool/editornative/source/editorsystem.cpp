
#include "tiki/editornative/editorsystem.hpp"

#include "tiki/editornative/transformgizmo.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/io/gamebuildfilesystem.hpp"
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

		return true;
	}

	void EditorSystem::dispose()
	{
		if ( m_pData == nullptr )
		{
			return;
		}

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
		
	}

	void EditorSystem::render()
	{
		GraphicsContext& context = m_pData->graphicsSystem.beginFrame();
		ImmediateRenderer& renderer = m_pData->immediateRenderer;		

		context.clear( context.getBackBuffer(), TIKI_COLOR_BLUE );

		renderer.beginRendering( context );
		renderer.beginRenderPass();

		Rectangle rect;
		rect.x		= 10.0f;
		rect.y		= 10.0f;
		rect.width	= 100.0f;
		rect.height	= 100.0f;
		renderer.drawRectangle( rect, TIKI_COLOR_GREEN );

		renderer.endRenderPass();
		renderer.endRendering();
		
		m_pData->graphicsSystem.endFrame();
	}

	bool EditorSystem::resize( int x, int y )
	{
		return m_pData->graphicsSystem.resize(x, y);
	}

	void EditorSystem::onKeyDown( KeyEventArgs^ e )
	{

	}

	void EditorSystem::onKeyUp( KeyEventArgs^ e )
	{

	}

	void EditorSystem::onMouseDown( MouseButtonEventArgs^ e )
	{

	}

	void EditorSystem::onMouseUp( MouseButtonEventArgs^ e )
	{

	}

	void EditorSystem::onMouseDoubleClick( MouseButtonEventArgs^ e )
	{

	}

	void EditorSystem::onMouseMove( MouseEventArgs^ e )
	{

	}

}