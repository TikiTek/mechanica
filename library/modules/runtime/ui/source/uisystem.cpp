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

	bool UiSystem::create( const UiSystemParameters& parameters )
	{
		if( !m_renderer.create( parameters.rendererParameters ) )
		{
			dispose();
			return false;
		}

		ChunkStorageParameters storageParameters;
		// TODO

		if( !m_storage.create( storageParameters ) )
		{
			dispose();
			return false;
		}

		return true;
	}

	void UiSystem::dispose()
	{
		m_storage.dispose();

		m_renderer.dispose();
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