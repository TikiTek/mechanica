
#include "tiki/game/debuggui_lighting.hpp"

#include "tiki/renderer/renderercontext.hpp"

namespace tiki
{
	void DebugGuiLightingWindow::create( DebugGui& debugGui )
	{
		DebugGuiWindow::create( debugGui, "Lighting", m_layout );

		m_positionX.create( -10.0f, 10.0f, -0.1f );
		m_positionY.create( -10.0f, 10.0f, 0.1f );
		m_positionZ.create( -10.0f, 10.0f, 0.0f );
		m_range.create( 0.0f, 10.0f, 5.0f );
		m_theta.create( -f32::pi, f32::pi, 0.174532925f );
		m_phi.create( -f32::pi, f32::pi, 1.22173048f );

		m_layout.addChildControl( &m_positionX );
		m_layout.addChildControl( &m_positionY );
		m_layout.addChildControl( &m_positionZ );
		m_layout.addChildControl( &m_range );
		m_layout.addChildControl( &m_theta );
		m_layout.addChildControl( &m_phi );
	}

	void DebugGuiLightingWindow::dispose()
	{
		m_layout.removeChildControl( &m_positionX );
		m_layout.removeChildControl( &m_positionY );
		m_layout.removeChildControl( &m_positionZ );
		m_layout.removeChildControl( &m_range );
		m_layout.removeChildControl( &m_theta );
		m_layout.removeChildControl( &m_phi );

		m_positionX.dispose();
		m_positionY.dispose();
		m_positionZ.dispose();
		m_range.dispose();
		m_theta.dispose();
		m_phi.dispose();

		DebugGuiWindow::dispose();
	}

	void DebugGuiLightingWindow::fillFrameData( FrameData& frameData )
	{
		SpotLightData& spotLight = frameData.spotLights.push();
		vector::set( spotLight.position, m_positionX.getValue(), m_positionY.getValue(), m_positionZ.getValue() );
		vector::set( spotLight.direction, 0.5f, 0.5f, 0.0f );
		spotLight.color = TIKI_COLOR_WHITE;
		spotLight.range = m_range.getValue();
		spotLight.theta = m_theta.getValue();
		spotLight.phi	= m_phi.getValue();
	}
}