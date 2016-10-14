#include "tiki/components3d/terraincomponent.hpp"

#include "tiki/components/componentstate.hpp"
#include "tiki/components3d/transform3dcomponent.hpp"
#include "tiki/renderer/renderscene.hpp"

#include "components.hpp"

namespace tiki
{
	struct Transform3dComponentState;

	struct TerrainComponentState : public ComponentState
	{
		const Transform3dComponentState*	pTransform;
		const Model*						pModel;
	};

	TerrainComponent::TerrainComponent()
	{
		m_pTransformComponent = nullptr;
	}

	TerrainComponent::~TerrainComponent()
	{
		TIKI_ASSERT( m_pTransformComponent == nullptr );
	}

	bool TerrainComponent::create( Transform3dComponent& transformComponent )
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
		pState->pTransform = (const Transform3dComponentState*)componentIterator.getFirstOfType( m_pTransformComponent->getTypeId() );
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