#pragma once
#ifndef __TIKI_PHYSICSCOLLIDERCOMPONENT_HPP_INCLUDED__
#define __TIKI_PHYSICSCOLLIDERCOMPONENT_HPP_INCLUDED__

#include "tiki/components/component.hpp"

namespace tiki
{
	class PhysicsWorld;
	struct PhysicsColliderComponentInitData;
	struct PhysicsColliderComponentState;

	class PhysicsColliderComponent : public Component< PhysicsColliderComponentState, PhysicsColliderComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsColliderComponent );

	public:

		explicit			PhysicsColliderComponent();
		virtual				~PhysicsColliderComponent();

		bool				create( PhysicsWorld& physicsWorld );
		void				dispose();

		virtual crc32		getTypeCrc() const;
		virtual uint32		getStateSize() const;
		virtual const char*	getTypeName() const;

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, PhysicsColliderComponentState* pComponentState, const PhysicsColliderComponentInitData* pComponentInitData );
		virtual void		internalDisposeState( PhysicsColliderComponentState* pComponentState );

	private:

		PhysicsWorld*		m_pWorld;

	};
}

#endif // __TIKI_PHYSICSCOLLIDERCOMPONENT_HPP_INCLUDED__
