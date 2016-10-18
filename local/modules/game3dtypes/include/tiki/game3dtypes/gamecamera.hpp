#pragma once
#ifndef __TIKI_PLAYERCAMERA_HPP_INCLUDED__
#define __TIKI_PLAYERCAMERA_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/components/component_types.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	class Camera;
	class TerrainComponent;
	struct TerrainComponentState;

	class GameCamera
	{
		TIKI_NONCOPYABLE_CLASS( GameCamera );

	public:

								GameCamera();
								~GameCamera();

		void					create( const TerrainComponent& terrainComponent, const Vector3& cameraPosition );
		void					dispose();

		void					update( Camera& targetCamera, const TerrainComponentState* pTerrainComponentState, float deltaTime );

		const Vector3&			getCameraPosition() const { return m_cameraPosition; }
		
	private:

		const TerrainComponent*	m_pTerrainComponent;

		Vector3					m_cameraPosition;

	};
}

#endif // __TIKI_PLAYERCAMERA_HPP_INCLUDED__
