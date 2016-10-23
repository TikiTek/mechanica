#include "tiki/components2d/texturecomponent.hpp"

#include "tiki/components/componentstate.hpp"
#include "tiki/components2d/transform2dcomponent.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/renderer2d/renderer2d.hpp"

#include "components2d.hpp"

namespace tiki
{
	struct Transform3dComponentState;

	struct TextureComponentState : public ComponentState
	{
		const Transform2dComponentState*	pTransform;
		const Texture*						pTexture;
		Vector2								offset;
		uint32								layerId;
	};
	TIKI_COMPONENT_STATE_CONSTRUCT_FUNCTIONS( TextureComponentState );

	TextureComponent::TextureComponent()
		: Component( Components2dType_Texture, "TextureComponent", sizeof( TextureComponentState ), false )
	{
		m_pTransformComponent = nullptr;
	}

	TextureComponent::~TextureComponent()
	{
		TIKI_ASSERT( m_pTransformComponent == nullptr );
	}

	bool TextureComponent::create( Transform2dComponent& transformComponent )
	{
		m_pTransformComponent = &transformComponent;

		return true;
	}

	void TextureComponent::dispose()
	{
		m_pTransformComponent = nullptr;
	}

	void TextureComponent::render( Renderer2d& renderer ) const
	{
		ConstIterator componentStates = getConstIterator();

		const State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			const Matrix32& worldTransform = m_pTransformComponent->getWorldTransform( pState->pTransform );		
			renderer.queueSprite( pState->pTexture->getTextureData(), worldTransform, pState->offset, pState->layerId );
		}
	}

	bool TextureComponent::internalInitializeState( ComponentEntityIterator& componentIterator, TextureComponentState* pState, const TextureComponentInitData* pInitData )
	{
		pState->pTransform	= (const Transform2dComponentState*)componentIterator.getFirstOfType( m_pTransformComponent->getTypeId() );
		pState->pTexture	= pInitData->texture.getData();
		pState->layerId		= pInitData->layerId;
		vector::set( pState->offset, pInitData->offset );

		return (pState->pTransform != nullptr && pState->pTexture != nullptr);
	}

	void TextureComponent::internalDisposeState( TextureComponentState* pState )
	{
		pState->pTransform	= nullptr;
		pState->pTexture	= nullptr;
	}
}