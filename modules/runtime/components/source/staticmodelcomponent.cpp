
#include "tiki/components/staticmodelcomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components/staticmodelcomponent_initdata.hpp"
#include "tiki/components/transformcomponent.hpp"
#include "tiki/renderer/gamerenderer.hpp"

namespace tiki
{
	struct TransformComponentState;

	struct StaticModelComponentState : public ComponentState
	{
		const TransformComponentState*	pTransform;
		const Model*					pModel;
	};

	StaticModelComponent::StaticModelComponent()
	{
		m_pTransformComponent		= nullptr;
		m_transformComponentTypeId	= InvalidComponentTypeId;
	}

	StaticModelComponent::~StaticModelComponent()
	{
		TIKI_ASSERT( m_pTransformComponent		== nullptr );
		TIKI_ASSERT( m_transformComponentTypeId	== InvalidComponentTypeId );
	}

	bool StaticModelComponent::create( TransformComponent& transformComponent )
	{
		m_pTransformComponent		= &transformComponent;
		m_transformComponentTypeId	= transformComponent.getTypeId();
		if ( m_transformComponentTypeId == InvalidComponentTypeId )
		{
			return false;
		}

		return true;
	}

	void StaticModelComponent::dispose()
	{
		m_pTransformComponent		= nullptr;
		m_transformComponentTypeId	= InvalidComponentTypeId;
	}

	void StaticModelComponent::render( GameRenderer& gameRenderer ) const
	{
		ConstIterator componentStates = getConstIterator();

		const State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			Matrix43 worldTransform;
			m_pTransformComponent->getWorldTransform( worldTransform, pState->pTransform );

			gameRenderer.queueModel( pState->pModel, &worldTransform, nullptr );
		}
	}

	crc32 StaticModelComponent::getTypeCrc() const
	{
		return crcString( "StaticModelComponent" );
	}

	uint32 StaticModelComponent::getStateSize() const
	{
		return sizeof( StaticModelComponentState );
	}

	const char* StaticModelComponent::getTypeName() const
	{
		return "StaticModelComponent";
	}

	bool StaticModelComponent::internalInitializeState( ComponentEntityIterator& componentIterator, StaticModelComponentState* pState, const StaticModelComponentInitData* pInitData )
	{
		pState->pTransform = static_cast< TransformComponentState* >( static_cast< void* >( componentIterator.getFirstOfType( m_transformComponentTypeId ) ) );
		if ( pState->pTransform == nullptr )
		{
			return false;
		}

		pState->pModel = pInitData->model.getData();
		if ( pState->pModel == nullptr )
		{
			return false;
		}

		return true;
	}

	void StaticModelComponent::internalDisposeState( StaticModelComponentState* pState )
	{
		pState->pTransform	= nullptr;
		pState->pModel		= nullptr;
	}
}