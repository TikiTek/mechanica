#pragma once
#ifndef __TIKI_DEBUGGUISLIDER_HPP_INCLUDED__
#define __TIKI_DEBUGGUISLIDER_HPP_INCLUDED__

#include "tiki/debuggui/debugguislider.hpp"

namespace tiki
{
	class DebugGuiSlider : public DebugGuiControl
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiSlider );

	public:

		void			create( float minValue, float maxValue, float defaultValue );
		void			dispose();

		float			getValue() const;
		void			setValue( float value );

		virtual Vector2	getMinimumSize();

		virtual void	update();
		virtual void	render( ImmediateRenderer& renderer );

		virtual bool	processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state );

	protected:

		virtual void	handleRectangleChanged( const Rectangle& boundingRectangle );

	private:

		float			m_minValue;
		float			m_maxValue;
		float			m_value;

		bool			m_mouseOver;
		bool			m_mouseDown;

		Rectangle		m_line;
		Rectangle		m_slider;

		void			setSliderRectangle();

	};
}

#endif // __TIKI_DEBUGGUISLIDER_HPP_INCLUDED__
