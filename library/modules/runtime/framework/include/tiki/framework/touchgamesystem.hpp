#pragma once
#ifndef __TIKI_TOUCHGAMESYSTEM_HPP_INCLUDED__
#define __TIKI_TOUCHGAMESYSTEM_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/input/inputsystem.hpp"

namespace tiki
{
	class Texture;

	class TouchGameSystem
	{
		TIKI_NONCOPYABLE_CLASS( TouchGameSystem );

	public:

		TouchGameSystem();
		~TouchGameSystem();

		bool		create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );
		void		dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );

		void		update( float timeDelta );
		void		render( GraphicsContext& graphicsContext ) const;

		bool		processInputEvent( const InputEvent& inputEvent );

	private:

		struct TouchPoint
		{
			bool		isDown;
			bool		isOnPad;
			Vector2		position;
			float		scale;
		};

		typedef FixedArray< TouchPoint, MaxInputTouchPoints > TouchPointArray;

		TouchPointArray		m_touchPoints;

		const Texture*		m_pPadTexture;
		const Texture*		m_pPadPointTexture;
		const Texture*		m_pPointTexture;
		Vector2				m_pointSize;
		Vector2				m_halfPointSize;

		ImmediateRenderer	m_renderer;

	};
}

#endif // __TIKI_TOUCHGAMESYSTEM_HPP_INCLUDED__
