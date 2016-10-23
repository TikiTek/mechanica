#pragma once
#ifndef TIKI_PHYSICS2DBODYCOMPONENT_HPP_INCLUDED
#define TIKI_PHYSICS2DBODYCOMPONENT_HPP_INCLUDED

#include "tiki/components/component.hpp"

#include "tiki/math/vector.hpp"

namespace tiki
{
	class Physics2dBody;
	class Physics2dWorld;
	class Transform2dComponent;
	struct Physics2dBodyComponentInitData;
	struct Physics2dBodyComponentState;

	class Physics2dBodyComponent : public Component< Physics2dBodyComponentState, Physics2dBodyComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dBodyComponent );

	public:

		explicit						Physics2dBodyComponent();
		virtual							~Physics2dBodyComponent();

		bool							create( Physics2dWorld& physicsWorld, const Transform2dComponent& transformComponent );
		void							dispose();

		void							update();

		void							applyForce( Physics2dBodyComponentState* pState, const Vector2& force, const Vector2& localPoint = Vector2::zero ) const;

		const Physics2dBody&			getPhysicsObject( const Physics2dBodyComponentState* pState ) const;

	protected:

		virtual bool					internalInitializeState( ComponentEntityIterator& componentIterator, Physics2dBodyComponentState* pComponentState, const Physics2dBodyComponentInitData* pComponentInitData ) TIKI_OVERRIDE_FINAL;
		virtual void					internalDisposeState( Physics2dBodyComponentState* pComponentState ) TIKI_OVERRIDE_FINAL;

	private:

		Physics2dWorld*					m_pPhysicsWorld;

		const Transform2dComponent*		m_pTranformComponent;
	};
}

#endif // TIKI_PHYSICS2DBODYCOMPONENT_HPP_INCLUDED
