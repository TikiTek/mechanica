#include "tiki/game_states/menu_state.hpp"

#include "tiki/game/game.hpp"
#include "tiki/game_flow/game_flow_system.hpp"

namespace tiki
{
	MenuState::MenuState()
	{
		m_pGame			= nullptr;
		m_pParentState	= nullptr;
	}

	MenuState::~MenuState()
	{
		TIKI_ASSERT( m_pGame == nullptr );
		TIKI_ASSERT( m_pParentState == nullptr );
	}

	void MenuState::create( Game* pGame, ApplicationState* pParentState )
	{
		TIKI_ASSERT( pGame != nullptr );
		TIKI_ASSERT( pParentState != nullptr );
		TIKI_ASSERT( m_pParentState == nullptr );

		m_pGame			= pGame;
		m_pParentState	= pParentState;
	}

	void MenuState::dispose()
	{
		m_pGame			= nullptr;
		m_pParentState	= nullptr;
	}

	TransitionState MenuState::processCreationStep( size_t currentStep, bool isInital )
	{
		switch( currentStep )
		{
		case MenuStateTransitionSteps_LoadResources:
			{
				ResourceRequestPool& resourceRequestPool = m_pGame->getResourceRequestPool();

				if( isInital )
				{
					resourceRequestPool.beginLoadResource( &m_pBundle, "menu_bundle.menu_bundle" );
				}

				return waitForResources( resourceRequestPool );
			}
			break;

		default:
			break;
		}

		return TransitionState_Finish;
	}

	TransitionState MenuState::processDestructionStep( size_t currentStep, bool isInital )
	{
		switch( currentStep )
		{
		case MenuStateTransitionSteps_LoadResources:
			{
				ResourceManager& resourceManager = m_pGame->getResourceManager();

				resourceManager.unloadResource( m_pBundle );
			}
			break;

		default:
			break;
		}

		return TransitionState_Finish;
	}

	void MenuState::update()
	{

	}

	void MenuState::render( GraphicsContext& graphicsContext )
	{
		const MechanicaMenuBundle& bundle = m_pBundle->getData();

		const ImmediateRenderer& renderer = m_pGame->getImmediateRenderer();
		renderer.beginRenderPass();
		const Vector2 renderSize = renderer.getRenderTargetSize();
		const Vector2 backgroundSize = bundle.background->getTextureData().getVectorSize();

		const Vector2 diff = vector::div( vector::create( renderSize ), backgroundSize );
		const Vector2 extends = vector::scale( vector::create( backgroundSize ), (diff.x > diff.y ? diff.x : diff.y) );

		const AxisAlignedRectangle backgroundRectangle = createAxisAlignedRectangleCentered(
			vector::scale( vector::create( renderSize ), 0.5f ),
			extends
		);
		renderer.drawTexturedRectangle( bundle.background->getTextureData(), backgroundRectangle );

		renderer.endRenderPass();
	}

	bool MenuState::processInputEvent( const InputEvent& inputEvent )
	{
		//if (inputEvent.eventType == InputEventType_Keyboard_Up)
		//{
		//	m_pGame->getGameFlowSystem().startTransition( GameStates_Play );

		//	return true;
		//}

		return false;
	}
}