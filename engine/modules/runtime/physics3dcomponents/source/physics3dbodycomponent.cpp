#include "tiki/physics3dcomponents/physics3dbodycomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components3d/transform3dcomponent.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/physics3d/physics3dbody.hpp"
#include "tiki/physics3d/physics3dboxshape.hpp"
#include "tiki/physics3d/physics3dcapsuleshape.hpp"
#include "tiki/physics3d/physics3dsphereshape.hpp"
#include "tiki/physics3d/physics3dworld.hpp"

#include "physics3dcomponents_shared.hpp"

#include "components.hpp"

namespace tiki
{
	struct Physics3dBodyComponentState : public ComponentState
	{
		Transform3dComponentState*	pTransform;

		Physics3dComponentShape		shape;

		Physics3dBody				body;
	};

	Physics3dBodyComponent::Physics3dBodyComponent()
	{
		m_pPhysicsWorld			= nullptr;

		m_pTranformComponent	= nullptr;
	}

	Physics3dBodyComponent::~Physics3dBodyComponent()
	{
		TIKI_ASSERT( m_pPhysicsWorld		== nullptr );
		TIKI_ASSERT( m_pTranformComponent	== nullptr );
	}

	bool Physics3dBodyComponent::create( Physics3dWorld& physicsWorld, const Transform3dComponent& transformComponent )
	{
		m_pPhysicsWorld			= &physicsWorld;

		m_pTranformComponent	= &transformComponent;

		return true;
	}

	void Physics3dBodyComponent::dispose()
	{
		m_pPhysicsWorld			= nullptr;

		m_pTranformComponent	= nullptr;
	}

	void Physics3dBodyComponent::update()
	{
		ConstIterator componentStates = getConstIterator();

		const State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			Vector3 position;
			Quaternion rotation;
			pState->body.getPosition( position );
			pState->body.getRotation( rotation );

			m_pTranformComponent->setPosition( pState->pTransform, position );
			m_pTranformComponent->setRotation( pState->pTransform, rotation );
		}
	}

	void Physics3dBodyComponent::applyForce( Physics3dBodyComponentState* pState, const Vector3& force ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->body.applyForce( force );
	}

	void Physics3dBodyComponent::getPosition( Vector3& targetPosition, const Physics3dBodyComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->body.getPosition( targetPosition );
	}

	void Physics3dBodyComponent::getRotation( Quaternion& targetRotation, const Physics3dBodyComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->body.getRotation( targetRotation );
	}

	const Physics3dCollisionObject& Physics3dBodyComponent::getPhysicsObject( const Physics3dBodyComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		return pState->body;
	}

	crc32 Physics3dBodyComponent::getTypeCrc() const
	{
		return crcString( "Physics3dBodyComponent" );
	}

	uint32 Physics3dBodyComponent::getStateSize() const
	{
		return sizeof( Physics3dBodyComponentState );
	}

	const char* Physics3dBodyComponent::getTypeName() const
	{
		return "Physics3dBodyComponent";
	}

	bool Physics3dBodyComponent::internalInitializeState( ComponentEntityIterator& componentIterator, Physics3dBodyComponentState* pState, const Physics3dBodyComponentInitData* pInitData )
	{
		TIKI_ASSERT( m_pPhysicsWorld != nullptr );

		pState = new( pState ) Physics3dBodyComponentState;

		pState->pTransform = (Transform3dComponentState*)componentIterator.getFirstOfType( m_pTranformComponent->getTypeId() );
		if ( pState->pTransform == nullptr )
		{
			return false;
		}

		Physics3dShape* pShape = createPhysics3dComponentShape( pState->shape, pInitData->shape );
		if ( pShape == nullptr )
		{
			return false;
		}

		pState->body.create( *pShape, vector::create( pInitData->position ), pInitData->mass, pInitData->freeRotation );
		m_pPhysicsWorld->addBody( pState->body );

		return true;
	}

	void Physics3dBodyComponent::internalDisposeState( Physics3dBodyComponentState* pState )
	{
		TIKI_ASSERT( m_pPhysicsWorld != nullptr );

		pState->pTransform = nullptr;

		m_pPhysicsWorld->removeBody( pState->body );
		pState->body.dispose();

		disposePhysics3dComponentShape( pState->shape );

		pState->~Physics3dBodyComponentState();
	}
}
