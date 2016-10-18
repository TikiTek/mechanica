#pragma once
#ifndef TIKI_PHYSICS2DCOLLIDERCOMPONENT_HPP_INCLUDED
#define TIKI_PHYSICS2DCOLLIDERCOMPONENT_HPP_INCLUDED

#include "tiki/components/component.hpp"

namespace tiki
{
	class Physics2dCollisionObject;
	class Physics2dWorld;
	struct Physics2dColliderComponentInitData;
	struct Physics2dColliderComponentState;

	class Physics2dColliderComponent : public Component< Physics2dColliderComponentState, Physics2dColliderComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dColliderComponent );

	public:

		explicit						Physics2dColliderComponent();
		virtual							~Physics2dColliderComponent();

		bool							create( Physics2dWorld& physicsWorld );
		void							dispose();

		const Physics2dCollisionObject&	getPhysicsObject( const Physics2dColliderComponentState* pState ) const;

		virtual crc32					getTypeCrc() const;
		virtual uint32					getStateSize() const;
		virtual const char*				getTypeName() const;

	protected:

		virtual bool					internalInitializeState( ComponentEntityIterator& componentIterator, Physics2dColliderComponentState* pComponentState, const Physics2dColliderComponentInitData* pComponentInitData );
		virtual void					internalDisposeState( Physics2dColliderComponentState* pComponentState );

	private:

		Physics2dWorld*					m_pPhysicsWorld;
	};
}

#endif // TIKI_PHYSICS2DCOLLIDERCOMPONENT_HPP_INCLUDED
