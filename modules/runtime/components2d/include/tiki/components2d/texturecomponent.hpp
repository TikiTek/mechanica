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

		virtual crc32		getTypeCrc() const TIKI_OVERRIDE_FINAL;
		virtual uint32		getStateSize() const TIKI_OVERRIDE_FINAL;
		virtual const char*	getTypeName() const TIKI_OVERRIDE_FINAL;

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, TextureComponentState* pComponentState, const TextureComponentInitData* pComponentInitData ) TIKI_OVERRIDE;
		virtual void		internalDisposeState( TextureComponentState* pComponentState ) TIKI_OVERRIDE;

	private:

		const Transform2dComponent*	m_pTransformComponent;
	};
}

#endif // TIKI_TEXTURECOMPONENT_HPP_INCLUDED
