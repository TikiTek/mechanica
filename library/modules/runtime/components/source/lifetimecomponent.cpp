
#include "tiki/components/lifetimecomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components/lifetimecomponent_initdata.hpp"
#include "tiki/entitysystem/entitysystem.hpp"

namespace tiki
{
	struct LifeTimeComponentState : public ComponentState
	{
		float	timeToLifeInSeconds;
	};

	LifeTimeComponent::LifeTimeComponent()
	{
	}

	LifeTimeComponent::~LifeTimeComponent()
	{
	}

	bool LifeTimeComponent::create()
	{
		return true;
	}

	void LifeTimeComponent::dispose()
	{
	}

	void LifeTimeComponent::update( EntitySystem& entitySystem, float timeDelta )
	{
		Iterator componentStates = getIterator();

		State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			pState->timeToLifeInSeconds -= timeDelta;

			if ( pState->timeToLifeInSeconds <= 0.0f )
			{
				entitySystem.disposeEntity( pState->entityId );
			}
		}
	}

	crc32 LifeTimeComponent::getTypeCrc() const
	{
		return crcString( "LifeTimeComponent" );
	}

	uint32 LifeTimeComponent::getStateSize() const
	{
		return sizeof( LifeTimeComponentState );
	}

	const char* LifeTimeComponent::getTypeName() const
	{
		return "LifeTimeComponent";
	}

	bool LifeTimeComponent::internalInitializeState( ComponentEntityIterator& componentIterator, LifeTimeComponentState* pState, const LifeTimeComponentInitData* pInitData )
	{
		pState->timeToLifeInSeconds = pInitData->lifeTimeInSeconds;

		return true;
	}

	void LifeTimeComponent::internalDisposeState( LifeTimeComponentState* pState )
	{
		pState->timeToLifeInSeconds = 0.0f;
	}
}