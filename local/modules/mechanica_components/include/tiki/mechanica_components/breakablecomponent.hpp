#pragma once
#ifndef TIKI_BREAKABLECOMPONENT_HPP_INCLUDED
#define TIKI_BREAKABLECOMPONENT_HPP_INCLUDED

#include "tiki/components/component.hpp"

namespace tiki
{
	class Physics2dBodyComponent;
	class Physics2dWorld;
	class Transform2dComponent;
	struct BreakableComponentInitData;
	struct BreakableComponentState;

	class BreakableComponent : public Component< BreakableComponentState, BreakableComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( BreakableComponent );

	public:

		explicit			BreakableComponent();
		virtual				~BreakableComponent();

		bool				create( Physics2dWorld& physicsWorld, const Transform2dComponent& transformComponent, const Physics2dBodyComponent& physicsBodyComponent );
		void				dispose();

		void				update( float deltaTime );

		void				breakBody( BreakableComponentState* pState );

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& iterator, BreakableComponentState* pState, const BreakableComponentInitData* pInitData ) TIKI_OVERRIDE_FINAL;
		virtual void		internalDisposeState( BreakableComponentState* pState ) TIKI_OVERRIDE_FINAL;

	private:

		Physics2dWorld*					m_pPhysicsWorld;

		const Transform2dComponent*		m_pTransformComponent;
		const Physics2dBodyComponent*	m_pPhysicsBodyComponent;
	};
}

#endif // TIKI_BREAKABLECOMPONENT_HPP_INCLUDED
