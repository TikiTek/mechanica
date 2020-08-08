#include "tiki/components_2d/polygon_component.hpp"

#include "tiki/components/component_state.hpp"
#include "tiki/components_2d/transform_2d_component.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/renderer_2d/renderer_2d.hpp"

#include "components_2d.hpp"

namespace tiki
{
	struct Transform3dComponentState;

	struct PolygonComponentState : public ComponentState
	{
		const Transform2dComponentState*	pTransform;
		const Texture*						pTexture;
		Vector2								offset;
		uint32								layerId;
	};
	TIKI_COMPONENT_STATE_CONSTRUCT_FUNCTIONS( PolygonComponentState );

	PolygonComponent::PolygonComponent()
		: Component( Components2dType_Polygon, "PolygonComponent", sizeof( PolygonComponentState ), false )
	{
		m_pTransformComponent = nullptr;
	}

	PolygonComponent::~PolygonComponent()
	{
		TIKI_ASSERT( m_pTransformComponent == nullptr );
	}

	bool PolygonComponent::create( const Transform2dComponent& transformComponent )
	{
		m_pTransformComponent = &transformComponent;

		return true;
	}

	void PolygonComponent::dispose()
	{
		m_pTransformComponent = nullptr;
	}

	void PolygonComponent::render( Renderer2d& renderer ) const
	{
		ConstIterator componentStates = getConstIterator();

		const State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			const Matrix32& worldTransform = m_pTransformComponent->getWorldTransform( pState->pTransform );
			renderer.queueSprite( pState->pTexture->getTextureData(), worldTransform, pState->offset, pState->layerId );
		}
	}

	bool PolygonComponent::internalInitializeState( const ComponentEntityIterator& componentIterator, PolygonComponentState* pState, const PolygonComponentInitData* pInitData )
	{
		pState->pTransform	= (const Transform2dComponentState*)componentIterator.getFirstOfType( m_pTransformComponent->getTypeId() );
		pState->pTexture	= pInitData->texture.getData();
		pState->layerId		= pInitData->layerId;

		return (pState->pTransform != nullptr && pState->pTexture != nullptr);
	}

	void PolygonComponent::internalDisposeState( PolygonComponentState* pState )
	{
		pState->pTransform	= nullptr;
		pState->pTexture	= nullptr;
	}
}