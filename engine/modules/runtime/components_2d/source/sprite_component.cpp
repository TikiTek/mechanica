#include "tiki/components_2d/sprite_component.hpp"

#include "tiki/components/component_state.hpp"
#include "tiki/components_2d/transform_2d_component.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/renderer_2d/renderer_2d.hpp"

#include "components_2d.hpp"

namespace tiki
{
	struct Transform3dComponentState;

	struct SpriteComponentState : public ComponentState
	{
		const Transform2dComponentState*	pTransform;
		const Texture*						pTexture;
		Vector2								offset;
		uint32								layerId;
	};
	TIKI_COMPONENT_STATE_CONSTRUCT_FUNCTIONS( SpriteComponentState );

	SpriteComponent::SpriteComponent()
		: Component( Components2dType_Sprite, "SpriteComponent", sizeof( SpriteComponentState ), false )
	{
		m_pTransformComponent = nullptr;
	}

	SpriteComponent::~SpriteComponent()
	{
		TIKI_ASSERT( m_pTransformComponent == nullptr );
	}

	bool SpriteComponent::create( Transform2dComponent& transformComponent )
	{
		m_pTransformComponent = &transformComponent;

		return true;
	}

	void SpriteComponent::dispose()
	{
		m_pTransformComponent = nullptr;
	}

	void SpriteComponent::render( Renderer2d& renderer ) const
	{
		ConstIterator componentStates = getConstIterator();

		const State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			const Matrix32& worldTransform = m_pTransformComponent->getWorldTransform( pState->pTransform );
			renderer.queueSprite( pState->pTexture->getTextureData(), worldTransform, pState->offset, pState->layerId );
		}
	}

	const tiki::Texture* SpriteComponent::getTexture( const SpriteComponentState* pState ) const
	{
		return pState->pTexture;
	}

	const tiki::Vector2& SpriteComponent::getOffset( const SpriteComponentState* pState ) const
	{
		return pState->offset;
	}

	uint32 SpriteComponent::getLayerId( const SpriteComponentState* pState ) const
	{
		return pState->layerId;
	}

	bool SpriteComponent::internalInitializeState( const ComponentEntityIterator& componentIterator, SpriteComponentState* pState, const SpriteComponentInitData* pInitData )
	{
		pState->pTransform	= (const Transform2dComponentState*)componentIterator.getFirstOfType( m_pTransformComponent->getTypeId() );
		pState->pTexture	= pInitData->texture.getData();
		pState->layerId		= pInitData->layerId;
		vector::set( pState->offset, pInitData->offset );

		return (pState->pTransform != nullptr && pState->pTexture != nullptr);
	}

	void SpriteComponent::internalDisposeState( SpriteComponentState* pState )
	{
		pState->pTransform	= nullptr;
		pState->pTexture	= nullptr;
	}
}