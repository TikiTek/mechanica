#pragma once
#ifndef __TIKI_TRANSFORMCOMPONENT_HPP_INCLUDED__
#define __TIKI_TRANSFORMCOMPONENT_HPP_INCLUDED__

#include "tiki/components/component.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	struct TransformComponentInitData;

	struct TransformComponentState : public ComponentState
	{
		Vector3		position;
		Quaternion	rotation;
	};

	class TransformComponent : public Component< TransformComponentState, TransformComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( TransformComponent );

	public:

		explicit			TransformComponent();
		virtual				~TransformComponent();

		bool				create();
		void				dispose();

		void				update( float timeStep );

		virtual crc32		getTypeCrc() const;
		virtual uint32		getStateSize() const;
		virtual const char*	getTypeName() const;

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, TransformComponentState* pComponentState, const TransformComponentInitData* pComponentInitData );
		virtual void		internalDisposeState( TransformComponentState* pComponentState );

	};
}

#endif // __TIKI_TRANSFORMCOMPONENT_HPP_INCLUDED__
