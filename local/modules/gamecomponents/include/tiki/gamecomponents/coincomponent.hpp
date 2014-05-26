#pragma once
#ifndef __TIKI_COINCOMPONENT_HPP_INCLUDED__
#define __TIKI_COINCOMPONENT_HPP_INCLUDED__

#include "tiki/components/component.hpp"

#include "tiki/base/fixedsizedarray.hpp"

namespace tiki
{
	class LifeTimeComponent;
	class PhysicsBodyComponent;
	class PhysicsCollisionObject;
	class PhysicsWorld;
	class TransformComponent;
	struct CoinComponentInitData;
	struct CoinComponentState;

	typedef FixedSizedArray< EntityId, 4u > CollectedCoinIdArray;

	class CoinComponent : public Component< CoinComponentState, CoinComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( CoinComponent );

	public:

		explicit			CoinComponent();
		virtual				~CoinComponent();

		bool				create( const TransformComponent& transformComponent, const PhysicsBodyComponent& physicsBodyComponent, const LifeTimeComponent& lifeTimeComponent, const PhysicsWorld& physicsWorld );
		void				dispose();

		void				update( const PhysicsCollisionObject& playerCollider, CollectedCoinIdArray& collectedCoins, float totalGameTime );

		virtual crc32		getTypeCrc() const;
		virtual uint32		getStateSize() const;
		virtual const char*	getTypeName() const;

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, CoinComponentState* pComponentState, const CoinComponentInitData* pComponentInitData );
		virtual void		internalDisposeState( CoinComponentState* pComponentState );

	private:

		const TransformComponent*		m_pTransformComponent;
		const PhysicsBodyComponent*		m_pPhysicsBodyComponent;
		const LifeTimeComponent*		m_pLifeTimeComponent;

		const PhysicsWorld*				m_pPhysicsWorld;

	};
}

#endif // __TIKI_COINCOMPONENT_HPP_INCLUDED__
