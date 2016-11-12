#pragma once
#ifndef TIKI_SPRITECOMPONENT_HPP_INCLUDED
#define TIKI_SPRITECOMPONENT_HPP_INCLUDED

#include "tiki/components/component.hpp"

namespace tiki
{
	class Renderer2d;
	class Transform2dComponent;
	struct SpriteComponentInitData;
	struct SpriteComponentState;
	struct Vector2;

	class SpriteComponent : public Component< SpriteComponentState, SpriteComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( SpriteComponent );

	public:

		explicit			SpriteComponent();
		virtual				~SpriteComponent();

		bool				create( Transform2dComponent& transformComponent );
		void				dispose();

		void				render( Renderer2d& renderer ) const;

	protected:

		virtual bool		internalInitializeState( const ComponentEntityIterator& componentIterator, SpriteComponentState* pComponentState, const SpriteComponentInitData* pComponentInitData ) TIKI_OVERRIDE_FINAL;
		virtual void		internalDisposeState( SpriteComponentState* pComponentState ) TIKI_OVERRIDE_FINAL;

	private:

		const Transform2dComponent*	m_pTransformComponent;
	};
}

#endif // TIKI_SPRITECOMPONENT_HPP_INCLUDED
