#include "tiki/mechanica_components/breakablecomponent.hpp"

#include "tiki/components/componentstate.hpp"
#include "tiki/components2d/polygoncomponent.hpp"
#include "tiki/components2d/spritecomponent.hpp"
#include "tiki/components2d/transform2dcomponent.hpp"
#include "tiki/container/fixedarray.hpp"
#include "tiki/entitysystem/entitysystem.hpp"
#include "tiki/physics2d/physics2dbody.hpp"
#include "tiki/physics2d/physics2djoint.hpp"
#include "tiki/physics2d/physics2dnoneshape.hpp"
#include "tiki/physics2dcomponents/physics2dbodycomponent.hpp"
#include "tiki/physics2dcomponents/physics2dbodycomponent.hpp"

#include "components2d.hpp"
#include "mechanica_components.hpp"

namespace tiki
{
	struct BreakableComponentState : public ComponentState
	{
		Transform2dComponentState*				pTransformState;

		const ResArray< BreakableFragment >*	pFragments;

		float									destructionForce;
		float									breakAfterSeconds;
		bool									enableBreakTimer;
	};
	TIKI_COMPONENT_STATE_CONSTRUCT_FUNCTIONS( BreakableComponentState );

	BreakableComponent::BreakableComponent()
		: Component( MechanicaComponentType_Breakable, "BreakableComponent", sizeof( BreakableComponentState ), true )
	{
		m_pEntitySystem			= nullptr;
		m_pPhysicsWorld			= nullptr;
		m_pTransformComponent	= nullptr;
		m_pSpriteComponent		= nullptr;
		m_pPolgonComponent		= nullptr;
	}

	BreakableComponent::~BreakableComponent()
	{
		TIKI_ASSERT( m_pEntitySystem		== nullptr );
		TIKI_ASSERT( m_pPhysicsWorld		== nullptr );
		TIKI_ASSERT( m_pTransformComponent	== nullptr );
		TIKI_ASSERT( m_pSpriteComponent		== nullptr );
		TIKI_ASSERT( m_pPolgonComponent		== nullptr );
	}

	bool BreakableComponent::create( EntitySystem& entitySystem, Physics2dWorld& physicsWorld, const Transform2dComponent& transformComponent, const SpriteComponent& spriteComponent, const PolygonComponent& polygonComponent )
	{
		m_pEntitySystem			= &entitySystem;
		m_pPhysicsWorld			= &physicsWorld;
		m_pTransformComponent	= &transformComponent;
		m_pSpriteComponent		= &spriteComponent;
		m_pPolgonComponent		= &polygonComponent;

		return true;
	}

	void BreakableComponent::dispose()
	{
		m_pEntitySystem			= nullptr;
		m_pPhysicsWorld			= nullptr;
		m_pTransformComponent	= nullptr;
		m_pSpriteComponent		= nullptr;
		m_pPolgonComponent		= nullptr;
	}

	void BreakableComponent::update( float deltaTime ) const
	{
		Iterator componentStates = getIterator();

		State* pState = nullptr;
		while( pState = componentStates.getNext() )
		{
			if( !pState->enableBreakTimer )
			{
				continue;
			}

			pState->breakAfterSeconds -= deltaTime;

			if( pState->breakAfterSeconds <= 0.0f )
			{
				breakBody( pState );
			}
		}
	}

	void BreakableComponent::breakBody( BreakableComponentState* pState ) const
	{
		ComponentEntityIterator entityComponentIterator( m_pEntitySystem->getFirstComponentOfEntity( pState->entityId ) );

		if( pState->pFragments != nullptr )
		{
			breakToCuttedFragmentEntities( entityComponentIterator, pState );
		}
		else
		{
			breakToCuttedFragmentEntities( entityComponentIterator, pState );
		}
	}

	bool BreakableComponent::internalInitializeState( const ComponentEntityIterator& componentIterator, BreakableComponentState* pState, const BreakableComponentInitData* pInitData )
	{
		if( pInitData->fragments.getCount() > 0u )
		{
			pState->pFragments		= &pInitData->fragments;
		}

		pState->destructionForce	= pInitData->destructionForce;
		pState->breakAfterSeconds	= pInitData->breakAfterSeconds;
		pState->enableBreakTimer	= pInitData->enableBreakTimer;

		return true;
	}

	void BreakableComponent::internalDisposeState( BreakableComponentState* pState )
	{
	}

	void BreakableComponent::breakToStaticFragmentEntities( const ComponentEntityIterator& parentEntityComponentIterator, const BreakableComponentState* pParentState ) const
	{
		const Transform2dComponentState* pTransformState	= (const Transform2dComponentState*)parentEntityComponentIterator.getFirstOfType( m_pTransformComponent->getTypeId() );
		const SpriteComponentState* pSpriteState			= (const SpriteComponentState*)parentEntityComponentIterator.getFirstOfType( m_pSpriteComponent->getTypeId() );

		for( uint i = 0u; i < pParentState->pFragments->getCount(); ++i )
		{
			const BreakableFragment& fragment = pParentState->pFragments->getAt( i );

			Transform2dComponentInitData transformInitData;
			createFloat2( transformInitData.position, 0.0f, 0.0f );
			createFloat2( transformInitData.scale, 1.0f, 1.0f );
			transformInitData.rotation	= 0.0f;

			PolygonComponentInitData polygonInitData;

		}
	}

	void BreakableComponent::breakToCuttedFragmentEntities( const ComponentEntityIterator& parentEntityComponentIterator, const BreakableComponentState* pParentState ) const
	{

	}
}