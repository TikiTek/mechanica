#pragma once
#ifndef TIKI_BREAKABLECOMPONENT_HPP_INCLUDED
#define TIKI_BREAKABLECOMPONENT_HPP_INCLUDED

#include "tiki/components/component.hpp"

namespace tiki
{
	class EntitySystem;
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

		bool				create( EntitySystem& entitySystem, Physics2dWorld& physicsWorld, const Transform2dComponent& transformComponent, const Physics2dBodyComponent& physicsBodyComponent );
		void				dispose();

		void				update( float deltaTime );

		void				breakBody( BreakableComponentState* pState );

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& iterator, BreakableComponentState* pState, const BreakableComponentInitData* pInitData ) TIKI_OVERRIDE_FINAL;
		virtual void		internalDisposeState( BreakableComponentState* pState ) TIKI_OVERRIDE_FINAL;

	private:

		EntitySystem*					m_pEntitySystem;

		Physics2dWorld*					m_pPhysicsWorld;

		const Transform2dComponent*		m_pTransformComponent;
		const Physics2dBodyComponent*	m_pPhysicsBodyComponent;

		void							createFacgmentEntities( const ResArray< BreakableFragment >& fragments );
	};
}

#endif // TIKI_BREAKABLECOMPONENT_HPP_INCLUDED
