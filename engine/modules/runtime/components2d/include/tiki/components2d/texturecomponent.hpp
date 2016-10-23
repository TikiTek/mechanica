#pragma once
#ifndef TIKI_TEXTURECOMPONENT_HPP_INCLUDED
#define TIKI_TEXTURECOMPONENT_HPP_INCLUDED

#include "tiki/components/component.hpp"

namespace tiki
{
	class Renderer2d;
	class Transform2dComponent;
	struct TextureComponentInitData;
	struct TextureComponentState;
	struct Vector2;

	class TextureComponent : public Component< TextureComponentState, TextureComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( TextureComponent );

	public:

		explicit			TextureComponent();
		virtual				~TextureComponent();

		bool				create( Transform2dComponent& transformComponent );
		void				dispose();

		void				render( Renderer2d& renderer ) const;

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, TextureComponentState* pComponentState, const TextureComponentInitData* pComponentInitData ) TIKI_OVERRIDE_FINAL;
		virtual void		internalDisposeState( TextureComponentState* pComponentState ) TIKI_OVERRIDE_FINAL;

	private:

		const Transform2dComponent*	m_pTransformComponent;
	};
}

#endif // TIKI_TEXTURECOMPONENT_HPP_INCLUDED
