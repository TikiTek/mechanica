#pragma once
#ifndef TIKI_TRANSFORM3DCOMPONENT_HPP_INCLUDED
#define TIKI_TRANSFORM3DCOMPONENT_HPP_INCLUDED

#include "tiki/components/component.hpp"

namespace tiki
{
	struct Matrix43;
	struct Quaternion;
	struct Transform3dComponentInitData;
	struct Transform3dComponentState;
	struct Vector3;

	class Transform3dComponent : public Component< Transform3dComponentState, Transform3dComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( Transform3dComponent );

	public:

		explicit			Transform3dComponent();
		virtual				~Transform3dComponent();

		bool				create();
		void				dispose();

		void				update();

		void				getPosition( Vector3& targetPosition, const Transform3dComponentState* pState ) const;
		void				getRotation( Quaternion& targetRotation, const Transform3dComponentState* pState ) const;
		void				getWorldTransform( Matrix43& targetMatrix, const Transform3dComponentState* pState ) const;

		void				setPosition( Transform3dComponentState* pState, const Vector3& position ) const;
		void				setRotation( Transform3dComponentState* pState, const Quaternion& rotation ) const;

		virtual crc32		getTypeCrc() const;
		virtual uint32		getStateSize() const;
		virtual const char*	getTypeName() const;

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, Transform3dComponentState* pComponentState, const Transform3dComponentInitData* pComponentInitData );
		virtual void		internalDisposeState( Transform3dComponentState* pComponentState );

	};
}

#endif // TIKI_TRANSFORM3DCOMPONENT_HPP_INCLUDED
