
#include "tiki/editornative/editorsystem.hpp"

#include "tiki/graphics/graphicssystem.hpp"

namespace tiki
{
	EditorSystem::EditorSystem()
	{
		m_pGraphicsSystem = nullptr;
		m_pResourceManager = nullptr;
	}

	bool EditorSystem::create( EditorParameters^ parameters )
	{
		GraphicsSystemParameters graphicsParams;
		graphicsParams.pWindowHandle = (void*)parameters->windowHandle;
		
		m_pGraphicsSystem = new GraphicsSystem();
		m_pGraphicsSystem->create( graphicsParams );

		GraphicsContext& c = m_pGraphicsSystem->beginFrame();

		c.clear( c.getBackBuffer() );

		m_pGraphicsSystem->endFrame();

		return true;
	}

	void EditorSystem::dispose()
	{

	}
}