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

		Vector2 mousePosition = vector::create( float( m_mouseState.x ), float( m_mouseState.y ) );
		vector::sub( mousePosition, vector::scale( vector::create( renderSize ), 0.5f ) );
		vector::scale( mousePosition, 0.01f );

		m_mainCircles[ 0u ].angle = float( m_mouseState.x ) / 500.0f;
		m_mainCircles[ 1u ].angle = float( m_mouseState.y ) / 500.0f;

		m_alpha -= (float)time.elapsedTime;
		m_alpha = TIKI_MAX( m_alpha, 0.0f );

		for( Circle& circle : m_mainCircles )
		{
			circle.position.x = renderSize.x * -0.005f;

			updateCircle( circle, time );
		}

		for( uint i = 0u; i < m_menuEntries.getCount(); ++i )
		{
			MenuEntry& menuEntry		= m_menuEntries[ i ];
			Circle& menuCircle			= menuEntry.circles.getFirst();
			Circle& connectionCircle	= m_connectionCircles[ i ];

			connectionCircle.position.x = renderSize.x * -0.005f;

			const float angleTime = TIKI_MAX( 0.0f, m_alpha - 0.5f );
			const float a = f32::pi * (menuEntry.angle - angleTime);

			Vector2 entryPosition = vector::create( f32::sin( a ), f32::cos( a ) );
			vector::scale( entryPosition, 5.0f );
			vector::add( entryPosition, connectionCircle.position );
			for( Circle& circle : menuEntry.circles )
			{
				circle.position = entryPosition;
			}

			const Vector2 diff = vector::sub( vector::create( connectionCircle.position ), menuCircle.position );
			const float dot = f32::atan2( diff.x, diff.y ) - (f32::pi * 0.5f);
			connectionCircle.angle = dot + f32::pi;

			float distance = vector::distance( menuCircle.position, mousePosition ) - 0.5f;
			distance = f32::clamp( distance, 0.0f, 1.0f );

			menuEntry.circles[ 3u ].alpha = distance;
			menuEntry.circles[ 4u ].alpha = 1.0f - distance;

			for( Circle& circle : menuEntry.circles )
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

		for( Circle& circle : m_connectionCircles )
		{
			renderCircle( renderer, circle );
		}

		for( MenuEntry& entry : m_menuEntries )
		{
			for( Circle& circle : entry.circles )
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

	void MenuState::clearCircle( Circle& circle )
	{
		memory::zero( circle );

		circle.scale	= 1.0f;
		circle.alpha	= 1.0f;
	}

	void MenuState::createMainCircles()
	{
		const MechanicaMenuBundle& bundle = m_pBundle->getData();

		Circle& circle0 = m_mainCircles.push();
		clearCircle( circle0 );
		circle0.pTexture = &bundle.circleBig.texture0->getTextureData();

		Circle& circle1 = m_mainCircles.push();
		clearCircle( circle1 );
		circle1.pTexture = &bundle.circleBig.texture1->getTextureData();

		Circle& circle2 = m_mainCircles.push();
		clearCircle( circle2 );
		circle2.pTexture = &bundle.circleBig.texture2->getTextureData();
		circle2.angleSpeed = 0.2f;

		Circle& circleText = m_mainCircles.push();
		clearCircle( circleText );
		circleText.pTexture		= &bundle.circleBigText->getTextureData();
		circleText.angleMin		= 0.1f;
		circleText.angleMax		= 0.95f;
		circleText.angleSpeed	= 0.0f;
	}

	void MenuState::createMenuCircles( const ResArray< MenuElement >& elements )
	{
		const MechanicaMenuBundle& bundle = m_pBundle->getData();

		RandomNumberGenerator random;
		random.createFromSeed( (uint64)&elements );

		m_alpha = 1.0f;

		m_menuEntries.clear();
		for( uint i = 0u; i < elements.getCount(); ++i )
		{
			const MenuElement& element = elements[ i ];
			MenuEntry& entry = m_menuEntries.push();

			entry.angle = f32::lerp( 0.75f, 0.25f, 1.0f / (elements.getCount() - 1u) * i );

			for( uint i = 0u; i < 3u; ++i )
			{
				Circle& circle = entry.circles[ i ];
				clearCircle( circle );

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
					circle.angleSpeed	= random.nextUniformFloat32( -1.0f, 1.0f );
				}
				else
				{
					circle.angleMin		= random.nextNormalizedFloat32();
					circle.angleMax		= random.nextNormalizedFloat32();
				}

				circle.scale = 0.75f;
			}

			Circle& circleIcon = entry.circles[ 3u ];
			clearCircle( circleIcon );
			circleIcon.pTexture			= &element.texture->getTextureData();
			circleIcon.scale			= 0.75f;

			Circle& circleIconHover = entry.circles[ 4u ];
			clearCircle( circleIconHover );
			circleIconHover.pTexture	= &element.textureHover->getTextureData();
			circleIconHover.scale		= 0.75f;
			circleIconHover.alpha		= 0.0f;

			Circle& circleText = entry.circles[ 5u ];
			clearCircle( circleText );
			circleText.pTexture			= &element.textureText->getTextureData();
			circleText.angleMin			= -0.4f;
			circleText.angleMax			= 0.1f;
			circleText.scale			= 0.75f;
		}

		createConnectionCircles( elements.getCount() );
	}

	void MenuState::createConnectionCircles( uint count )
	{
		const MechanicaMenuBundle& bundle = m_pBundle->getData();

		m_connectionCircles.clear();
		for( uint i = 0u; i < count; ++i )
		{
			Circle& circle = m_connectionCircles.push();
			clearCircle( circle );

			circle.pTexture = &bundle.menuConnection->getTextureData();
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
		Matrix32 scaleMatrix;
		scaleMatrix.pos = Vector2::zero;
		matrix::createScale( scaleMatrix.rot, vector::create( circle.scale, circle.scale ) );

		Matrix32 rotationMatrix;
		rotationMatrix.pos = Vector2::zero;
		matrix::createRotationZ( rotationMatrix.rot, circle.angle );

		Matrix32 translationMatrix;
		translationMatrix.pos = circle.position;
		matrix::createIdentity( translationMatrix.rot );

		Matrix32 matrix;
		matrix::mul( matrix, scaleMatrix, rotationMatrix );
		matrix::mul( matrix, translationMatrix );

		const Color color = color::fromFloatRGBA( 1.0f, 1.0f, 1.0f, circle.alpha );
		renderer.queueSprite( *circle.pTexture, matrix, MechanicaRenderLayer_UI, color );
	}
}