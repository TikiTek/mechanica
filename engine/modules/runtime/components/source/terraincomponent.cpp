
#include "tiki/components/terraincomponent.hpp"

#include "tiki/components/componentstate.hpp"
#include "tiki/components/transformcomponent.hpp"
#include "tiki/renderer/renderscene.hpp"

#include "components.hpp"

namespace tiki
{
	struct TransformComponentState;

	struct TerrainComponentState : public ComponentState
	{
		const TransformComponentState*	pTransform;
		const Model*					pModel;
	};

	TerrainComponent::TerrainComponent()
	{
		m_pTransformComponent = nullptr;
	}

	TerrainComponent::~TerrainComponent()
	{
		TIKI_ASSERT( m_pTransformComponent == nullptr );
	}

	bool TerrainComponent::create( TransformComponent& transformComponent )
	{
		m_pTransformComponent = &transformComponent;

		return true;
	}

	void TerrainComponent::dispose()
	{
		m_pTransformComponent = nullptr;
	}

	void TerrainComponent::render( RenderScene& scene ) const
	{
		ConstIterator componentStates = getConstIterator();

		const State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			Matrix43 worldTransform;
			m_pTransformComponent->getWorldTransform( worldTransform, pState->pTransform );

			scene.queueModel( pState->pModel, &worldTransform, nullptr );
		}
	}

	float TerrainComponent::getHeightAtPosition( const TerrainComponentState* pComponentState, const Vector2& position ) const
	{
		return 0.0f;
	}

	uint32 TerrainComponent::getStateSize() const
	{
		return sizeof( TerrainComponentState );
	}

	const char* TerrainComponent::getTypeName() const
	{
		return "TerrainComponent";
	}

	bool TerrainComponent::internalInitializeState( ComponentEntityIterator& componentIterator, TerrainComponentState* pState, const TerrainComponentInitData* pInitData )
	{
		pState->pTransform = (const TransformComponentState*)componentIterator.getFirstOfType( m_pTransformComponent->getTypeId() );
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

	void TerrainComponent::internalDisposeState( TerrainComponentState* pState )
	{
		pState->pTransform	= nullptr;
		pState->pModel		= nullptr;
	}
}