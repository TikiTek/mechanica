#include "tiki/physics2dcomponents/physics2djointcomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/physics2d/physics2djoint.hpp"
#include "tiki/physics2d/physics2dworld.hpp"
#include "tiki/physics2dcomponents/physics2dbodycomponent.hpp"

#include "physic2dcomponents.hpp"

namespace tiki
{
	struct Physics2dJointComponentState : public ComponentState
	{
		Physics2dJoint	joint;
	};
	TIKI_COMPONENT_STATE_CONSTRUCT_FUNCTIONS( Physics2dJointComponentState );

	Physics2dJointComponent::Physics2dJointComponent()
		: Component( Physics2dComponentType_Joint, "Physics2dJointComponent", sizeof( Physics2dJointComponentState ), true )
	{
		m_pPhysicsWorld				= nullptr;
	}

	Physics2dJointComponent::~Physics2dJointComponent()
	{
		TIKI_ASSERT( m_pPhysicsWorld == nullptr );
	}

	bool Physics2dJointComponent::create( Physics2dWorld& physicsWorld )
	{
		m_pPhysicsWorld = &physicsWorld;

		return true;
	}

	void Physics2dJointComponent::dispose()
	{
		m_pPhysicsWorld = nullptr;
	}

	bool Physics2dJointComponent::internalInitializeState( ComponentEntityIterator& componentIterator, Physics2dJointComponentState* pState, const Physics2dJointComponentInitData* pInitData )
	{
		TIKI_ASSERT( m_pPhysicsWorld != nullptr );

		TIKI_NOT_IMPLEMENTED;

		return true;
	}

	void Physics2dJointComponent::internalDisposeState( Physics2dJointComponentState* pState )
	{
		TIKI_ASSERT( m_pPhysicsWorld != nullptr );

		TIKI_NOT_IMPLEMENTED;
	}
}
