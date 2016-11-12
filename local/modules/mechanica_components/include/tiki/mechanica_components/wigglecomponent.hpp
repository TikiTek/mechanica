#pragma once
#ifndef TIKI_WIGGLECOMPONENT_HPP_INCLUDED
#define TIKI_WIGGLECOMPONENT_HPP_INCLUDED

#include "tiki/components/component.hpp"

#include "tiki/container/fixedsizedarray.hpp"

namespace tiki
{
	class Physics2dBodyComponent;
	class Physics2dWorld;
	class Transform2dComponent;
	struct WiggleComponentInitData;
	struct WiggleComponentState;

	typedef FixedSizedArray< EntityId, 4u > CollectedWiggleIdArray;

	class WiggleComponent : public Component< WiggleComponentState, WiggleComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( WiggleComponent );

	public:

		explicit			WiggleComponent();
		virtual				~WiggleComponent();

		bool				create( Physics2dWorld& physicsWorld, const Transform2dComponent& transformComponent, const Physics2dBodyComponent& physicsBodyComponent );
		void				dispose();

	protected:

		virtual bool		internalInitializeState( const ComponentEntityIterator& componentIterator, WiggleComponentState* pComponentState, const WiggleComponentInitData* pComponentInitData ) TIKI_OVERRIDE_FINAL;
		virtual void		internalDisposeState( WiggleComponentState* pComponentState ) TIKI_OVERRIDE_FINAL;

	private:

		Physics2dWorld*					m_pPhysicsWorld;

		const Transform2dComponent*		m_pTransformComponent;
		const Physics2dBodyComponent*	m_pPhysicsBodyComponent;


	};
}

#endif // TIKI_WIGGLECOMPONENT_HPP_INCLUDED
