#include "tiki/mechanica_components/wigglecomponent.hpp"

#include "tiki/components/componentstate.hpp"
#include "tiki/physics2d/physics2dbody.hpp"
#include "tiki/physics2d/physics2djoint.hpp"
#include "tiki/physics2d/physics2dworld.hpp"

#include "mechanica_components.hpp"

namespace tiki
{
	struct WiggleComponentState : public ComponentState
	{
	};

	WiggleComponent::WiggleComponent()
	{
		m_pPhysicsBodyComponent	= nullptr;
		m_pPhysicsWorld			= nullptr;
	}

	WiggleComponent::~WiggleComponent()
	{
		TIKI_ASSERT( m_pPhysicsBodyComponent	== nullptr );
		TIKI_ASSERT( m_pPhysicsWorld			== nullptr );
	}

	bool WiggleComponent::create( Physics2dWorld& physicsWorld, const Physics2dBodyComponent& physicsBodyComponent )
	{
		m_pPhysicsWorld			= &physicsWorld;
		m_pPhysicsBodyComponent	= &physicsBodyComponent;

		return true;
	}

	void WiggleComponent::dispose()
	{
		m_pPhysicsBodyComponent	= nullptr;
		m_pPhysicsWorld			= nullptr;
	}

	crc32 WiggleComponent::getTypeCrc() const
	{
		return MechanicaComponentType_Wiggle;
	}

	uint32 WiggleComponent::getStateSize() const
	{
		return sizeof( WiggleComponentState );
	}

	const char* WiggleComponent::getTypeName() const
	{
		return "WiggleComponent";
	}

	bool WiggleComponent::internalInitializeState( ComponentEntityIterator& componentIterator, WiggleComponentState* pState, const WiggleComponentInitData* pInitData )
	{
		return true;
	}

	void WiggleComponent::internalDisposeState( WiggleComponentState* pState )
	{
	}
}