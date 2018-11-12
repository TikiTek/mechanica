#pragma once

#include "tiki/base/types.hpp"
#include "tiki/container/fixed_array.hpp"
#include "tiki/container/fixed_sized_array.hpp"
#include "tiki/input/input_system.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	class GraphicsSystem;
	class ImmediateRenderer;
	class ResourceManager;
	class Texture;

	class TouchGameSystem
	{
		TIKI_NONCOPYABLE_CLASS( TouchGameSystem );

	public:

		TouchGameSystem();
		~TouchGameSystem();

		bool				create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );
		void				dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );

		void				update( float timeDelta );
		void				render( const ImmediateRenderer& renderer ) const;

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
	};
}
