#include "tiki/components2d/texturecomponent.hpp"

#include "tiki/components/componentstate.hpp"
#include "tiki/components2d/transform2dcomponent.hpp"
#include "tiki/graphics/texture.hpp"

#include "components2d.hpp"

namespace tiki
{
	struct Transform3dComponentState;

	struct TextureComponentState : public ComponentState
	{
		const Transform2dComponentState*	pTransform;
		const Texture*						pTexture;
	};

	TextureComponent::TextureComponent()
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

	//void TextureComponent::render( RenderScene& scene ) const
	//{
	//	ConstIterator componentStates = getConstIterator();

	//	const State* pState = nullptr;
	//	while ( pState = componentStates.getNext() )
	//	{
	//		Matrix43 worldTransform;
	//		m_pTransformComponent->getWorldTransform( worldTransform, pState->pTransform );

	//		scene.queueModel( pState->pModel, &worldTransform, nullptr );
	//	}
	//}

	crc32 TextureComponent::getTypeCrc() const
	{
		return Components2dType_Texture;
	}

	uint32 TextureComponent::getStateSize() const
	{
		return sizeof( TextureComponentState );
	}

	const char* TextureComponent::getTypeName() const
	{
		return "TextureComponent";
	}

	bool TextureComponent::internalInitializeState( ComponentEntityIterator& componentIterator, TextureComponentState* pState, const TextureComponentInitData* pInitData )
	{
		pState->pTransform = (const Transform2dComponentState*)componentIterator.getFirstOfType( m_pTransformComponent->getTypeId() );
		if ( pState->pTransform == nullptr )
		{
			return false;
		}

		pState->pTexture = pInitData->texture.getData();
		if ( pState->pTexture == nullptr )
		{
			return false;
		}

		return true;
	}

	void TextureComponent::internalDisposeState( TextureComponentState* pState )
	{
		pState->pTransform	= nullptr;
		pState->pTexture	= nullptr;
	}
}