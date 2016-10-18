#include "tiki/physics2dcomponents/physics2dbodycomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components2d/transform2dcomponent.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/physics2d/physics2dbody.hpp"
#include "tiki/physics2d/physics2dworld.hpp"

#include "physics2dcomponents_shared.hpp"

#include "physic2dcomponents.hpp"

namespace tiki
{
	struct Physics2dBodyComponentState : public ComponentState
	{
		Transform2dComponentState*	pTransform;

		Physics2dComponentShape		shape;

		Physics2dBody				body;
	};

	Physics2dBodyComponent::Physics2dBodyComponent()
	{
		m_pPhysicsWorld			= nullptr;

		m_pTranformComponent	= nullptr;
	}

	Physics2dBodyComponent::~Physics2dBodyComponent()
	{
		TIKI_ASSERT( m_pPhysicsWorld		== nullptr );
		TIKI_ASSERT( m_pTranformComponent	== nullptr );
	}

	bool Physics2dBodyComponent::create( Physics2dWorld& physicsWorld, const Transform2dComponent& transformComponent )
	{
		m_pPhysicsWorld			= &physicsWorld;

		m_pTranformComponent	= &transformComponent;

		return true;
	}

	void Physics2dBodyComponent::dispose()
	{
		m_pPhysicsWorld			= nullptr;

		m_pTranformComponent	= nullptr;
	}

	void Physics2dBodyComponent::update()
	{
		ConstIterator componentStates = getConstIterator();

		const State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			m_pTranformComponent->setPosition( pState->pTransform, pState->body.getPosition() );
			m_pTranformComponent->setRotation( pState->pTransform, pState->body.getRotation() );
		}
	}

	void Physics2dBodyComponent::applyForce( Physics2dBodyComponentState* pState, const Vector2& force, const Vector2& localPoint /* = Vector2::zero */ ) const
	{
		TIKI_ASSERT( pState != nullptr );
		pState->body.applyForce( force, localPoint );
	}

	const Physics2dCollisionObject& Physics2dBodyComponent::getPhysicsObject( const Physics2dBodyComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );
		return pState->body;
	}

	crc32 Physics2dBodyComponent::getTypeCrc() const
	{
		return Physics2dComponentType_Body;
	}

	uint32 Physics2dBodyComponent::getStateSize() const
	{
		return sizeof( Physics2dBodyComponentState );
	}

	const char* Physics2dBodyComponent::getTypeName() const
	{
		return "Physics2dBodyComponent";
	}

	bool Physics2dBodyComponent::internalInitializeState( ComponentEntityIterator& componentIterator, Physics2dBodyComponentState* pState, const Physics2dBodyComponentInitData* pInitData )
	{
		TIKI_ASSERT( m_pPhysicsWorld != nullptr );

		pState = new( pState ) Physics2dBodyComponentState;

		pState->pTransform = (Transform2dComponentState*)componentIterator.getFirstOfType( m_pTranformComponent->getTypeId() );
		if ( pState->pTransform == nullptr )
		{
			return false;
		}

		Physics2dShape* pShape = createPhysics2dComponentShape( pState->shape, pInitData->shape );
		if ( pShape == nullptr )
		{
			return false;
		}
				
		return pState->body.create( *m_pPhysicsWorld, *pShape, vector::create( pInitData->position ), pInitData->density, pInitData->friction, pInitData->freeRotation );
	}

	void Physics2dBodyComponent::internalDisposeState( Physics2dBodyComponentState* pState )
	{
		TIKI_ASSERT( m_pPhysicsWorld != nullptr );

		pState->pTransform = nullptr;

		pState->body.dispose( *m_pPhysicsWorld );

		disposePhysics2dComponentShape( pState->shape );

		pState->~Physics2dBodyComponentState();
	}
}
