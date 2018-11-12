#pragma once
#ifndef __TIKI_INPUTSYSTEM_WIN32_HPP_INCLUDED__
#define __TIKI_INPUTSYSTEM_WIN32_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/container/sortedsizedmap.hpp"

struct SDL_KeyboardEvent;
struct SDL_MouseButtonEvent;
struct SDL_MouseMotionEvent;
union SDL_Event;

namespace tiki
{
	class InputSystem;
	struct InputSystemState;

	enum
	{
		MaxInputTouchPoints			= 16u,
		InvalidTouchInputMapping	= 0xffffffffu
	};

	struct InputSystemPlatformData
	{
		InputSystemPlatformData()
		{
		}

		SortedSizedMap< int, KeyboardKey > keyboardMapping;
	};

	class InputSystemPlatform
	{
	public:

		static void		processEvent( InputSystem& inputSystem, const SDL_Event& sdlEvent );

	private:

		static void		processKeyEvent( InputSystem& inputSystem, const SDL_KeyboardEvent& keyboardEvent );
		static void		processMouseMotionEvent( InputSystem& inputSystem, const SDL_MouseMotionEvent& mouseMotionEvent );
		static void		processMouseButtonEvent( InputSystem& inputSystem, const SDL_MouseButtonEvent& mouseButtonEvent );

	};
}

#endif // __TIKI_INPUTSYSTEM_WIN32_HPP_INCLUDED__
