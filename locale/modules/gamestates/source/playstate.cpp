
#include "tiki/gamestates/playstate.hpp"

#include "tiki/graphics/staticmodelcomponent.hpp"
#include "tiki/graphics/skinnedmodelcomponent.hpp"
#include "tiki/game/gd.hpp"
#include "tiki/game/buffsystem.hpp"
#include "tiki/game/projectilesystem.hpp"
#include "tiki/game/gamedata.hpp"
#include "tiki/gameplay/pathfinding.hpp"
#include "tiki/gameplay/navmesh.hpp"
#include "tiki/graphics/level.hpp"
#include "tiki/graphics/model.hpp"
#include "tiki/game/budgetsystem.hpp"
#include "tiki/game/itemattachment.hpp"
#include "tiki/gamecore/levelcollider.hpp"
#include "tiki\game\collectablesystem.hpp"
#include "tiki/game/gamehud.hpp"
#include "tiki/game/gamecamera.hpp"
#include "tiki/gamecore/camera.hpp"
#include "tiki/game/playerentity.hpp"
#include "tiki\game\collectablelifeleech.hpp"
#include "tiki/graphics/directionallight.hpp"
#include "tiki/game/aicontroller.hpp"

namespace tiki
{
	void PlayState::create( ApplicationState* pParentState )
	{
		m_pParentState	= pParentState;
		//hi, das ist nur wegen lukas!!!
	}

	void PlayState::dispose()
	{
		m_pParentState = nullptr;
	}

	TransitionState PlayState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		switch ( currentStep )
		{
		case PlayStateTransitionSteps_InitializeBase:
			TIKI_ASSERT( isInital );

			if ( isCreating )
			{
				GD::dataInitFunction();

				BuffSystem::init();
				ProjectileSystem::init();
				BudgetSystem::init();
				CollectableSystem::init();
				GameHud::create();

				Projectile::count = 0;

				m_enemyDeath = false;
				m_debugCamera = false;
			}
			else
			{
				BuffSystem::dispose();
				ProjectileSystem::dispose();
				BudgetSystem::dispose();
				CollectableSystem::dispose();
				GameHud::dispose();
			}

			return TransitionState_Finish;
		case PlayStateTransitionSteps_LoadResources:
			if ( isCreating )
			{
				TIKI_ASSERT( isInital );

				ResourceManager& content	= framework::getResourceManager();
				m_pBaseLevelCollider		= content.loadResource< LevelCollider >( "arenabase.collision" );
				m_pBaseLevel				= content.loadResource< Level >( "ultraarena.level" );

				m_pBaseLevelCollider->hideCollider();

				TIKI_ASSERT( m_pBaseLevelCollider );
				TIKI_ASSERT( m_pBaseLevel );

				//CollectableLifeLeech* st = new CollectableLifeLeech();
				//st->init(Vector3(50,20,50));
				//CollectableSystem::addCollectable(st);

				return TransitionState_Finish;
			}
			else
			{
				ResourceManager& content = framework::getResourceManager();

				content.unloadResource( m_pBaseLevel );
				content.unloadResource( m_pBaseLevelCollider );
				return TransitionState_Finish;
			}
			break;
		case PlayStateTransitionSteps_CreateComponents:
			if ( isCreating )
			{
				TIKI_ASSERT( isInital );

				return TransitionState_Finish;
			}
			else
			{

				return TransitionState_Finish;
			}
			break;
		case PlayStateTransitionSteps_InitializeComponents:
			if ( isCreating )
			{

				return TransitionState_Finish;
			}
			else
			{
				return TransitionState_Finish;
			}
			break;
		case PlayStateTransitionSteps_CreateNavMesh:
			if ( isCreating )
			{
				ResourceManager& content = framework::getResourceManager();

				m_pLevelCollider	= content.loadResource< LevelCollider >( "testlevel.collision" );
				m_pLevel			= content.loadResource< Level >( "testlevel.level" );
				m_pNavMesh			= content.loadResource< NavMesh >( "testlevel.navmesh" );

			/*
					Level Auswahl

					level00 =	Standard Ebene, keine Hindernisse
					level01 =	Verengung um den Spielerstartpunkt
					level02 =	Spieler mittig und durch Navmesh-Unterbrechung isoliert; 
								Unterbrechung durch Objekte angedeutet
					level03 =	Gegner starten auf Feldern ohne Navmesh (S,W logischerweise nicht sichtbar)
								oder in einer abgegrenzten Area (N,O durch gesetzte Objekte angedeutet)
					level04 =	Mini-Labyrinth
				
				testlevel= fertiges Level, ready to be played
			*/


				//m_pNavMesh		= content.loadResource< NavMesh >( "arenabase.navmesh" );

				m_pLevelCollider->hideCollider();

				TIKI_ASSERT( m_pLevelCollider );
				TIKI_ASSERT( m_pLevel );
				TIKI_ASSERT( m_pNavMesh );

				Pathfinding::init( m_pNavMesh->getNavMeshQuery() );

				return TransitionState_Finish;
			}
			else
			{
				ResourceManager& content = framework::getResourceManager();

				content.unloadResource( m_pLevelCollider );
				content.unloadResource( m_pLevel );
				content.unloadResource( m_pNavMesh );

				return TransitionState_Finish;
			}
			break;
		case PlayStateTransitionSteps_CreateEntities:
			if ( isCreating )
			{
				playerModel = framework::getResourceManager().loadResource< Model >( "player.model" );

				TIKI_ASSERT( playerModel );

				player.init( Vector3( 0, 25, 0 ), playerModel );
				playerController.init( &player, 30, 5 );

				gameData.playerController = &playerController;
				gameData.player = &player;
				gameData.playerAlive = true;

				BudgetSystem::createEnemiesForLvl(GD::GameLvl);

				m_gameCamera.initialize( Vector3( 5, 2, -10 ), player.getTransform() );

				m_pDirectionalLight	= DirectionalLight::create();
				m_pDirectionalLight->initialize( Vector3(-1) );


				m_freeCamera.initialize();
				Camera::setMainCamera( m_gameCamera.getCamera() );

				return TransitionState_Finish;
			}
			else
			{
				m_gameCamera.dispose();
				m_freeCamera.dispose();

				m_pDirectionalLight->dispose();

				if(gameData.playerAlive)
				{
					player.dispose();
					playerController.dispose();
				}

				framework::getResourceManager().unloadResource< Model >( playerModel );

				return TransitionState_Finish;
			}
			break;
		case PlayStateTransitionSteps_CreateLevel:
			if ( isCreating )
			{
				TIKI_ASSERT( isInital );
				return TransitionState_Finish;
			}
			else
			{
				return TransitionState_Finish;
			}
			break;
		default:
			break;
		}

