#include "tiki/game_states/menu_state.hpp"

#include "tiki/base/float64.hpp"
#include "tiki/base/random.hpp"
#include "tiki/game/game.hpp"
#include "tiki/game_flow/game_flow_system.hpp"
#include "tiki/math/rectangle.hpp"

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

		case MenuStateTransitionSteps_Initialize:
			{
				Renderer2dRenderParameters& renderParameters = m_pParentState->getRenderParameters();
				renderParameters.enableBloom		= false;
				renderParameters.enableColorGrading	= false;

				createMainCircles();
				createMenuCircles( m_pBundle->getData().menuElements );
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
		case MenuStateTransitionSteps_Initialize:
			{
				m_mainCircles.clear();
				m_menuEntries.clear();
			}
			break;

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
		const GameTime time			= m_pGame->getFrameTimer().getTime();
		const Vector2 renderSize	= m_pParentState->getRenderer().getVectorSize();

		m_mainCircles[ 0u ].angle = float( m_mouseState.x ) / 500.0f;
		m_mainCircles[ 1u ].angle = float( m_mouseState.y ) / 500.0f;

		for( Circle& circle : m_mainCircles )
		{
			circle.position.x = renderSize.x * -0.005f;

			updateCircle( circle, time );
		}

		for( MenuEntry& entry : m_menuEntries )
		{
			for( Circle& circle : entry )
			{
				updateCircle( circle, time );
			}
		}
	}

	void MenuState::render( GraphicsContext& graphicsContext )
	{
		Renderer2d& renderer				= m_pParentState->getRenderer();

		const MechanicaMenuBundle& bundle	= m_pBundle->getData();
		const Vector2 renderSize			= renderer.getVectorSize();
		const Vector2 backgroundSize		= bundle.background->getTextureData().getVectorSize();

		const Vector2 diff = vector::div( vector::create( renderSize ), backgroundSize );
		const Vector2 extends = vector::scale( vector::create( backgroundSize ), (diff.x > diff.y ? diff.x : diff.y) );

		const AxisAlignedRectangle backgroundRectangle = createAxisAlignedRectangleCentered(
			Vector2::zero,
			extends
		);
		renderer.queueSprite( bundle.background->getTextureData(), backgroundRectangle, MechanicaRenderLayer_BackgroundIslands );

		for( Circle& circle : m_mainCircles )
		{
			renderCircle( renderer, circle );
		}

		for( MenuEntry& entry : m_menuEntries )
		{
			for( Circle& circle : entry )
			{
				renderCircle( renderer, circle );
			}
		}
	}

	bool MenuState::processInputEvent( const InputEvent& inputEvent )
	{
		if( inputEvent.eventType == InputEventType_Mouse_Moved )
		{
			createUint2( m_mouseState, inputEvent.data.mouseMoved.xState, inputEvent.data.mouseMoved.yState );
		}

		return false;
	}

	void MenuState::createMainCircles()
	{
		const MechanicaMenuBundle& bundle = m_pBundle->getData();

		Circle& circle0 = m_mainCircles.push();
		memory::zero( circle0 );
		circle0.pTexture = &bundle.circleBig.texture0->getTextureData();

		Circle& circle1 = m_mainCircles.push();
		memory::zero( circle1 );
		circle1.pTexture = &bundle.circleBig.texture1->getTextureData();

		Circle& circle2 = m_mainCircles.push();
		memory::zero( circle2 );
		circle2.pTexture = &bundle.circleBig.texture2->getTextureData();
		circle2.angleSpeed = 0.2f;

		Circle& circleText = m_mainCircles.push();
		memory::zero( circleText );
		circleText.pTexture		= &bundle.circleBigText->getTextureData();
		circleText.angleMin		= 0.1f;
		circleText.angleMax		= 0.95f;
		circleText.angleSpeed	= 0.0f;
	}

	void MenuState::createMenuCircles( const ResArray< MenuElement >& elements )
	{
		const MechanicaMenuBundle& bundle = m_pBundle->getData();

		RandomNumberGenerator random;
		random.createFromTime();

		m_menuEntries.clear();

		for( const MenuElement& element : elements.getView() )
		{
			MenuEntry& entry = m_menuEntries.push();

			for( uint i = 0u; i < 3u; ++i )
			{
				Circle& circle = entry[ i ];
				memory::zero( circle );

				const uint64 angleType = random.nextUniformUInt64( 0u, 1u );
				const uint64 textureIndex = random.nextUniformUInt64( 0u, bundle.circleSmall.getCount() );

				switch( i )
				{
				case 0u:
					circle.pTexture = &bundle.circleSmall[ textureIndex ].texture0->getTextureData();
					break;

				case 1u:
					circle.pTexture = &bundle.circleSmall[ textureIndex ].texture1->getTextureData();
					break;

				case 2u:
					circle.pTexture = &bundle.circleSmall[ textureIndex ].texture2->getTextureData();
					break;

				default:
					break;
				}

				if( angleType == 0u )
				{
					circle.angleSpeed	= random.nextUniformFloat32( -0.001f, 0.001f );
				}
				else
				{
					circle.angleMin		= random.nextNormalizedFloat32();
					circle.angleMax		= random.nextNormalizedFloat32();
				}
			}

			Circle& circleIcon = entry[ 3u ];
			memory::zero( circleIcon );
			circleIcon.pTexture			= &element.texture->getTextureData();

			Circle& circleIconHover = entry[ 4u ];
			memory::zero( circleIconHover );
			circleIconHover.pTexture	= &element.textureHover->getTextureData();

			Circle& circleText = entry[ 5u ];
			memory::zero( circleText );
			circleText.pTexture			= &element.textureText->getTextureData();
			circleText.angleMin			= -0.4f;
			circleText.angleMax			= 0.1f;
		}
	}

	void MenuState::updateCircle( Circle& circle, GameTime time )
	{
		if( circle.angleMin != 0 || circle.angleMax != 0 )
		{
			const float t = (float)f64::sin( f64::mod( time.totalTime, f64::twoPi ) );
			circle.angle = circle.angleMin + ((circle.angleMax - circle.angleMin) * t);
		}
		else
		{
			circle.angle += float( circle.angleSpeed * time.elapsedTime );
		}
	}

	void MenuState::renderCircle( Renderer2d& renderer, const Circle& circle )
	{
		Matrix32 rotationMatrix;
		rotationMatrix.pos = Vector2::zero;
		matrix::createRotationZ( rotationMatrix.rot, circle.angle );

		Matrix32 translationMatrix;
		translationMatrix.pos = circle.position;
		matrix::createIdentity( translationMatrix.rot );

		Matrix32 matrix;
		matrix::mul( matrix, rotationMatrix, translationMatrix );

		renderer.queueSprite( *circle.pTexture, matrix, MechanicaRenderLayer_UI );
	}
}