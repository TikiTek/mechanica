
#include "tiki/gamecomponents/coincomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/gamecomponents/coincomponent_initdata.hpp"

namespace tiki
{
	struct CoinComponentState : public ComponentState
	{
		float	value;
	};

	CoinComponent::CoinComponent()
	{
	}

	CoinComponent::~CoinComponent()
	{
	}

	bool CoinComponent::create()
	{
		return true;
	}

	void CoinComponent::dispose()
	{
	}

	void CoinComponent::update()
	{
		Iterator componentStates = getIterator();

		State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
		}
	}

	crc32 CoinComponent::getTypeCrc() const
	{
		return crcString( "CoinComponent" );
	}

	uint32 CoinComponent::getStateSize() const
	{
		return sizeof( CoinComponentState );
	}

	const char* CoinComponent::getTypeName() const
	{
		return "CoinComponent";
	}

	bool CoinComponent::internalInitializeState( ComponentEntityIterator& componentIterator, CoinComponentState* pState, const CoinComponentInitData* pInitData )
	{
		pState->value = pInitData->value;

		return true;
	}

	void CoinComponent::internalDisposeState( CoinComponentState* pState )
	{
		pState->value = 0.0f;
	}
}