		return TransitionState_Error;
	}

	void PlayState::update()
	{
		InputSystem& input = framework::getInputSystem();

		Transform::update();

		//if( input.isKeyPressed( KEY_F1 ) )
		//{
		//	m_debugCamera = !m_debugCamera;

		//	if( m_debugCamera )
		//		Camera::setMainCamera( m_freeCamera.getCamera() );
		//	else
		//		Camera::setMainCamera( m_gameCamera.getCamera() );
		//}

		if( input.isKeyPressed( KEY_L ) )
		{
			if( gameData.playerAlive )
			{
				player.dispose();
				playerController.dispose();
			}
			gameData.playerAlive = true;
			player.init( Vector3( 0, 25, 0 ), playerModel );
			playerController.init( &player, 30, 5 );
			playerController.setCollectable(nullptr);
			m_gameCamera.setTraget( player.getTransform() );
			BudgetSystem::clearEnemies();
			BudgetSystem::createEnemiesForLvl(1);
			CollectableSystem::clear();
			ProjectileSystem::clear();
			gameData.score = 0;
		}

		if( m_debugCamera )
			m_freeCamera.update();
		else
			m_gameCamera.update();

		Camera::updateMainCamera();

		BudgetSystem::update();

		if(gameData.playerAlive)
		{
			if(player.getHealth() <= 0)
			{
				gameData.playerAlive = false;
				player.dispose();
				playerController.dispose();
			}
			else
			{
				player.update();
				playerController.update();
			}
		}

		BuffSystem::update();
		ProjectileSystem::update();
		CollectableSystem::update();
		ItemAttachment::update();
	}

	void PlayState::render( GpuContext* pCommandWriter ) const
	{
		StaticModelComponent::render( pCommandWriter );
		SkinnedModelComponent::render( pCommandWriter );

		for ( std::vector<EnemyData*>::iterator it = gameData.enemies->begin() ; it != gameData.enemies->end(); ++it )
		{
			EnemyData* ed = (*it);
			ed->controller->debugRender(pCommandWriter);
		}

		GameHud::drawLifeBars( pCommandWriter );
		GameHud::drawPlayerLife( pCommandWriter );
		GameHud::drawCurrentLevel( pCommandWriter );
		GameHud::drawAnzCharges( pCommandWriter );
	}
}