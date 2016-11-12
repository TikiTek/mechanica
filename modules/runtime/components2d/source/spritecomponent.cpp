#include "tiki/components2d/spritecomponent.hpp"

#include "tiki/components/componentstate.hpp"
#include "tiki/components2d/transform2dcomponent.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/renderer2d/renderer2d.hpp"

#include "components2d.hpp"

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