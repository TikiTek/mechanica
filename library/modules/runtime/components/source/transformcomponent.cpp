
#include "tiki/components/transformcomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components/transformcomponent_initdata.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	struct TransformComponentState : public ComponentState
	{
		Vector3		position;
		Quaternion	rotation;
		Vector3		scale;

		bool		needUpdate;
		Matrix43	worldTransform;
	};

	static void checkAndUpdateWorldTransform( TransformComponentState* pState )
	{
		if ( pState->needUpdate )
		{
			quaternion::toMatrix( pState->worldTransform.rot, pState->rotation );
			vector::scale( pState->worldTransform.rot.x, pState->scale.x );
			vector::scale( pState->worldTransform.rot.y, pState->scale.y );
			vector::scale( pState->worldTransform.rot.z, pState->scale.z );

			pState->worldTransform.pos = pState->position;
		}
	}

	TransformComponent::TransformComponent()
	{
	}

	TransformComponent::~TransformComponent()
	{
	}

	bool TransformComponent::create()
	{
		return true;
	}

	void TransformComponent::dispose()
	{

	}

	void TransformComponent::update()
	{
		Iterator componentStates = getIterator();

		State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			checkAndUpdateWorldTransform( pState );
		}
	}

	void TransformComponent::getPosition( Vector3& targetPosition, const TransformComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		targetPosition = pState->position;
	}

	void TransformComponent::getRotation( Quaternion& targetRotation, const TransformComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		targetRotation = pState->rotation;
	}

	void TransformComponent::getWorldTransform( Matrix43& targetMatrix, const TransformComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );
		
		targetMatrix = pState->worldTransform;
	}

	void TransformComponent::setPosition( TransformComponentState* pState, const Vector3& position ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->position	= position;
		pState->needUpdate	= true;
	}

	void TransformComponent::setRotation( TransformComponentState* pState, const Quaternion& rotation ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->rotation	= rotation;
		pState->needUpdate	= true;
	}

	crc32 TransformComponent::getTypeCrc() const
	{
		return crcString( "TransformComponent" );
	}

	uint32 TransformComponent::getStateSize() const
	{
		return sizeof( TransformComponentState );
	}

	const char* TransformComponent::getTypeName() const
	{
		return "TransformComponent";
	}

	bool TransformComponent::internalInitializeState( ComponentEntityIterator& componentIterator, TransformComponentState* pState, const TransformComponentInitData* pInitData )
	{
		vector::set( pState->position, pInitData->position );
		quaternion::set( pState->rotation, pInitData->rotation );
		vector::set( pState->scale, pInitData->scale );
		pState->needUpdate = true;

		return true;
	}

	void TransformComponent::internalDisposeState( TransformComponentState* pState )
	{
		vector::clear( pState->position );
		quaternion::clear( pState->rotation );
		vector::clear( pState->scale );
	}
}