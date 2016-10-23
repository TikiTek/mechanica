
#include "tiki/components/lifetimecomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/entitysystem/entitysystem.hpp"

#include "components.hpp"

namespace tiki
{
	struct LifeTimeComponentState : public ComponentState
	{
		timems	timeToLifeInMs;
	};
	TIKI_COMPONENT_STATE_CONSTRUCT_FUNCTIONS( LifeTimeComponentState );

	LifeTimeComponent::LifeTimeComponent()
		: Component( ComponentsType_LifeTime, "LifeTimeComponent", sizeof( LifeTimeComponentState ), false )
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

	void LifeTimeComponent::update( EntitySystem& entitySystem, timems timeMs )
	{
		Iterator componentStates = getIterator();

		State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			pState->timeToLifeInMs -= timeMs;

			if ( pState->timeToLifeInMs <= 0.0f )
			{
				entitySystem.disposeEntity( pState->entityId );
			}
		}
	}

	bool LifeTimeComponent::internalInitializeState( ComponentEntityIterator& componentIterator, LifeTimeComponentState* pState, const LifeTimeComponentInitData* pInitData )
	{
		pState->timeToLifeInMs = pInitData->lifeTimeInMs;

		return true;
	}

	void LifeTimeComponent::internalDisposeState( LifeTimeComponentState* pState )
	{
		pState->timeToLifeInMs = 0;
	}
}