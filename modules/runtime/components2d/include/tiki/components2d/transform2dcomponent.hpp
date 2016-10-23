#pragma once
#ifndef TIKI_TRANSFORM2DCOMPONENT_HPP_INCLUDED
#define TIKI_TRANSFORM2DCOMPONENT_HPP_INCLUDED

#include "tiki/components/component.hpp"

namespace tiki
{
	struct Matrix32;
	struct Transform2dComponentInitData;
	struct Transform2dComponentState;
	struct Vector2;

	class Transform2dComponent : public Component< Transform2dComponentState, Transform2dComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( Transform2dComponent );

	public:

		explicit			Transform2dComponent();
		virtual				~Transform2dComponent();

		bool				create();
		void				dispose();

		void				update();

		const Vector2&		getPosition( const Transform2dComponentState* pState ) const;
		float				getRotation( const Transform2dComponentState* pState ) const;
		const Matrix32&		getWorldTransform( const Transform2dComponentState* pState ) const;

		void				setPosition( Transform2dComponentState* pState, const Vector2& position ) const;
		void				setRotation( Transform2dComponentState* pState, float rotation ) const;

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, Transform2dComponentState* pComponentState, const Transform2dComponentInitData* pComponentInitData ) TIKI_OVERRIDE_FINAL;
		virtual void		internalDisposeState( Transform2dComponentState* pComponentState ) TIKI_OVERRIDE_FINAL;

	};
}

#endif // TIKI_TRANSFORM2DCOMPONENT_HPP_INCLUDED
