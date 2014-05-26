
#include "tiki/gamecomponents/coincomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components/lifetimecomponent.hpp"
#include "tiki/components/physicsbodycomponent.hpp"
#include "tiki/components/transformcomponent.hpp"
#include "tiki/gamecomponents/coincomponent_initdata.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/physics/physicsworld.hpp"

namespace tiki
{
	struct CoinComponentState : public ComponentState
	{
		TransformComponentState*	pTransform;
		LifeTimeComponentState*		pLifeTime;
		PhysicsBodyComponentState*	pPhysicsBody;

		bool						collected;
		float						value;
	};

	CoinComponent::CoinComponent()
	{
		m_pTransformComponent	= nullptr;
		m_pPhysicsBodyComponent	= nullptr;
		m_pLifeTimeComponent	= nullptr;

		m_pPhysicsWorld			= nullptr;
	}

	CoinComponent::~CoinComponent()
	{
		TIKI_ASSERT( m_pTransformComponent		== nullptr );
		TIKI_ASSERT( m_pPhysicsBodyComponent	== nullptr );
		TIKI_ASSERT( m_pLifeTimeComponent		== nullptr );

		TIKI_ASSERT( m_pPhysicsWorld			== nullptr );
	}

	bool CoinComponent::create( const TransformComponent& transformComponent, const PhysicsBodyComponent& physicsBodyComponent, const LifeTimeComponent& lifeTimeComponent, const PhysicsWorld& physicsWorld )
	{
		m_pTransformComponent	= &transformComponent;
		m_pPhysicsBodyComponent	= &physicsBodyComponent;
		m_pLifeTimeComponent	= &lifeTimeComponent;

		m_pPhysicsWorld			= &physicsWorld;

		return true;
	}

	void CoinComponent::dispose()
	{
		m_pTransformComponent	= nullptr;
		m_pPhysicsBodyComponent	= nullptr;
		m_pLifeTimeComponent	= nullptr;

		m_pPhysicsWorld			= nullptr;
	}

	void CoinComponent::update( const PhysicsCollisionObject& playerCollider, CollectedCoinIdArray& collectedCoins, float totalGameTime )
	{
		Iterator componentStates = getIterator();

		State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			Quaternion rotation;
			quaternion::fromYawPitchRoll( rotation, totalGameTime, 0.0f, 0.0f );

			m_pTransformComponent->setRotation( pState->pTransform, rotation );

			if ( m_pPhysicsWorld->checkIntersection( playerCollider, m_pPhysicsBodyComponent->getPhysicsObject( pState->pPhysicsBody ) ) )
			{
				collectedCoins.push( pState->entityId );
			}
		}
	}

	crc32 CoinComponent::getTypeCrc() const
	{
		return crcString( "CoinComponent" );
	}

	uint32 CoinComponent::getStateSize() const
	{
		return sizeof( CoinComponentState );
	}

	const char* CoinComponent::getTypeName() const
	{
		return "CoinComponent";
	}

	bool CoinComponent::internalInitializeState( ComponentEntityIterator& componentIterator, CoinComponentState* pState, const CoinComponentInitData* pInitData )
	{
		pState->pTransform = (TransformComponentState*)componentIterator.getFirstOfType( m_pTransformComponent->getTypeId() );
		pState->pPhysicsBody = (PhysicsBodyComponentState*)componentIterator.getFirstOfType( m_pPhysicsBodyComponent->getTypeId() );
		pState->pLifeTime = (LifeTimeComponentState*)componentIterator.getFirstOfType( m_pLifeTimeComponent->getTypeId() );

		if ( pState->pTransform == nullptr || pState->pPhysicsBody == nullptr || pState->pLifeTime == nullptr )
		{
			return false;
		}

		pState->value = pInitData->value;

		return true;
	}

	void CoinComponent::internalDisposeState( CoinComponentState* pState )
	{
		pState->pTransform		= nullptr;
		pState->pPhysicsBody	= nullptr;
		pState->pLifeTime		= nullptr;

		pState->value		= 0.0f;
	}
}