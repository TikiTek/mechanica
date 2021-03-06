#include "tiki/mechanica_components/wiggle_component.hpp"

#include "tiki/components/component_state.hpp"
#include "tiki/container/fixed_array.hpp"
#include "tiki/physics_2d/physics_2d_collider.hpp"
#include "tiki/physics_2d/physics_2d_joint.hpp"
#include "tiki/physics_2d/physics_2d_none_shape.hpp"
#include "tiki/physics_2d_components/physics_2d_body_component.hpp"

#include "mechanica_components.hpp"

namespace tiki
{
	struct WiggleComponentState : public ComponentState
	{
		Transform2dComponentState*			pTransformState;
		Physics2dBodyComponentState*		pBodyState;

		FixedArray< Physics2dCollider, 4u >	colliders;
		FixedArray< Physics2dJoint, 4u >	joints;
	};
	TIKI_COMPONENT_STATE_CONSTRUCT_FUNCTIONS( WiggleComponentState );

	WiggleComponent::WiggleComponent()
		: Component( MechanicaComponentType_Wiggle, "WiggleComponent", sizeof( WiggleComponentState ), true )
	{
		m_pPhysicsBodyComponent	= nullptr;
		m_pPhysicsWorld			= nullptr;
	}

	WiggleComponent::~WiggleComponent()
	{
		TIKI_ASSERT( m_pPhysicsBodyComponent	== nullptr );
		TIKI_ASSERT( m_pPhysicsWorld			== nullptr );
	}

	bool WiggleComponent::create( Physics2dWorld& physicsWorld, const Transform2dComponent& transformComponent, const Physics2dBodyComponent& physicsBodyComponent )
	{
		m_pPhysicsWorld			= &physicsWorld;

		m_pTransformComponent	= &transformComponent;
		m_pPhysicsBodyComponent	= &physicsBodyComponent;

		return true;
	}

	void WiggleComponent::dispose()
	{
		m_pPhysicsBodyComponent	= nullptr;
		m_pTransformComponent	= nullptr;

		m_pPhysicsWorld			= nullptr;
	}

	bool WiggleComponent::internalInitializeState( const ComponentEntityIterator& componentIterator, WiggleComponentState* pState, const WiggleComponentInitData* pInitData )
	{
		pState->pTransformState = (Transform2dComponentState*)componentIterator.getFirstOfType( m_pTransformComponent->getTypeId() );
		pState->pBodyState		= (Physics2dBodyComponentState*)componentIterator.getFirstOfType( m_pPhysicsBodyComponent->getTypeId() );

		const Vector2 position = m_pTransformComponent->getPosition( pState->pTransformState );
		const Physics2dBody& body = m_pPhysicsBodyComponent->getPhysicsObject( pState->pBodyState );

		Physics2dNoneShape noneShape;
		for( uint i = 0u; i < 4u; ++i )
		{
			const float x = (i == 0 || i == 1 ? 3.0f : -3.0f);
			const float y = (i == 0 || i == 2 ? 1.0f : -1.0f);

			const float lx = (i == 0 || i == 1 ? 2.5f : -2.5f);
			const float ly = (i == 0 || i == 2 ? 0.0f : -0.0f);

			const Vector2 colliderPosition = vector::create( x, y );
			const Vector2 localOffst = vector::create( lx, ly );

			Vector2 localPosition;
			vector::add( localPosition, position, localOffst );

			Vector2 distance;
			vector::sub( distance, localPosition, colliderPosition );

			const float ff = (i == 0 || i == 2 ? 1.1f : 1.0f);

			if( !pState->colliders[ i ].create( *m_pPhysicsWorld, noneShape, colliderPosition ) )
			{
				return false;
			}

			if( !pState->joints[ i ].createAsRope( *m_pPhysicsWorld, pState->colliders[ i ], Vector2::zero, body, vector::create( lx, ly ), vector::length( distance ) * ff ) )
			{
				return false;
			}
		}

		return true;
	}

	void WiggleComponent::internalDisposeState( WiggleComponentState* pState )
	{
		for( uint i = 0u; i < 4u; ++i )
		{
			pState->joints[ i ].dispose( *m_pPhysicsWorld );
			pState->colliders[ i ].dispose( *m_pPhysicsWorld );
		}
	}
}