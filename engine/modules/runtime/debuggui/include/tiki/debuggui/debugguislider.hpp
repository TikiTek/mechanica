#pragma once
#ifndef __TIKI_DEBUGGUISLIDER_HPP_INCLUDED__
#define __TIKI_DEBUGGUISLIDER_HPP_INCLUDED__

#include "tiki/debuggui/debugguicontrol.hpp"

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

		virtual Vector2	getMinimumSize() TIKI_OVERRIDE TIKI_FINAL;

		virtual void	update( double elapsedTime ) TIKI_OVERRIDE TIKI_FINAL;
		virtual void	render( ImmediateRenderer& renderer ) TIKI_OVERRIDE TIKI_FINAL;

		virtual bool	processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state ) TIKI_OVERRIDE TIKI_FINAL;

	protected:

		virtual void	handleRectangleChanged( const Rectangle& boundingRectangle ) TIKI_OVERRIDE TIKI_FINAL;

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
