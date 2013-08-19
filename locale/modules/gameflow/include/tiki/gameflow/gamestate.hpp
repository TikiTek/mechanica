#pragma once
#ifndef TIKI_GAMESTATE_HPP__INCLUDED
#define TIKI_GAMESTATE_HPP__INCLUDED

#include "tiki/base/types.hpp"

#include "tiki/gameflow/statetree.hpp"

namespace tiki
{
	class GpuContext;

	class GameState
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( GameState );

	public:

		virtual TransitionState	processTransitionStep( size_t currentStep, bool isCreating, bool isInital ) = 0;

		virtual void			update() = 0;
		virtual void			render( GpuContext* pCommandWriter ) const = 0;

	};
}

#endif // TIKI_GAMESTATE_HPP__INCLUDED
