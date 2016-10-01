#pragma once
#ifndef TIKI_PHYSICS3DCOLLIDERCOMPONENT_HPP_INCLUDED
#define TIKI_PHYSICS3DCOLLIDERCOMPONENT_HPP_INCLUDED

#include "tiki/components/component.hpp"

namespace tiki
{
	class Physics3dCollisionObject;
	class Physics3dWorld;
	struct Physics3dColliderComponentInitData;
	struct Physics3dColliderComponentState;

	class Physics3dColliderComponent : public Component< Physics3dColliderComponentState, Physics3dColliderComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( Physics3dColliderComponent );

	public:

		explicit						Physics3dColliderComponent();
		virtual							~Physics3dColliderComponent();

		bool							create( Physics3dWorld& physicsWorld );
		void							dispose();

		const Physics3dCollisionObject&	getPhysicsObject( const Physics3dColliderComponentState* pState ) const;

		virtual crc32					getTypeCrc() const;
		virtual uint32					getStateSize() const;
		virtual const char*				getTypeName() const;

	protected:

		virtual bool					internalInitializeState( ComponentEntityIterator& componentIterator, Physics3dColliderComponentState* pComponentState, const Physics3dColliderComponentInitData* pComponentInitData );
		virtual void					internalDisposeState( Physics3dColliderComponentState* pComponentState );

	private:

		Physics3dWorld*					m_pWorld;
	};
}

#endif // TIKI_PHYSICS3DCOLLIDERCOMPONENT_HPP_INCLUDED
