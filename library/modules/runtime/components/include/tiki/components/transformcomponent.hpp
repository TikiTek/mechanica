#pragma once
#ifndef __TIKI_TRANSFORMCOMPONENT_HPP_INCLUDED__
#define __TIKI_TRANSFORMCOMPONENT_HPP_INCLUDED__

#include "tiki/components/component.hpp"

namespace tiki
{
	struct Matrix43;
	struct Quaternion;
	struct TransformComponentInitData;
	struct TransformComponentState;
	struct Vector3;

	class TransformComponent : public Component< TransformComponentState, TransformComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( TransformComponent );

	public:

		explicit			TransformComponent();
		virtual				~TransformComponent();

		bool				create();
		void				dispose();

		void				update();

		void				getPosition( Vector3& targetPosition, const TransformComponentState* pState ) const;
		void				getRotation( Quaternion& targetRotation, const TransformComponentState* pState ) const;
		void				getWorldTransform( Matrix43& targetMatrix, const TransformComponentState* pState ) const;

		void				setPosition( TransformComponentState* pState, const Vector3& position ) const;
		void				setRotation( TransformComponentState* pState, const Quaternion& rotation ) const;

		virtual crc32		getTypeCrc() const;
		virtual uint32		getStateSize() const;
		virtual const char*	getTypeName() const;

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, TransformComponentState* pComponentState, const TransformComponentInitData* pComponentInitData );
		virtual void		internalDisposeState( TransformComponentState* pComponentState );

	};
}

#endif // __TIKI_TRANSFORMCOMPONENT_HPP_INCLUDED__
