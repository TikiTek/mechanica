#pragma once
#ifndef __TIKI_COINCOMPONENT_HPP_INCLUDED__
#define __TIKI_COINCOMPONENT_HPP_INCLUDED__

#include "tiki/components/component.hpp"

#include "tiki/container/fixedsizedarray.hpp"

namespace tiki
{
	class LifeTimeComponent;
	class Physics3dBodyComponent;
	class Physics3dCollisionObject;
	class Physics3dWorld;
	class Transform3dComponent;
	struct CoinComponentInitData;
	struct CoinComponentState;

	typedef FixedSizedArray< EntityId, 4u > CollectedCoinIdArray;

	class CoinComponent : public Component< CoinComponentState, CoinComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( CoinComponent );

	public:

		explicit			CoinComponent();
		virtual				~CoinComponent();

		bool				create( const Transform3dComponent& transformComponent, const Physics3dBodyComponent& physicsBodyComponent, const LifeTimeComponent& lifeTimeComponent, const Physics3dWorld& physicsWorld );
		void				dispose();

		void				update( const Physics3dCollisionObject* pPlayerCollider, CollectedCoinIdArray& collectedCoins, float totalGameTime );

		virtual crc32		getTypeCrc() const;
		virtual uint32		getStateSize() const;
		virtual const char*	getTypeName() const;

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, CoinComponentState* pComponentState, const CoinComponentInitData* pComponentInitData );
		virtual void		internalDisposeState( CoinComponentState* pComponentState );

	private:

		const Transform3dComponent*		m_pTransformComponent;
		const Physics3dBodyComponent*	m_pPhysicsBodyComponent;
		const LifeTimeComponent*		m_pLifeTimeComponent;

		const Physics3dWorld*			m_pPhysicsWorld;

	};
}

#endif // __TIKI_COINCOMPONENT_HPP_INCLUDED__
