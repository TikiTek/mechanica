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
		Transform2dComponentState*		pTransformState;
		Physics2dBodyComponentState*	pBodyState;

		float							destructionForce;
		float							breakAfterSeconds;
		bool							enableBreakTimer;
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

	bool BreakableComponent::create( Physics2dWorld& physicsWorld, const Transform2dComponent& transformComponent, const Physics2dBodyComponent& physicsBodyComponent )
	{
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

	bool BreakableComponent::internalInitializeState( ComponentEntityIterator& componentIterator, BreakableComponentState* pState, const BreakableComponentInitData* pInitData )
	{

		pState->pTransformState = (Transform2dComponentState*)componentIterator.getFirstOfType( m_pTransformComponent->getTypeId() );
		pState->pBodyState		= (Physics2dBodyComponentState*)componentIterator.getFirstOfType( m_pPhysicsBodyComponent->getTypeId() );

		return true;
	}

	void BreakableComponent::internalDisposeState( BreakableComponentState* pState )
	{


	}
}