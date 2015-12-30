#include "tiki/ui/uisystem.hpp"

#include "uitypes.hpp"

namespace tiki
{
	UiSystem::UiSystem()
	{
	}

	UiSystem::~UiSystem()
	{
	}

	bool UiSystem::create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager, const UiSystemParameters& parameters )
	{
		if( !m_renderer.create( graphicsSystem, resourceManager, parameters.rendererParameters ) ||
			!m_elementPool.create( parameters.maxElementCount ) )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		return true;
	}

	void UiSystem::dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		m_elementPool.dispose();
		
		m_renderer.dispose( graphicsSystem, resourceManager );
	}

	void UiSystem::update()
	{
		UiRenderData renderData;

		m_renderer.update( renderData );
	}

	void UiSystem::render( GraphicsContext& context ) const
	{
		m_renderer.render( context );
	}

	bool UiSystem::processInputEvent( InputEvent& inputEvent )
	{
		return false;
	}
}