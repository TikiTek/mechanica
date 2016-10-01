#include "tiki/physics3dcomponents/physics3dcollidercomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/physics3d/physics3dboxshape.hpp"
#include "tiki/physics3d/physics3dcapsuleshape.hpp"
#include "tiki/physics3d/physics3dcollider.hpp"
#include "tiki/physics3d/physics3dsphereshape.hpp"
#include "tiki/physics3d/physics3dworld.hpp"

#include "physics3dcomponents_shared.hpp"

#include "components.hpp"

namespace tiki
{
	struct Physics3dColliderComponentState : public ComponentState
	{
		Physics3dComponentShape	shape;

		Physics3dCollider		collider;
	};

	Physics3dColliderComponent::Physics3dColliderComponent()
	{
		m_pWorld = nullptr;
	}

	Physics3dColliderComponent::~Physics3dColliderComponent()
	{
		TIKI_ASSERT( m_pWorld == nullptr );
	}

	bool Physics3dColliderComponent::create( Physics3dWorld& physicsWorld )
	{
		m_pWorld = &physicsWorld;

		return true;
	}

	void Physics3dColliderComponent::dispose()
	{
		m_pWorld = nullptr;
	}

	const Physics3dCollisionObject& Physics3dColliderComponent::getPhysicsObject( const Physics3dColliderComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		return pState->collider;
	}

	crc32 Physics3dColliderComponent::getTypeCrc() const
	{
		return crcString( "Physics3dColliderComponent" );
	}

	uint32 Physics3dColliderComponent::getStateSize() const
	{
		return sizeof( Physics3dColliderComponentState );
	}

	const char* Physics3dColliderComponent::getTypeName() const
	{
		return "Physics3dColliderComponent";
	}

	bool Physics3dColliderComponent::internalInitializeState( ComponentEntityIterator& componentIterator, Physics3dColliderComponentState* pState, const Physics3dColliderComponentInitData* pInitData )
	{
		TIKI_ASSERT( m_pWorld != nullptr );

		pState = new( pState ) Physics3dColliderComponentState;

		Physics3dShape* pShape = createPhysics3dComponentShape( pState->shape, pInitData->shape );
		if ( pShape == nullptr )
		{
			return false;
		}

		pState->collider.create( *pShape, vector::create( pInitData->position ) );
		m_pWorld->addCollider( pState->collider );

		return true;
	}

	void Physics3dColliderComponent::internalDisposeState( Physics3dColliderComponentState* pState )
	{
		TIKI_ASSERT( m_pWorld != nullptr );

		m_pWorld->removeCollider( pState->collider );
		pState->collider.dispose();

		disposePhysics3dComponentShape( pState->shape );

		pState->~Physics3dColliderComponentState();
	}
}
