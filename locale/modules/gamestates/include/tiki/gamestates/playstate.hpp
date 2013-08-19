#pragma once
#ifndef TIKI_PLAYSTATE_HPP__INCLUDED
#define TIKI_PLAYSTATE_HPP__INCLUDED

#include "tiki/gameflow/gamestate.hpp"

#include "tiki/game/playerentity.hpp"
#include "tiki/game/gamecamera.hpp"
#include "tiki/game/map.hpp"
#include "tiki/game/playercontroller.hpp"
#include "tiki/gamecore/freecamera.hpp"
#include "tiki\game\collectablestronghit.hpp"

namespace tiki
{
	class ApplicationState;
	class Level;
	class Model;
	class Shader;
	class Texture;
	class NavMesh;
	class LevelCollider;
	class DirectionalLight;

	enum PlayStateTransitionSteps
	{
		PlayStateTransitionSteps_InitializeBase,
		PlayStateTransitionSteps_LoadResources,
		PlayStateTransitionSteps_CreateComponents,
		PlayStateTransitionSteps_InitializeComponents,
		PlayStateTransitionSteps_CreateNavMesh,
		PlayStateTransitionSteps_CreateEntities,
		PlayStateTransitionSteps_CreateLevel,

		PlayStateTransitionSteps_Count
	};

	class PlayState : public GameState
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( PlayState );

	public:

		void							create( ApplicationState* pParentState );
		void							dispose();

		virtual TransitionState			processTransitionStep( size_t currentStep, bool isCreating, bool isInital );

		virtual void					update();
		virtual void					render( GpuContext* pCommandWriter ) const;

	private:
		
		ApplicationState*				m_pParentState;

		GameCamera						m_gameCamera;
		FreeCamera						m_freeCamera;

		std::vector<Vector3*>			path;

		PlayerEntity					player;
		PlayerController				playerController;
		const Model*					playerModel;

		CollectableStrongHit			collStrongHit;

		const LevelCollider*			m_pBaseLevelCollider;
		const Level*					m_pBaseLevel;

		const LevelCollider*			m_pLevelCollider;
		const Level*					m_pLevel;
		const NavMesh*					m_pNavMesh;

		DirectionalLight*				m_pDirectionalLight;


		bool							m_enemyDeath;

		bool							m_debugCamera;

	};
}

#endif // TIKI_PLAYSTATE_HPP__INCLUDED
