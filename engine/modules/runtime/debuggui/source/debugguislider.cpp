
#include "tiki/debuggui/debugguislider.hpp"

#include "tiki/debuggui/debuggui.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/input/inputevent.hpp"

namespace tiki
{
	void DebugGuiSlider::create( float minValue, float maxValue, float defaultValue )
	{
		m_minValue	= minValue;
		m_maxValue	= maxValue;
		m_value		= defaultValue;

		m_mouseOver = false;
		m_mouseDown = false;
	}

	void DebugGuiSlider::dispose()
	{
		m_minValue	= 0.0f;
		m_maxValue	= 0.0f;
		m_value		= 0.0f;

		m_mouseOver = false;
		m_mouseDown = false;
	}

	float DebugGuiSlider::getValue() const
	{
		return m_value;
	}

	void DebugGuiSlider::setValue( float value )
	{
		m_value = value;
		setSliderRectangle();
	}

	Vector2 DebugGuiSlider::getMinimumSize()
	{
		return vector::create( 25.0f, 25.0f );
	}

	void DebugGuiSlider::update( double elapsedTime )
	{
	}

	void DebugGuiSlider::render( ImmediateRenderer& renderer )
	{
		renderer.drawRectangle( m_line, TIKI_COLOR( 255, 255, 255, 128 ) );
		renderer.drawRectangle( m_slider, TIKI_COLOR( 255, 255, 255, 224 ) );
	}

	void DebugGuiSlider::handleRectangleChanged( const AxisAlignedRectangle& boundingRectangle )
	{
		const float yMid = boundingRectangle.getTop() + ( boundingRectangle.getHeight() / 2.0f );

		m_line = createAxisAlignedRectangle(
			boundingRectangle.getLeft() + getPadding().left,
			yMid - (getPadding().left / 2.0f),
			boundingRectangle.getWidth() - getPadding().getWidth(),
			getPadding().top
		);

		setSliderRectangle();
	}

	bool DebugGuiSlider::processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state )
	{
		if ( inputEvent.eventType == InputEventType_Mouse_Moved )
		{
			m_mouseOver = m_slider.contains( state.mousePosition );

			if ( m_mouseDown )
			{
				const float normalizeValue = f32::clamp( ( state.mousePosition.x - getRectangle().getLeft() ) / getRectangle().getWidth(), 0.0f, 1.0f );
				m_value = m_minValue + ( normalizeValue * ( m_maxValue - m_minValue ) );
				setSliderRectangle();
				pushEvent( DebugGuiEvent( this, DebugGuiEventType_ValueChanged ) );
			}
		}
		else if ( inputEvent.eventType == InputEventType_Mouse_ButtonDown && m_mouseOver )
		{
			m_mouseDown = true;
			return true;
		}
		else if ( inputEvent.eventType == InputEventType_Mouse_ButtonUp && m_mouseDown )
		{
			m_mouseDown = false;
			return true;
		}

		return false;
	}

	void DebugGuiSlider::setSliderRectangle()
	{
		const float yMid = getRectangle().getTop() + ( getRectangle().getHeight() / 2.0f );
		const float xSlider = getRectangle().getLeft() + getPadding().left + ( ( ( m_value - m_minValue ) / ( m_maxValue - m_minValue ) ) * ( getRectangle().getWidth() - getPadding().getWidth() ) );

		const float sliderHeight = TIKI_MIN( 25.0f, getRectangle().getHeight() );
		m_slider = createAxisAlignedRectangle(
			xSlider - getPadding().left,
			yMid - (sliderHeight / 2.0f),
			getPadding().getWidth(),
			sliderHeight
		);
	}
}
