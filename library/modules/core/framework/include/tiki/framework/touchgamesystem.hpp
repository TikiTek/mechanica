#pragma once
#ifndef __TIKI_TOUCHGAMESYSTEM_HPP_INCLUDED__
#define __TIKI_TOUCHGAMESYSTEM_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/input/inputsystem.hpp"
#include "tiki/base/fixedarray.hpp"
#include "tiki/base/fixedsizedarray.hpp"

namespace tiki
{
	class Texture;

	class TouchGameSystem
	{
		TIKI_NONCOPYABLE_CLASS( TouchGameSystem );

	public:

		TouchGameSystem();
		~TouchGameSystem();

		bool				create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );
		void				dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );

		void				update( float timeDelta, const GraphicsSystem& graphicsSystem );
		void				render( GraphicsContext& graphicsContext ) const;

		bool				processInputEvent( const InputEvent& inputEvent );

		uint				getInputEventCount() const					{ return m_inputEvents.getCount(); }
		const InputEvent&	getInputEventByIndex( uint index ) const	{ return m_inputEvents[ index ]; }

	private:

		enum
		{
			PadCount					= 2u,

			MaxTouchInputEventsPerFrame = 16u
		};

		struct TouchPoint
		{
			bool		isDown;
			bool		isOnPad[ PadCount ];
			Vector2		position;
			float		scale;
		};

		typedef FixedArray< TouchPoint, MaxInputTouchPoints > TouchPointArray;
		typedef FixedSizedArray< InputEvent, MaxTouchInputEventsPerFrame > InputEventArray;

		bool					m_isEnabled;

		const GraphicsSystem*	m_pGraphicsSystem;

		TouchPointArray			m_touchPoints;
		InputEventArray			m_inputEvents;

		const Texture*			m_pPadTexture;
		const Texture*			m_pPadPointTexture;
		const Texture*			m_pPointTexture;

		Vector2					m_pointSize;
		Vector2					m_halfPointSize;
		Vector2					m_padSize;
		Vector2					m_halfPadSize;

		ImmediateRenderer		m_renderer;

	};
}

#endif // __TIKI_TOUCHGAMESYSTEM_HPP_INCLUDED__
