#pragma once
#ifndef __TIKI_LIFETIMECOMPONENT_HPP_INCLUDED__
#define __TIKI_LIFETIMECOMPONENT_HPP_INCLUDED__

#include "tiki/components/component.hpp"

namespace tiki
{
	class EntitySystem;
	struct LifeTimeComponentInitData;
	struct LifeTimeComponentState;

	class LifeTimeComponent : public Component< LifeTimeComponentState, LifeTimeComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( LifeTimeComponent );

	public:

		explicit			LifeTimeComponent();
		virtual				~LifeTimeComponent();

		bool				create();
		void				dispose();

		void				update( EntitySystem& entitySystem, timems timeMs );

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, LifeTimeComponentState* pComponentState, const LifeTimeComponentInitData* pComponentInitData ) TIKI_OVERRIDE_FINAL;
		virtual void		internalDisposeState( LifeTimeComponentState* pComponentState ) TIKI_OVERRIDE_FINAL;
	};
}

#endif // __TIKI_LIFETIMECOMPONENT_HPP_INCLUDED__
