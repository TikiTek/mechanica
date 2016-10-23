#include "tiki/physics2dcomponents/physics2dcollidercomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/physics2d/physics2dcollider.hpp"
#include "tiki/physics2d/physics2dworld.hpp"

#include "physics2dcomponents_shared.hpp"

namespace tiki
{
	struct Physics2dColliderComponentState : public ComponentState
	{
		Physics2dComponentShape	shape;

		Physics2dCollider		collider;
	};
	TIKI_COMPONENT_STATE_CONSTRUCT_FUNCTIONS( Physics2dColliderComponentState );

	Physics2dColliderComponent::Physics2dColliderComponent()
		: Component( Physics2dComponentType_Collider, "Physics2dColliderComponent", sizeof( Physics2dColliderComponentState ), true )
	{
		m_pPhysicsWorld = nullptr;
	}

	Physics2dColliderComponent::~Physics2dColliderComponent()
	{
		TIKI_ASSERT( m_pPhysicsWorld == nullptr );
	}

	bool Physics2dColliderComponent::create( Physics2dWorld& physicsWorld )
	{
		m_pPhysicsWorld = &physicsWorld;

		return true;
	}

	void Physics2dColliderComponent::dispose()
	{
		m_pPhysicsWorld = nullptr;
	}

	const Physics2dCollisionObject& Physics2dColliderComponent::getPhysicsObject( const Physics2dColliderComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		return pState->collider;
	}

	bool Physics2dColliderComponent::internalInitializeState( ComponentEntityIterator& componentIterator, Physics2dColliderComponentState* pState, const Physics2dColliderComponentInitData* pInitData )
	{
		TIKI_ASSERT( m_pPhysicsWorld != nullptr );

		Physics2dShape* pShape = createPhysics2dComponentShape( pState->shape, pInitData->shape );
		if ( pShape == nullptr )
		{
			return false;
		}

		return pState->collider.create( *m_pPhysicsWorld, *pShape, vector::create( pInitData->position ), pInitData->materialId );
	}

	void Physics2dColliderComponent::internalDisposeState( Physics2dColliderComponentState* pState )
	{
		TIKI_ASSERT( m_pPhysicsWorld != nullptr );

		pState->collider.dispose( *m_pPhysicsWorld );

		disposePhysics2dComponentShape( pState->shape );
	}
}