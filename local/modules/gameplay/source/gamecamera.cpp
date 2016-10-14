
#include "tiki/gameplay/gamecamera.hpp"

#include "tiki/base/debugprop.hpp"
#include "tiki/components3d/terraincomponent.hpp"
#include "tiki/components3d/transform3dcomponent.hpp"
#include "tiki/math/camera.hpp"

namespace tiki
{
	TIKI_DEBUGPROP_FLOAT( s_cameraAltitude, "GameCamera/CameraAltitude", 15.0f, 1.0f, 100.0f );
	TIKI_DEBUGPROP_FLOAT( s_cameraDistanceZ, "GameCamera/CameraDistanceZ", 8.0f, 0.0f, 100.0f );
	TIKI_DEBUGPROP_FLOAT( s_cameraXAngle, "GameCamera/CameraXAngle", f32::piOver4, 0.0f, f32::twoPi );
	TIKI_DEBUGPROP_FLOAT( s_cameraSpeedZ, "GameCamera/SpeedZ", 2.0f, 0.0f, 100.0f );

	GameCamera::GameCamera()
	{		
		m_pTerrainComponent = nullptr;

		vector::clear( m_cameraPosition );
	}

	GameCamera::~GameCamera()
	{
		TIKI_ASSERT( m_pTerrainComponent == nullptr );
	}

	void GameCamera::create( const TerrainComponent& terrainComponent, const Vector3& cameraPosition )
	{
		m_pTerrainComponent = &terrainComponent;

		m_cameraPosition = cameraPosition;
	}

	void GameCamera::dispose()
	{
		m_pTerrainComponent = nullptr;
	}

	void GameCamera::update( Camera& targetCamera, const TerrainComponentState* pTerrainComponentState, float deltaTime )
	{
		//m_cameraPosition.z += s_cameraSpeedZ * deltaTime;
		m_cameraPosition.y = m_pTerrainComponent->getHeightAtPosition( pTerrainComponentState, vector::create( m_cameraPosition.x, m_cameraPosition.x ) ) + s_cameraAltitude;
		//position.z = s_cameraDistanceZ;

		Vector3 lookAt = m_cameraPosition;
		lookAt.y += 1.0f;

		Quaternion rotation;
		quaternion::createLookAt( rotation, m_cameraPosition, lookAt );

		Quaternion angleRotation;
		quaternion::fromYawPitchRoll( angleRotation, 0.0f, -s_cameraXAngle, 0.0f );

		quaternion::mul( rotation, angleRotation );

		targetCamera.setTransform( m_cameraPosition, rotation );
	}
}