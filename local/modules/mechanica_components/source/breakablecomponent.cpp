#include "tiki/mechanica_components/breakablecomponent.hpp"

#include "tiki/components/componentstate.hpp"
#include "tiki/components2d/transform2dcomponent.hpp"
#include "tiki/container/fixedarray.hpp"
#include "tiki/physics2d/physics2dbody.hpp"
#include "tiki/physics2d/physics2djoint.hpp"
#include "tiki/physics2d/physics2dnoneshape.hpp"
#include "tiki/physics2dcomponents/physics2dbodycomponent.hpp"
#include "tiki/physics2dcomponents/physics2dbodycomponent.hpp"

#include "mechanica_components.hpp"

namespace tiki
{
	struct BreakableComponentState : public ComponentState
	{
		Transform2dComponentState*				pTransformState;
		Physics2dBodyComponentState*			pBodyState;

		const ResArray< BreakableFragment >*	pFragments;

		float									destructionForce;
		float									breakAfterSeconds;
		bool									enableBreakTimer;
	};
	TIKI_COMPONENT_STATE_CONSTRUCT_FUNCTIONS( BreakableComponentState );

	BreakableComponent::BreakableComponent()
		: Component( MechanicaComponentType_Breakable, "BreakableComponent", sizeof( BreakableComponentState ), true )
	{
		m_pPhysicsBodyComponent	= nullptr;
		m_pPhysicsWorld			= nullptr;
	}

	BreakableComponent::~BreakableComponent()
	{
		TIKI_ASSERT( m_pPhysicsBodyComponent	== nullptr );
		TIKI_ASSERT( m_pPhysicsWorld			== nullptr );
	}

	bool BreakableComponent::create( EntitySystem& entitySystem, Physics2dWorld& physicsWorld, const Transform2dComponent& transformComponent, const Physics2dBodyComponent& physicsBodyComponent )
	{
		m_pEntitySystem			= &entitySystem;

		m_pPhysicsWorld			= &physicsWorld;

		m_pTransformComponent	= &transformComponent;
		m_pPhysicsBodyComponent	= &physicsBodyComponent;

		return true;
	}

	void BreakableComponent::dispose()
	{
		m_pPhysicsBodyComponent	= nullptr;
		m_pTransformComponent	= nullptr;

		m_pPhysicsWorld			= nullptr;
	}

	void BreakableComponent::update( float deltaTime )
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

	void BreakableComponent::breakBody( BreakableComponentState* pState )
	{
		if( pState->pFragments != nullptr )
		{
			createFragmentEntities( *pState->pFragments );
		}
		else
		{

		}
	}

	bool BreakableComponent::internalInitializeState( ComponentEntityIterator& componentIterator, BreakableComponentState* pState, const BreakableComponentInitData* pInitData )
	{
		pState->pTransformState		= (Transform2dComponentState*)componentIterator.getFirstOfType( m_pTransformComponent->getTypeId() );
		pState->pBodyState			= (Physics2dBodyComponentState*)componentIterator.getFirstOfType( m_pPhysicsBodyComponent->getTypeId() );

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

	void BreakableComponent::createFragmentEntities( const ResArray< BreakableFragment >& fragments )
	{

	}

}