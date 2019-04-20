#include "tiki/physics_2d_components/physics_2d_joint_component.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/component_state.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/physics_2d/physics_2d_joint.hpp"
#include "tiki/physics_2d/physics_2d_world.hpp"
#include "tiki/physics_2d_components/physics_2d_body_component.hpp"

#include "physics_2d_components.hpp"

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

	bool Physics2dJointComponent::internalInitializeState( const ComponentEntityIterator& componentIterator, Physics2dJointComponentState* pState, const Physics2dJointComponentInitData* pInitData )
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
