
#include "tiki/gamestates/teststate.hpp"

#include "tiki/animation/animation.hpp"
#include "tiki/animation/animationcomponent.hpp"
#include "tiki/game/itemattachment.hpp"
#include "tiki/gamecore/transform.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/skinnedmodelcomponent.hpp"

namespace tiki
{
	void TestState::create( ApplicationState* pParentState )
	{
		m_pParentState	= pParentState;

		m_pFont		= nullptr;

		m_freeCamera.initialize();
	}

	void TestState::dispose()
	{
		m_pParentState = nullptr;

		m_freeCamera.dispose();
	}

	TransitionState TestState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		switch ( currentStep )
		{
		case TestStateTransitionSteps_LoadResources:
			if ( isCreating )
			{
				if ( isInital )
				{
					m_pFont				= framework::getResourceManager().loadResource< Font >( "dorcla.font" );
					TIKI_ASSERT( m_pFont != nullptr );

					//m_pModel		= framework::getResourceManager().load Resource< Model >( "char_rhaulu.model" );
					//m_pAnimation	= framework::getResourceManager().loadResource< Animation >( "char_rhaulu.anm" );

					m_pModel[ 0u ]		= framework::getResourceManager().loadResource< Model >( "enemy.model" );
					m_pModel[ 1u ]		= framework::getResourceManager().loadResource< Model >( "enemy.model" );
					m_pModel[ 2u ]		= framework::getResourceManager().loadResource< Model >( "enemy.model" );

					m_pAnimation[ 0u ]	= framework::getResourceManager().loadResource< Animation >( "enemy.attack.anm" );
					m_pAnimation[ 1u ]	= framework::getResourceManager().loadResource< Animation >( "enemy.casting.anm" );
					m_pAnimation[ 2u ] 	= framework::getResourceManager().loadResource< Animation >( "enemy.shielddash.anm" );

					m_pHelm				= framework::getResourceManager().loadResource< Model >( "shield.model" );
					m_pShoulderL		= framework::getResourceManager().loadResource< Model >( "expbarrel.model" );
					m_pShoulderR		= framework::getResourceManager().loadResource< Model >( "mace.model" );

					TIKI_ASSERT( m_pHelm );
					TIKI_ASSERT( m_pShoulderL );
					TIKI_ASSERT( m_pShoulderR );

					TIKI_ASSERT( m_pAnimation[ 0u ] );
					TIKI_ASSERT( m_pAnimation[ 1u ] );
					TIKI_ASSERT( m_pAnimation[ 2u ] );

					return TransitionState_Finish;
				}
				else
				{
					// todo: make resource manager async

					return TransitionState_Error;
				}
			}
			else
			{
				TIKI_ASSERT( isInital );

				framework::getResourceManager().unloadResource( m_pFont );

				for (size_t i = 0u; i < TIKI_COUNT( m_pAnimation ); ++i)
				{
					framework::getResourceManager().unloadResource< Model >( m_pModel[ i ] );
					m_pModel[ i ] = nullptr;
				}
				
				for (size_t i = 0u; i < TIKI_COUNT( m_pAnimation ); ++i)
				{
					framework::getResourceManager().unloadResource< Animation >( m_pAnimation[ i ] );
					m_pAnimation[ i ] = nullptr;
				}

				framework::getResourceManager().unloadResource( m_pHelm );
				framework::getResourceManager().unloadResource( m_pShoulderL );
				framework::getResourceManager().unloadResource( m_pShoulderR );

				return TransitionState_Finish;
			}
			break;
		case TestStateTransitionSteps_CreateConponents:
			if ( isCreating )
			{
				TIKI_ASSERT( isInital );

				for (size_t i = 0u; i < TIKI_COUNT( m_pTransform ); ++i)
				{
					m_pTransform[ i ]			= Transform::create();
					m_pModelComponent[ i ]		= SkinnedModelComponent::create();
					m_pAnimationComponent[ i ]	= AnimationComponent::create();
				}

				for( uint i = 0; i < testCount * 3; ++i )
				{
					m_pAttTrans[ i ]			= Transform::create();
					m_pAttModel[ i ]			= StaticModelComponent::create();
					m_pAtt[ i ]					= ItemAttachment::create();
				}


				/*m_pTransform2			= Transform::create();
				m_pStaticModel			= StaticModelComponent::create();
				m_pAttachment			= ItemAttachment::create();*/

				return TransitionState_Finish;
			}
			else
			{
				TIKI_ASSERT( isInital );

				for (size_t i = 0u; i < TIKI_COUNT( m_pTransform ); ++i)
				{
					m_pTransform[ i ]->dispose();
					m_pModelComponent[ i ]->dispose();
					m_pAnimationComponent[ i ]->dispose();
				}

				for( uint i = 0; i < testCount * 3; ++i )
				{
					m_pAttTrans[ i ]->dispose();	
					m_pAttModel[ i ]->dispose();	
					m_pAtt[ i ]->dispose();	
				}

				return TransitionState_Finish;
			}
			break;
		case TestStateTransitionSteps_InitializeConponents:
			if ( isCreating )
			{
				TIKI_ASSERT( isInital );

				for (size_t i = 0u; i < TIKI_COUNT( m_pTransform ); ++i)
				{
					Vector3 pos;
					pos.x = ( i * 30.0f );

					m_pTransform[ i ]->initialize( pos );
					m_pModelComponent[ i ]->initialize( m_pTransform[ i ], m_pModel[ i ] );
					m_pAnimationComponent[ i ]->initialize( m_pAnimation[ i ], m_pModelComponent[ i ] );
				}

				const Material* pMaterial = framework::getResourceManager().loadResource< Material >( "enemy.mage.material" );
				m_pModelComponent[ 1 ]->setMaterial( pMaterial );
				framework::getResourceManager().unloadResource( pMaterial);

				for( uint i = 0; i < testCount * 3; ++i )
				{
					m_pAttTrans[ i ]->initialize();
				}

				for( uint i = 0; i < testCount; ++i )
				{
					m_pAttModel[ i	   ]->initialize( m_pAttTrans[ i     ], m_pHelm );
					m_pAttModel[ i + 3 ]->initialize( m_pAttTrans[ i + 3 ], m_pShoulderL );
					m_pAttModel[ i + 6 ]->initialize( m_pAttTrans[ i + 6 ], m_pShoulderR );

					m_pAtt[ i     ]->initialize( m_pModelComponent[ i ], 20,  m_pAttTrans[ i     ] );
					m_pAtt[ i + 3 ]->initialize( m_pModelComponent[ i ], 3,  m_pAttTrans[ i + 3 ] );
					m_pAtt[ i + 6 ]->initialize( m_pModelComponent[ i ], 31,  m_pAttTrans[ i + 6 ] );
				}


				// left		shoulder		7
				// right	shoulder		23

				// head						5

				// right	hand			31
				// left		hand			20

				// back						3

				Camera::setMainCamera( m_freeCamera.getCamera() );

				return TransitionState_Finish;
			}
			else
			{
				TIKI_ASSERT( isInital );
				return TransitionState_Finish;
			}
			break;
		default:
			break;
		}

		TIKI_BREAK( "no result\n" );
		return TransitionState_Error;
	}

	void TestState::update()
	{
		float elapsedTime = (float)framework::getTimeSystem().getElapsedTime();// * 0.25f;

		InputSystem& input = framework::getInputSystem();

		m_freeCamera.update();

		Transform::update();

		AnimationComponent::update( elapsedTime );
		ItemAttachment::update();

		Camera::updateMainCamera();
	}

	void TestState::render( GpuContext* pCommandWriter ) const
	{
		StaticModelComponent::render( pCommandWriter );
		SkinnedModelComponent::render( pCommandWriter );

		pCommandWriter->drawText( Vector2( 50.0f, 50.0f ), *m_pFont, "This is a Test" );
	}
}