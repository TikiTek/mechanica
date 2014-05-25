#pragma once
#ifndef __TIKI_COINCOMPONENT_HPP_INCLUDED__
#define __TIKI_COINCOMPONENT_HPP_INCLUDED__

#include "tiki/components/component.hpp"

namespace tiki
{
	class LifeTimeComponent;
	class TransformComponent;
	struct CoinComponentInitData;
	struct CoinComponentState;

	class CoinComponent : public Component< CoinComponentState, CoinComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( CoinComponent );

	public:

		explicit			CoinComponent();
		virtual				~CoinComponent();

		bool				create( const TransformComponent& transformComponent, const LifeTimeComponent& lifeTimeComponent );
		void				dispose();

		void				update( float totalGameTime );

		virtual crc32		getTypeCrc() const;
		virtual uint32		getStateSize() const;
		virtual const char*	getTypeName() const;

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, CoinComponentState* pComponentState, const CoinComponentInitData* pComponentInitData );
		virtual void		internalDisposeState( CoinComponentState* pComponentState );

	private:

		const TransformComponent*	m_pTransformComponent;
		const LifeTimeComponent*	m_pLifeTimeComponent;

	};
}

#endif // __TIKI_COINCOMPONENT_HPP_INCLUDED__
