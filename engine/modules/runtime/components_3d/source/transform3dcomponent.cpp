#include "tiki/components3d/transform3dcomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"

#include "components3d.hpp"

namespace tiki
{
	struct Transform3dComponentState : public ComponentState
	{
		Vector3		position;
		Quaternion	rotation;
		Vector3		scale;

		bool		needUpdate;
		Matrix43	worldTransform;
	};

	static void checkAndUpdateWorldTransform( Transform3dComponentState* pState )
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

	Transform3dComponent::Transform3dComponent()
	{
	}

	Transform3dComponent::~Transform3dComponent()
	{
	}

	bool Transform3dComponent::create()
	{
		return true;
	}

	void Transform3dComponent::dispose()
	{

	}

	void Transform3dComponent::update()
	{
		Iterator componentStates = getIterator();

		State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			checkAndUpdateWorldTransform( pState );
		}
	}

	void Transform3dComponent::getPosition( Vector3& targetPosition, const Transform3dComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		targetPosition = pState->position;
	}

	void Transform3dComponent::getRotation( Quaternion& targetRotation, const Transform3dComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		targetRotation = pState->rotation;
	}

	void Transform3dComponent::getWorldTransform( Matrix43& targetMatrix, const Transform3dComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );
		
		targetMatrix = pState->worldTransform;
	}

	void Transform3dComponent::setPosition( Transform3dComponentState* pState, const Vector3& position ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->position	= position;
		pState->needUpdate	= true;
	}

	void Transform3dComponent::setRotation( Transform3dComponentState* pState, const Quaternion& rotation ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->rotation	= rotation;
		pState->needUpdate	= true;
	}

	crc32 Transform3dComponent::getTypeCrc() const
	{
		return crcString( "Transform3dComponent" );
	}

	uint32 Transform3dComponent::getStateSize() const
	{
		return sizeof( Transform3dComponentState );
	}

	const char* Transform3dComponent::getTypeName() const
	{
		return "Transform3dComponent";
	}

	bool Transform3dComponent::internalInitializeState( ComponentEntityIterator& componentIterator, Transform3dComponentState* pState, const Transform3dComponentInitData* pInitData )
	{
		vector::set( pState->position, pInitData->position );
		quaternion::set( pState->rotation, pInitData->rotation );
		vector::set( pState->scale, pInitData->scale );
		pState->needUpdate = true;

		checkAndUpdateWorldTransform( pState );

		return true;
	}

	void Transform3dComponent::internalDisposeState( Transform3dComponentState* pState )
	{
		vector::clear( pState->position );
		quaternion::clear( pState->rotation );
		vector::clear( pState->scale );
	}
}