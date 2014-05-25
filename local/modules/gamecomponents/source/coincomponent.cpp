
#include "tiki/gamecomponents/coincomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components/lifetimecomponent.hpp"
#include "tiki/components/transformcomponent.hpp"
#include "tiki/gamecomponents/coincomponent_initdata.hpp"
#include "tiki/math/quaternion.hpp"

namespace tiki
{
	struct CoinComponentState : public ComponentState
	{
		TransformComponentState*	pTransform;

		float						value;
	};

	CoinComponent::CoinComponent()
	{
	}

	CoinComponent::~CoinComponent()
	{
	}

	bool CoinComponent::create( const TransformComponent& transformComponent, const LifeTimeComponent& lifeTimeComponent )
	{
		m_pTransformComponent	= &transformComponent;
		m_pLifeTimeComponent	= &lifeTimeComponent;

		return true;
	}

	void CoinComponent::dispose()
	{
	}

	void CoinComponent::update( float totalGameTime )
	{
		Iterator componentStates = getIterator();

		State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			Quaternion rotation;
			quaternion::fromYawPitchRoll( rotation, totalGameTime, 0.0f, 0.0f );

			m_pTransformComponent->setRotation( pState->pTransform, rotation );
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
		pState->pTransform = (TransformComponentState*)componentIterator.getFirstOfType( m_pTransformComponent->getTypeId() );
		if ( pState->pTransform == nullptr )
		{
			return false;
		}

		pState->value = pInitData->value;

		return true;
	}

	void CoinComponent::internalDisposeState( CoinComponentState* pState )
	{
		pState->pTransform	= nullptr;

		pState->value		= 0.0f;
	}
}