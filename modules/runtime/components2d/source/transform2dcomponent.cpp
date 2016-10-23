#include "tiki/components2d/transform2dcomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"

#include "components2d.hpp"

namespace tiki
{
	struct Transform2dComponentState : public ComponentState
	{
		Vector2		position;
		float		rotation;
		Vector2		scale;

		bool		needUpdate;
		Matrix32	worldTransform;
	};
	TIKI_COMPONENT_STATE_CONSTRUCT_FUNCTIONS( Transform2dComponentState );

	static void checkAndUpdateWorldTransform( Transform2dComponentState* pState )
	{
		if ( pState->needUpdate )
		{
			matrix::createRotationZ( pState->worldTransform.rot, pState->rotation );
			vector::scale( pState->worldTransform.rot.x, pState->scale.x );
			vector::scale( pState->worldTransform.rot.y, pState->scale.y );

			pState->worldTransform.pos = pState->position;
			pState->needUpdate = false;
		}
	}

	Transform2dComponent::Transform2dComponent()
		: Component( Components2dType_Transform, "Transform2dComponent", sizeof( Transform2dComponentState ), false )
	{
	}

	Transform2dComponent::~Transform2dComponent()
	{
	}

	bool Transform2dComponent::create()
	{
		return true;
	}

	void Transform2dComponent::dispose()
	{
	}

	void Transform2dComponent::update()
	{
		Iterator componentStates = getIterator();

		State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			checkAndUpdateWorldTransform( pState );
		}
	}

	const Vector2& Transform2dComponent::getPosition( const Transform2dComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );
		return pState->position;
	}

	float Transform2dComponent::getRotation( const Transform2dComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );
		return pState->rotation;
	}

	const Matrix32& Transform2dComponent::getWorldTransform( const Transform2dComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );		
		return pState->worldTransform;
	}

	void Transform2dComponent::setPosition( Transform2dComponentState* pState, const Vector2& position ) const
	{
		TIKI_ASSERT( pState != nullptr );
		pState->position	= position;
		pState->needUpdate	= true;
	}

	void Transform2dComponent::setRotation( Transform2dComponentState* pState, float rotation ) const
	{
		TIKI_ASSERT( pState != nullptr );
		pState->rotation	= rotation;
		pState->needUpdate	= true;
	}

	bool Transform2dComponent::internalInitializeState( ComponentEntityIterator& componentIterator, Transform2dComponentState* pState, const Transform2dComponentInitData* pInitData )
	{
		vector::set( pState->position, pInitData->position );
		pState->rotation = pInitData->rotation;
		vector::set( pState->scale, pInitData->scale );
		pState->needUpdate = true;

		checkAndUpdateWorldTransform( pState );

		return true;
	}

	void Transform2dComponent::internalDisposeState( Transform2dComponentState* pState )
	{
	}
}