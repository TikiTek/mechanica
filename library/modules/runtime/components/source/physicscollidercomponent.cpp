
#include "tiki/components/physicscollidercomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/physics/physicsboxshape.hpp"
#include "tiki/physics/physicscapsuleshape.hpp"
#include "tiki/physics/physicscollider.hpp"
#include "tiki/physics/physicssphereshape.hpp"
#include "tiki/physics/physicsworld.hpp"

#include "physicscomponents_shared.hpp"

#include "components.hpp"

namespace tiki
{
	struct PhysicsColliderComponentState : public ComponentState
	{
		PhysicsComponentShape	shape;

		PhysicsCollider			collider;
	};

	PhysicsColliderComponent::PhysicsColliderComponent()
	{
		m_pWorld = nullptr;
	}

	PhysicsColliderComponent::~PhysicsColliderComponent()
	{
		TIKI_ASSERT( m_pWorld == nullptr );
	}

	bool PhysicsColliderComponent::create( PhysicsWorld& physicsWorld )
	{
		m_pWorld = &physicsWorld;

		return true;
	}

	void PhysicsColliderComponent::dispose()
	{
		m_pWorld = nullptr;
	}

	const PhysicsCollisionObject& PhysicsColliderComponent::getPhysicsObject( const PhysicsColliderComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		return pState->collider;
	}

	crc32 PhysicsColliderComponent::getTypeCrc() const
	{
		return crcString( "PhysicsColliderComponent" );
	}

	uint32 PhysicsColliderComponent::getStateSize() const
	{
		return sizeof( PhysicsColliderComponentState );
	}

	const char* PhysicsColliderComponent::getTypeName() const
	{
		return "PhysicsColliderComponent";
	}

	bool PhysicsColliderComponent::internalInitializeState( ComponentEntityIterator& componentIterator, PhysicsColliderComponentState* pState, const PhysicsColliderComponentInitData* pInitData )
	{
		TIKI_ASSERT( m_pWorld != nullptr );

		pState = new( pState ) PhysicsColliderComponentState;

		PhysicsShape* pShape = createPhysicsComponentShape( pState->shape, pInitData->shape );
		if ( pShape == nullptr )
		{
			return false;
		}

		pState->collider.create( *pShape, vector::create( pInitData->position ) );
		m_pWorld->addCollider( pState->collider );

		return true;
	}

	void PhysicsColliderComponent::internalDisposeState( PhysicsColliderComponentState* pState )
	{
		TIKI_ASSERT( m_pWorld != nullptr );

		m_pWorld->removeCollider( pState->collider );
		pState->collider.dispose();

		disposePhysicsComponentShape( pState->shape );

		pState->~PhysicsColliderComponentState();
	}
}
