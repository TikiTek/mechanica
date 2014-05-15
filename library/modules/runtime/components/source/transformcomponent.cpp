
#include "tiki/components/transformcomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/transformcomponent_initdata.hpp"

namespace tiki
{
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
			quaternion::toMatrix( pState->worldTransform.rot, pState->rotation );
			pState->worldTransform.pos = pState->position;
		}
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

		return true;
	}

	void TransformComponent::internalDisposeState( TransformComponentState* pState )
	{
		vector::clear( pState->position );
		quaternion::clear( pState->rotation );
	}
}